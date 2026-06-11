function arm_scale_q31()
clc;
fprintf('=== Testing arm_scale_q31 ===\n\n');
errors = 0;

% Test 1: unity scale, shift=31
x = int32([0, 2147483647, -2147483648, 1073741824, -1073741824, 1000, -1000]);
y = arm_scale_q31_mex(x, int32(2147483647), int8(31));
fprintf('Test 1 - near unity, shift=31:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
kShift = 0;
s = (double(x) .* 2147483647.0) ./ (2.0^kShift);
ref = int32(max(min(floor(s), 2147483647.0), -2147483648.0));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: scale=0.5, shift=0
y = arm_scale_q31_mex(x, int32(1073741824), int8(0));
fprintf('Test 2 - scale=0.5, shift=0:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
kShift = 31;
s = (double(x) .* 1073741824.0) ./ (2.0^kShift);
ref = int32(max(min(floor(s), 2147483647.0), -2147483648.0));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: scale=2.0, shift=30 (kShift=1)
y = arm_scale_q31_mex(x, int32(1073741824), int8(30));
fprintf('Test 3 - scale=2.0, shift=30:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
kShift = 1;
s = (double(x) .* 1073741824.0) ./ (2.0^kShift);
ref = int32(max(min(floor(s), 2147483647.0), -2147483648.0));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: random large vector
rng(42);
n = 10000;
x = int32(randi([-2147483648, 2147483647], n, 1));
tic; y = arm_scale_q31_mex(x, int32(1073741824), int8(2)); t = toc;
fprintf('Test 4 - Random %d-element: %.3f ms\n', n, t*1000);
kShift = 31 - 2;
s = (double(x) .* 1073741824.0) ./ (2.0^kShift);
ref = int32(max(min(floor(s), 2147483647.0), -2147483648.0));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    mismatches = find(y ~= ref, 5, 'first');
    fprintf('  FAIL at indices: ['); fprintf('%d ', mismatches); fprintf(']\n');
    for k = mismatches(:)', fprintf('    x(%d)=%d, y=%d, expected=%d\n', k, x(k), y(k), ref(k)); end
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end