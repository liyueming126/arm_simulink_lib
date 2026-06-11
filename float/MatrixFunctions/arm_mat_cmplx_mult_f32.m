function arm_mat_cmplx_mult_f32_test()
    fprintf('Testing arm_mat_cmplx_mult_f32...\n');

    A = single([1+2i 3+4i; 5+6i 7+8i]);
    B = single([9+10i 11+12i; 13+14i 15+16i]);
    C = arm_mat_cmplx_mult_f32_mex(A, B);
    expected = A * B;
    if all(abs(C(:) - expected(:)) < 1e-4)
        fprintf('Test 1 PASSED\n');
    else
        fprintf('Test 1 FAILED\n');
    end

    A2 = single([1+1i 2+2i; 3+3i 4+4i]);
    B2 = single([1+0i 0+0i; 0+0i 1+0i]);
    C2 = arm_mat_cmplx_mult_f32_mex(A2, B2);
    expected2 = A2 * B2;
    if all(abs(C2(:) - expected2(:)) < 1e-4)
        fprintf('Test 2 PASSED\n');
    else
        fprintf('Test 2 FAILED\n');
    end

    fprintf('Done.\n');
end