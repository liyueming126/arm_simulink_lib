function arm_cmplx_mag_q15()
    fprintf('=== arm_cmplx_mag_q15 ===\n');

    tests = {
        int16([0, 0, 0, 0]),
        int16([16384, 0, 0, 16384]),
        int16([32767, 0, 0, 0]),
        int16([0, 32767, 0, 0]),
        int16([23170, 23170, 10000, 20000]),
    };

    for i = 1:length(tests)
        inp = tests{i};
        out = arm_cmplx_mag_q15_mex(inp);
        fprintf('  Test %d:', i);
        for k = 1:length(out)
            re = double(inp(2*k-1));
            im = double(inp(2*k));
            expected = sqrt(re * re + im * im) / 32768.0;
            actual = double(out(k)) / 16384.0;
            fprintf(' out=%d (exp=%.4f)', out(k), expected);
            assert(abs(actual - expected) < 0.01, sprintf('Mag mismatch at sample %d: %g vs %g', k, actual, expected));
        end
        fprintf('\n');
    end

    fprintf('  All tests passed.\n');
end