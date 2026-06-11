function arm_cmplx_mult_real_q15()
    fprintf('=== arm_cmplx_mult_real_q15 ===\n');

    tests = {
        {int16([0,0,0,0]), int16([0,0])},
        {int16([16384,8192,32767,-32768]), int16([16384,16384])},
        {int16([32767,32767,-32768,-32768]), int16([32767,32767])},
        {int16([10000,20000,-5000,30000]), int16([15000,-20000])},
    };

    for i = 1:length(tests)
        cmplx = tests{i}{1};
        real = tests{i}{2};
        out = arm_cmplx_mult_real_q15_mex(cmplx, real);
        n = length(real);
        fprintf('  Test %d:', i);
        for k = 1:n
            re_c = double(cmplx(2*k-1));
            im_c = double(cmplx(2*k));
            r = double(real(k));
            exp_re = re_c * r / (32768.0 * 32768.0);
            exp_im = im_c * r / (32768.0 * 32768.0);
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