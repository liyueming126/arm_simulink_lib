function arm_cmplx_mult_real_q31()
    fprintf('=== arm_cmplx_mult_real_q31 ===\n');

    tests = {
        {int32([0,0,0,0]), int32([0,0])},
        {int32([1073741824,536870912,2147483647,-2147483648]), int32([1073741824,1073741824])},
        {int32([2147483647,2147483647,-2147483648,-2147483648]), int32([2147483647,2147483647])},
        {int32([655360000,1310720000,-327680000,1966080000]), int32([983040000,-1310720000])},
    };

    for i = 1:length(tests)
        cmplx = tests{i}{1};
        real = tests{i}{2};
        out = arm_cmplx_mult_real_q31_mex(cmplx, real);
        n = length(real);
        fprintf('  Test %d:', i);
        for k = 1:n
            re_c = double(cmplx(2*k-1));
            im_c = double(cmplx(2*k));
            r = double(real(k));
            exp_re = re_c * r / (2147483648.0 * 2147483648.0);
            exp_im = im_c * r / (2147483648.0 * 2147483648.0);
            actual_re = double(out(2*k-1)) / 2147483648.0;
            actual_im = double(out(2*k)) / 2147483648.0;
            tol = 2.0 / 2147483648.0;
            assert(abs(actual_re - exp_re) < tol, sprintf('Real mismatch at sample %d: %.12g vs %.12g', k, actual_re, exp_re));
            assert(abs(actual_im - exp_im) < tol, sprintf('Imag mismatch at sample %d', k));
        end
        fprintf('passed\n');
    end

    fprintf('  All tests passed.\n');
end