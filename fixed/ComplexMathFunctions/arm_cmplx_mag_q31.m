function arm_cmplx_mag_q31()
    fprintf('=== arm_cmplx_mag_q31 ===\n');

    tests = {
        int32([0, 0, 0, 0]),
        int32([1073741824, 0, 0, 1073741824]),
        int32([2147483647, 0, 0, 0]),
        int32([0, 2147483647, 0, 0]),
        int32([1518500250, 1518500250, 655360000, 1310720000]),
    };

    for i = 1:length(tests)
        inp = tests{i};
        out = arm_cmplx_mag_q31_mex(inp);
        fprintf('  Test %d:', i);
        for k = 1:length(out)
            re = double(inp(2*k-1));
            im = double(inp(2*k));
            % CMSIS output is in 2.30 format: value = out * 2^-30
            % Actual magnitude = sqrt(re^2 + im^2) / 2^31
            expected = sqrt(re * re + im * im) / 2147483648.0;
            actual = double(out(k)) / 1073741824.0;  % 2^30 for 2.30 format output
            fprintf(' out=%d (exp=%.6f)', out(k), expected);
            assert(abs(actual - expected) < 0.001, sprintf('Mag mismatch at sample %d', k));
        end
        fprintf('\n');
    end

    fprintf('  All tests passed.\n');
end