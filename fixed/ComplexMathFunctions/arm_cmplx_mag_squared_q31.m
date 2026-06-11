function arm_cmplx_mag_squared_q31()
    fprintf('=== arm_cmplx_mag_squared_q31 ===\n');

    tests = {
        int32([0, 0, 0, 0]),
        int32([1073741824, 0, 0, 1073741824]),
        int32([2147483647, 0, 0, 0]),
        int32([0, 2147483647, -2147483648, -2147483648]),
        int32([1518500250, 1518500250]),
    };

    for i = 1:length(tests)
        inp = tests{i};
        out = arm_cmplx_mag_squared_q31_mex(inp);
        fprintf('  Test %d: ', i);
        for k = 1:length(out)
            re = double(inp(2*k-1));
            im = double(inp(2*k));
            % CMSIS output is in 3.29 format: value = out * 2^-29
            % Actual mag^2 = (re^2 + im^2) / (2^31)^2
            expected = (re*re + im*im) / (4611686018427387904.0);  % 2^62
            actual = double(out(k)) / 536870912.0;  % 2^29 for 3.29 format output
            assert(abs(actual - expected) < 0.001, 'Magnitude squared mismatch');
        end
        fprintf('passed\n');
    end

    fprintf('  All tests passed.\n');
end