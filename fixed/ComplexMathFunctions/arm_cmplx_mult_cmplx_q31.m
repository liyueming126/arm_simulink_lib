function arm_cmplx_mult_cmplx_q31()
    fprintf('=== arm_cmplx_mult_cmplx_q31 ===\n');

    tests = {
        {int32([0,0,0,0]), int32([0,0,0,0])},
        {int32([1073741824,0,0,1073741824]), int32([1073741824,0,0,1073741824])},
        {int32([2147483647,0,0,0]), int32([2147483647,0,0,2147483647])},
        {int32([1518500250,1518500250,655360000,-1310720000]), int32([1518500250,-1518500250,-327680000,1966080000])},
    };

    for i = 1:length(tests)
        a = tests{i}{1};
        b = tests{i}{2};
        out = arm_cmplx_mult_cmplx_q31_mex(a, b);
        n = length(a) / 2;
        fprintf('  Test %d:', i);
        for k = 1:n
            ar = double(a(2*k-1)); ai = double(a(2*k));
            br = double(b(2*k-1)); bi = double(b(2*k));
            % CMSIS output is in 3.29 format: value = out * 2^-29
            % Complex product in Q31: (a+bi)(c+di) = (ac-bd) + (ad+bc)i, each divided by 2^31
            % Then stored in 3.29 format means >> 32 then >> 1 = total >> 33 from the 2.62 product
            exp_re = (ar * br - ai * bi) / (4611686018427387904.0);  % 2^62
            exp_im = (ar * bi + ai * br) / (4611686018427387904.0);  % 2^62
            actual_re = double(out(2*k-1)) / 536870912.0;  % 2^29 for 3.29 format
            actual_im = double(out(2*k)) / 536870912.0;
            tol = 2.0 / 536870912.0;
            assert(abs(actual_re - exp_re) < tol, sprintf('Real mismatch at sample %d', k));
            assert(abs(actual_im - exp_im) < tol, sprintf('Imag mismatch at sample %d', k));
        end
        fprintf('passed\n');
    end

    fprintf('  All tests passed.\n');
end