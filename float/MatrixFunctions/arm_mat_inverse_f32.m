function arm_mat_inverse_f32_test()
    fprintf('Testing arm_mat_inverse_f32...\n');

    A = single([1 2; 3 4]);
    B = arm_mat_inverse_f32_mex(A);
    expected = inv(A);
    if all(abs(B(:) - expected(:)) < 1e-5)
        fprintf('Test 1 PASSED\n');
    else
        fprintf('Test 1 FAILED\n');
    end

    A2 = single([4 7; 2 6]);
    B2 = arm_mat_inverse_f32_mex(A2);
    expected2 = inv(A2);
    if all(abs(B2(:) - expected2(:)) < 1e-5)
        fprintf('Test 2 PASSED\n');
    else
        fprintf('Test 2 FAILED\n');
    end

    fprintf('Done.\n');
end