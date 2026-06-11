function arm_cos_q31()

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_cos_q31_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

fprintf('=== arm_cos_q31 tests ===\n');
pass = 0;
fail = 0;

test_angles = [0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 0.9999];

for i = 1:length(test_angles)
    frac = test_angles(i);
    x = int32(fix(frac * 2147483648));
    if x > 2147483647
        x = 2147483647;
    end

    y = arm_cos_q31_mex(x);
    yf = double(y) / 2147483648.0;

    expected = cos(frac * 2 * pi);

    err = abs(yf - expected);
    fprintf('  input=%.4f (0x%08X) => output=%.6f, expected=%.6f, err=%.6f\n', ...
            frac, uint32(x), yf, expected, err);

    if err < 0.005
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
