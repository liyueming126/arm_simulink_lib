function arm_mat_sub_f32_test()
    fprintf('Testing arm_mat_sub_f32...\n');

    A = single([5 6; 7 8]);
    B = single([1 2; 3 4]);
    C = arm_mat_sub_f32_mex(A, B);
    expected = single([4 4; 4 4]);
    if all(abs(C(:) - expected(:)) < 1e-6)
        fprintf('Test 1 PASSED\n');
    else
        fprintf('Test 1 FAILED\n');
    end

    A2 = single([10 20 30; 40 50 60]);
    B2 = single([1 2 3; 4 5 6]);
    C2 = arm_mat_sub_f32_mex(A2, B2);
    expected2 = single([9 18 27; 36 45 54]);
    if all(abs(C2(:) - expected2(:)) < 1e-6)
        fprintf('Test 2 PASSED\n');
    else
        fprintf('Test 2 FAILED\n');
    end

    fprintf('Done.\n');
end