function arm_cmplx_mag_squared_q15()
    fprintf('=== arm_cmplx_mag_squared_q15 ===\n');

    tests = {
        int16([0, 0, 0, 0]),                     % zero -> zero
        int16([16384, 0, 0, 16384]),              % half-scale real, half-scale imag
        int16([32767, 0, 0, 0]),                  % max real, zero imag
        int16([0, 32767, -32768, -32768]),        % max imag, min both
        int16([23170, 23170]),                    % equal ~0.707 * 32768
    };

    for i = 1:length(tests)
        inp = tests{i};
        out = arm_cmplx_mag_squared_q15_mex(inp);
        fprintf('  Test %d: in=[', i);
        fprintf('%d,', inp);
        fprintf('\b] out=[');
        fprintf('%d,', out);
        fprintf('\b]\n');

        for k = 1:length(out)
            re = double(inp(2*k-1));
            im = double(inp(2*k));
            expected = (re*re + im*im) / (32768.0 * 32768.0);
            actual = double(out(k)) / 8192.0;
            assert(abs(actual - expected) < 0.01, 'Magnitude squared mismatch');
        end
    end

    fprintf('  All tests passed.\n');
end