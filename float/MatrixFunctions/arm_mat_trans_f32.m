function arm_mat_trans_f32_test()
    fprintf('Testing arm_mat_trans_f32...\n');

    A = single([1 2; 3 4]);
    B = arm_mat_trans_f32_mex(A);
    expected = A';
    if all(abs(B(:) - expected(:)) < 1e-6)
        fprintf('Test 1 PASSED\n');
    else
        fprintf('Test 1 FAILED\n');
    end

    A2 = single([1 2 3; 4 5 6]);
    B2 = arm_mat_trans_f32_mex(A2);
    expected2 = A2';
    if all(abs(B2(:) - expected2(:)) < 1e-6)
        fprintf('Test 2 PASSED\n');
    else
        fprintf('Test 2 FAILED\n');
    end

    fprintf('Done.\n');
end