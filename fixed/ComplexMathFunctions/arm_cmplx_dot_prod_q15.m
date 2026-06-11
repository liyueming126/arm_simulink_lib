function arm_cmplx_dot_prod_q15()
    fprintf('=== arm_cmplx_dot_prod_q15 ===\n');

    tests = {
        {int16([0,0,0,0]), int16([0,0,0,0])},
        {int16([16384,0,0,16384]), int16([16384,0,0,16384])},
        {int16([10000,20000,-5000,30000]), int16([15000,-20000,8000,12000])},
        {int16([32767,32767,32767,32767]), int16([32767,32767,32767,32767])},
        {int16([32767,-32768,10000,20000]), int16([-32768,32767,20000,-10000])},
    };

    for i = 1:length(tests)
        a = tests{i}{1};
        b = tests{i}{2};
        [realOut, imagOut] = arm_cmplx_dot_prod_q15_mex(a, b);

        n = length(a) / 2;
        exp_re = int64(0);
        exp_im = int64(0);
        for k = 1:n
            ar = int32(a(2*k-1)); ai = int32(a(2*k));
            br = int32(b(2*k-1)); bi = int32(b(2*k));
            exp_re = exp_re + int64(ar) * int64(br) - int64(ai) * int64(bi);
            exp_im = exp_im + int64(ar) * int64(bi) + int64(ai) * int64(br);
        end
        exp_re_out = int32(bitshift(exp_re, -6));
        exp_im_out = int32(bitshift(exp_im, -6));

        fprintf('  Test %d: real=%d (exp=%d), imag=%d (exp=%d)\n', i, realOut, exp_re_out, imagOut, exp_im_out);
        assert(double(realOut) == double(exp_re_out), 'Real dot product mismatch');
        assert(double(imagOut) == double(exp_im_out), 'Imag dot product mismatch');
    end

    fprintf('  All tests passed.\n');
end