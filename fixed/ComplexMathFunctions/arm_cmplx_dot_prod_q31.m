function arm_cmplx_dot_prod_q31()
    fprintf('=== arm_cmplx_dot_prod_q31 ===\n');

    tests = {
        {int32([0,0,0,0]), int32([0,0,0,0])},
        {int32([1073741824,0,0,1073741824]), int32([1073741824,0,0,1073741824])},
        {int32([655360000,1310720000,-327680000,1966080000]), int32([983040000,-1310720000,524288000,786432000])},
        {int32([2147483647,2147483647,2147483647,2147483647]), int32([2147483647,2147483647,2147483647,2147483647])},
    };

    for i = 1:length(tests)
        a = tests{i}{1};
        b = tests{i}{2};
        [realOut, imagOut] = arm_cmplx_dot_prod_q31_mex(a, b);

        n = length(a) / 2;
        exp_re = int64(0);
        exp_im = int64(0);
        for k = 1:n
            ar = int64(a(2*k-1)); ai = int64(a(2*k));
            br = int64(b(2*k-1)); bi = int64(b(2*k));
            exp_re = exp_re + bitshift(ar * br - ai * bi, -14);
            exp_im = exp_im + bitshift(ar * bi + ai * br, -14);
        end

        fprintf('  Test %d: real=%ld (exp=%ld), imag=%ld (exp=%ld)\n', i, realOut, exp_re, imagOut, exp_im);
        assert(double(realOut) == double(exp_re), 'Real dot product mismatch');
        assert(double(imagOut) == double(exp_im), 'Imag dot product mismatch');
    end

    fprintf('  All tests passed.\n');
end