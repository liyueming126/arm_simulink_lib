function arm_cmplx_mult_cmplx_q15()
    fprintf('=== arm_cmplx_mult_cmplx_q15 ===\n');

    tests = {
        {int16([0,0,0,0]), int16([0,0,0,0])},                                       % zeros
        {int16([16384,0,0,16384]), int16([16384,0,0,16384])},                        % half-scale
        {int16([32767,0,0,0]), int16([32767,0,0,32767])},                            % max
        {int16([23170,23170,10000,-20000]), int16([23170,-23170,-5000,30000])},       % general
    };

    for i = 1:length(tests)
        a = tests{i}{1};
        b = tests{i}{2};
        out = arm_cmplx_mult_cmplx_q15_mex(a, b);
        n = length(a) / 2;
        fprintf('  Test %d:', i);
        for k = 1:n
            ar = double(a(2*k-1)); ai = double(a(2*k));
            br = double(b(2*k-1)); bi = double(b(2*k));
            exp_re = (ar * br - ai * bi) / (32768.0 * 32768.0);
            exp_im = (ar * bi + ai * br) / (32768.0 * 32768.0);
            actual_re = double(out(2*k-1)) / 32768.0;
            actual_im = double(out(2*k)) / 32768.0;
            tol = 2.0 / 32768.0;
            assert(abs(actual_re - exp_re) < tol, sprintf('Real mismatch at sample %d', k));
            assert(abs(actual_im - exp_im) < tol, sprintf('Imag mismatch at sample %d', k));
        end
        fprintf('passed\n');
    end

    fprintf('  All tests passed.\n');
end