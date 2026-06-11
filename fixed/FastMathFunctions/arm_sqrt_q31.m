function arm_sqrt_q31()

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_sqrt_q31_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

fprintf('=== arm_sqrt_q31 tests ===\n');
pass = 0;
fail = 0;

test_vals = [0.0, 0.01, 0.04, 0.09, 0.16, 0.25, 0.36, 0.49, 0.64, 0.81, 0.9999];
N = length(test_vals);
in = int32(zeros(1, N));

for i = 1:N
    in(i) = int32(fix(test_vals(i) * 2147483647));
end

out = arm_sqrt_q31_mex(in);

for i = 1:N
    frac = test_vals(i);
    yf = double(out(i)) / 2147483648.0;
    expected = sqrt(frac);
    err = abs(yf - expected);
    fprintf('  input=%.6f (0x%08X) => sqrt=%.6f, expected=%.6f, err=%.6f\n', ...
            frac, uint32(in(i)), yf, expected, err);

    if frac == 0.0
        if yf < 0.001
            pass = pass + 1;
            fprintf('    PASS\n');
        else
            fail = fail + 1;
            fprintf('    FAIL\n');
        end
    elseif err < 0.005
        pass = pass + 1;
        fprintf('    PASS\n');
    else
        fail = fail + 1;
        fprintf('    FAIL\n');
    end
end

fprintf('\nResults: %d passed, %d failed\n', pass, fail);


if fail == 0
    fprintf('=== All tests PASSED ===\n');
end

end
