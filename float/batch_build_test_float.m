function batch_build_test_float(varargin)
% batch_build_test_float  编译/测试 float 目录下的浮点 MEX 函数
%
%    batch_build_test_float              编译 + 测试 (默认)
%    batch_build_test_float all          编译 + 测试
%    batch_build_test_float compile      仅编译所有
%    batch_build_test_float test         仅测试所有 (需已有 .mexw64)
%    batch_build_test_float failed       仅重新编译和测试失败项 (读取日志)
%    batch_build_test_float folderName   编译+测试指定文件夹 (如 basicmath)
%    batch_build_test_float compile folderName  仅编译指定文件夹
%    batch_build_test_float test   folderName  仅测试指定文件夹
%
% 结果日志: float/batch_test_results.txt
% 编译日志: float/batch_build_log.txt
%
% 提示: 在任意路径运行此脚本，它会自动定位到自身所在目录。

    % --- 自动定位到脚本所在目录 ---
    script_dir = fileparts(mfilename('fullpath'));
    cd(script_dir);
    % script_dir 就是 float/ 目录

    c_list = dir(fullfile(script_dir, '**', '*_mex.c'));
    if isempty(c_list)
        fprintf('No *_mex.c files found under %s\n', script_dir);
        return;
    end
    N = length(c_list);

    % --- 解析命令和文件夹筛选 ---
    if nargin < 1
        cmd = 'all';
        folder_filter = '';
    else
        first = lower(varargin{1});
        switch first
            case {'all', 'a'},       cmd = 'all';
            case {'compile', 'c'},   cmd = 'compile';
            case {'test', 't'},      cmd = 'test';
            case {'failed', 'f'},    cmd = 'failed';
            otherwise
                cmd = 'all';
                folder_filter = first;
        end
        if ~exist('folder_filter', 'var')
            if nargin >= 2
                folder_filter = lower(varargin{2});
            else
                folder_filter = '';
            end
        end
    end

    % --- 应用文件夹筛选 ---
    if ~isempty(folder_filter)
        keep = false(N, 1);
        for i = 1:N
            if contains(lower(c_list(i).folder), folder_filter)
                keep(i) = true;
            end
        end
        c_list = c_list(keep);
        N = length(c_list);
        if N == 0
            fprintf('文件夹 "%s" 下未找到 *_mex.c 文件\n', folder_filter);
            return;
        end
        fprintf('筛选文件夹 "%s" -> %d 个文件\n', folder_filter, N);
    end

    % --- failed 模式: 从日志中筛选失败项 ---
    if strcmp(cmd, 'failed')
        failed_names = parse_failed_list(script_dir);
        if isempty(failed_names)
            fprintf('没有发现失败记录，无需重试。\n');
            return;
        end
        keep = false(N, 1);
        for i = 1:N
            [~, func_name, ~] = fileparts(c_list(i).name);
            test_name = strrep(func_name, '_mex', '');
            if ismember(test_name, failed_names)
                keep(i) = true;
            end
        end
        c_list = c_list(keep);
        N = length(c_list);
        fprintf('失败项: %d 个文件需要重新编译和测试\n', N);
    end

    % --- 阶段1: 编译 ---
    if ismember(cmd, {'all','compile','failed'})
        build_status = compile_all(c_list, N, script_dir);
    else
        build_status = check_existing_mex(c_list, N);
    end

    % --- 阶段2: 测试 ---
    if ismember(cmd, {'all','test','failed'})
        test_all(c_list, N, build_status, script_dir);
    end
end

% ======================================================================
function failed_names = parse_failed_list(script_dir)
    failed_names = {};

    build_log = fullfile(script_dir, 'batch_build_log.txt');
    if isfile(build_log)
        fid = fopen(build_log, 'r');
        if fid > 0
            while ~feof(fid)
                line = strtrim(fgetl(fid));
                if isempty(line), continue; end
                tokens = regexp(line, '\[\s*\d+/\d+\]\s+(\S+)\s+FAIL', 'tokens');
                if ~isempty(tokens)
                    name = strrep(tokens{1}{1}, '_mex', '');
                    if ~ismember(name, failed_names)
                        failed_names{end+1} = name;
                    end
                end
            end
            fclose(fid);
        end
    end

    test_log = fullfile(script_dir, 'batch_test_results.txt');
    if isfile(test_log)
        fid = fopen(test_log, 'r');
        if fid > 0
            while ~feof(fid)
                line = strtrim(fgetl(fid));
                if isempty(line), continue; end
                tokens = regexp(line, '\[\s*\d+/\d+\]\s+(\S+)\s+(FAIL|ERROR|SKIP)', 'tokens');
                if ~isempty(tokens)
                    name = strrep(tokens{1}{1}, '_mex', '');
                    if ~ismember(name, failed_names)
                        failed_names{end+1} = name;
                    end
                end
            end
            fclose(fid);
        end
    end

    if ~isempty(failed_names)
        fprintf('\n从日志中读取到 %d 个失败/异常项:\n', length(failed_names));
        for i = 1:length(failed_names)
            fprintf('  %d. %s\n', i, failed_names{i});
        end
        fprintf('\n');
    end
end

% ======================================================================
function build_status = compile_all(c_list, N, script_dir)
    log_file = fullfile(script_dir, 'batch_build_log.txt');

    fid = fopen(log_file, 'w');
    fprintf(fid, '=== MEX 编译日志 (float) ===  %s\n\n', datestr(now));

    fprintf('============================================================\n');
    fprintf('  编译: %d 个 *_mex.c (float)\n', N);
    fprintf('============================================================\n');

    build_status = repmat(struct('name','','ok',false,'err',''), N, 1);
    t_start = tic;

    for i = 1:N
        c_path = fullfile(c_list(i).folder, c_list(i).name);
        [~, func_name, ~] = fileparts(c_list(i).name);
        out_dir = c_list(i).folder;
        mex_path = fullfile(out_dir, [func_name, '.', mexext]);
        build_status(i).name = func_name;

        if isfile(mex_path)
            try delete(mex_path); catch; end
        end

        fprintf('[%3d/%3d] %-55s ', i, N, func_name);
        fprintf(fid, '[%3d/%3d] %s ', i, N, func_name);

        try
            mex('-outdir', out_dir, c_path);
            build_status(i).ok = true;
            fprintf('OK\n');
            fprintf(fid, 'OK\n');
        catch ME
            build_status(i).ok = false;
            build_status(i).err = ME.message;
            fprintf('FAIL\n');
            fprintf(fid, 'FAIL\n    %s\n', strrep(ME.message, newline, [newline '    ']));
        end
        drawnow('update');
    end

    elapsed = toc(t_start);
    n_ok = sum([build_status.ok]);
    n_fail = sum(~[build_status.ok]);

    fprintf('\n  编译完成: %d 成功, %d 失败  (%.1f s)\n', n_ok, n_fail, elapsed);
    fprintf(fid, '\n总计: %d 成功, %d 失败  (%.1f s)\n', n_ok, n_fail, elapsed);

    if n_fail > 0
        fprintf('\n--- 编译失败 ---\n');
        fprintf(fid, '\n--- 编译失败 ---\n');
        for i = 1:N
            if ~build_status(i).ok
                msg = strrep(build_status(i).err, newline, [newline '    ']);
                fprintf('  %s\n    %s\n', build_status(i).name, msg);
                fprintf(fid, '  %s\n    %s\n', build_status(i).name, msg);
            end
        end
    end
    fclose(fid);
    fprintf('\n日志: %s\n', log_file);
end

% ======================================================================
function build_status = check_existing_mex(c_list, N)
    build_status = repmat(struct('name','','ok',false,'err',''), N, 1);
    for i = 1:N
        [~, func_name, ~] = fileparts(c_list(i).name);
        mex_path = fullfile(c_list(i).folder, [func_name, '.', mexext]);
        build_status(i).name = func_name;
        build_status(i).ok = isfile(mex_path);
        if ~build_status(i).ok
            build_status(i).err = 'mexw64 not found';
        end
    end
end

% ======================================================================
function test_all(c_list, N, build_status, script_dir)
    log_file = fullfile(script_dir, 'batch_test_results.txt');

    fid = fopen(log_file, 'w');
    fprintf(fid, '=== MEX 测试结果 (float) ===  %s\n', datestr(now));
    fprintf(fid, '格式:  [序号] 函数名  结果\n\n');

    fprintf('\n============================================================\n');
    fprintf('  测试阶段 (float)\n');
    fprintf('============================================================\n\n');

    orig_path = path;
    t_start = tic;
    results = cell(N, 1);

    for i = 1:N
        [~, func_name, ~] = fileparts(c_list(i).name);
        test_func = strrep(func_name, '_mex', '');
        test_dir = c_list(i).folder;
        mex_path = fullfile(test_dir, [func_name, '.', mexext]);

        if ~build_status(i).ok || ~isfile(mex_path)
            results{i} = struct('name',test_func,'result','SKIP','detail','no mexw64');
            fprintf('[%3d/%3d] %-55s SKIP (no mexw64)\n', i, N, test_func);
            fprintf(fid, '[%3d/%3d] %-55s SKIP  (no mexw64)\n', i, N, test_func);
            continue;
        end

        path(orig_path);
        addpath(test_dir);
        clear mex;
        clear functions;

        fprintf('[%3d/%3d] %-55s ', i, N, test_func);

        try
            output = evalc([test_func '();']);

            if contains(output, 'All tests PASSED') || ...
               contains(output, 'PASSED') || ...
               (contains(output, 'PASS', 'IgnoreCase', true) && ...
                ~contains(output, 'FAIL', 'IgnoreCase', true))
                results{i} = struct('name',test_func,'result','PASS','detail','');
                fprintf('PASS\n');
                fprintf(fid, '[%3d/%3d] %-55s PASS\n', i, N, test_func);
            elseif contains(output, 'FAIL')
                results{i} = struct('name',test_func,'result','FAIL','detail',output);
                fprintf('FAIL\n');
                fprintf(fid, '[%3d/%3d] %-55s FAIL\n', i, N, test_func);
                out_lines = strsplit(output, newline);
                detail_count = 0;
                for k = 1:min(length(out_lines), 12)
                    line = strtrim(out_lines{k});
                    if ~isempty(line)
                        fprintf(fid, '  detail: %s\n', line);
                        detail_count = detail_count + 1;
                    end
                end
                if detail_count == 0
                    fprintf(fid, '  detail: (no output)\n');
                end
                for k = 1:min(length(out_lines), 8)
                    line = strtrim(out_lines{k});
                    if ~isempty(line), fprintf('  > %s\n', line); end
                end
            elseif contains(output, 'SKIP') || contains(output, 'SKIPPED')
                results{i} = struct('name',test_func,'result','SKIP','detail','');
                fprintf('SKIP\n');
                fprintf(fid, '[%3d/%3d] %-55s SKIP\n', i, N, test_func);
            else
                results{i} = struct('name',test_func,'result','CHECK','detail',output);
                fprintf('CHECK\n');
                fprintf(fid, '[%3d/%3d] %-55s CHECK\n', i, N, test_func);
                out_lines = strsplit(output, newline);
                for k = 1:min(length(out_lines), 5)
                    line = strtrim(out_lines{k});
                    if ~isempty(line), fprintf('  > %s\n', line); end
                end
            end

        catch ME
            if any(contains(ME.identifier, ...
                    {'MATLAB:UndefinedFunction','MATLAB:mex:NoCompiler'}))
                results{i} = struct('name',test_func,'result','SKIP','detail',ME.message);
                fprintf('SKIP (%s)\n', ME.message);
                fprintf(fid, '[%3d/%3d] %-55s SKIP  (%s)\n', i, N, test_func, ME.message);
            else
                stack_str = ME.getReport('extended', 'hyperlinks', 'off');
                results{i} = struct('name',test_func,'result','ERROR','detail',stack_str);
                fprintf('ERROR (%s)\n', ME.message);
                fprintf(fid, '[%3d/%3d] %-55s ERROR (%s)\n', i, N, test_func, ME.message);
                fprintf(fid, '  detail: %s\n', strrep(ME.message, newline, [newline '  detail: ']));
                st_lines = strsplit(stack_str, newline);
                for k = 1:min(length(st_lines), 12)
                    line = strtrim(st_lines{k});
                    if ~isempty(line), fprintf('  > %s\n', line); end
                end
            end
        end

        drawnow('update');
    end

    path(orig_path);
    elapsed = toc(t_start);

    n_pass  = sum(strcmp(cellfun(@(r)r.result, results, 'UniformOutput',false), 'PASS'));
    n_fail  = sum(strcmp(cellfun(@(r)r.result, results, 'UniformOutput',false), 'FAIL'));
    n_err   = sum(strcmp(cellfun(@(r)r.result, results, 'UniformOutput',false), 'ERROR'));
    n_skip  = sum(strcmp(cellfun(@(r)r.result, results, 'UniformOutput',false), 'SKIP'));
    n_check = sum(strcmp(cellfun(@(r)r.result, results, 'UniformOutput',false), 'CHECK'));

    fprintf('\n');
    fprintf('========================================\n');
    fprintf('  测试汇总 (float)\n');
    fprintf('========================================\n');
    fprintf('  总: %d  PASS: %d  FAIL: %d  ERROR: %d  SKIP: %d  CHECK: %d\n', ...
        N, n_pass, n_fail, n_err, n_skip, n_check);
    fprintf('  耗时: %.1f s\n', elapsed);
    fprintf('========================================\n');

    fprintf(fid, '\n');
    fprintf(fid, '========================================\n');
    fprintf(fid, '  测试汇总 (float)\n');
    fprintf(fid, '========================================\n');
    fprintf(fid, '  总: %d  PASS: %d  FAIL: %d  ERROR: %d  SKIP: %d  CHECK: %d\n', ...
        N, n_pass, n_fail, n_err, n_skip, n_check);
    fprintf(fid, '  耗时: %.1f s\n', elapsed);
    fprintf(fid, '========================================\n');

    if n_fail > 0
        fprintf(fid, '\n--- FAIL 详情 ---\n');
        for i = 1:N
            if strcmp(results{i}.result, 'FAIL')
                fprintf(fid, '%s:\n', results{i}.name);
                fprintf(fid, '%s\n', results{i}.detail);
            end
        end
    end

    if n_err > 0
        fprintf(fid, '\n--- ERROR 详情 ---\n');
        for i = 1:N
            if strcmp(results{i}.result, 'ERROR')
                fprintf(fid, '  %s: %s\n', results{i}.name, results{i}.detail);
            end
        end
    end

    if n_check > 0
        fprintf(fid, '\n--- CHECK 详情 ---\n');
        for i = 1:N
            if strcmp(results{i}.result, 'CHECK')
                fprintf(fid, '  %s:\n', results{i}.name);
                fprintf(fid, '    %s\n', results{i}.detail);
            end
        end
    end

    fclose(fid);
    fprintf('\n结果日志: %s\n', log_file);
end