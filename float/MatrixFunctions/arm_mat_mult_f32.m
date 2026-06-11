function arm_mat_mult_f32_test()
    fprintf('Testing arm_mat_mult_f32...\n');

    A = single([1 2; 3 4]);
    B = single([5 6; 7 8]);
    C = arm_mat_mult_f32_mex(A, B);
    expected = single([19 22; 43 50]);
    if all(abs(C(:) - expected(:)) < 1e-6)
        fprintf('Test 1 PASSED\n');
    else
        fprintf('Test 1 FAILED\n');
    end

    A2 = single([1 0 2; -1 3 1]);
    B2 = single([3 1; 2 1; 1 0]);
    C2 = arm_mat_mult_f32_mex(A2, B2);
    expected2 = single([5 1; 4 2]);
    if all(abs(C2(:) - expected2(:)) < 1e-6)
        fprintf('Test 2 PASSED\n');
    else
        fprintf('Test 2 FAILED\n');
    end

    fprintf('Done.\n');
end