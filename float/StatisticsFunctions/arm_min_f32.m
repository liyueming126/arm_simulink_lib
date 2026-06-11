function arm_min_f32_test()
    fprintf('Testing arm_min_f32...\n');

    x = single([1.0, 5.0, 3.0, 9.0, 2.0]);
    [val, idx] = arm_min_f32_mex(x);
    expected_val = min(x);
    expected_idx = find(x == min(x), 1);
    if abs(val - expected_val) < 1e-6 && idx == expected_idx
        fprintf('Test 1 PASSED\n');
    else
        fprintf('Test 1 FAILED\n');
    end

    x2 = single([-1.0, -5.0, -3.0, -9.0, -2.0]);
    [val2, idx2] = arm_min_f32_mex(x2);
    expected_val2 = min(x2);
    expected_idx2 = find(x2 == min(x2), 1);
    if abs(val2 - expected_val2) < 1e-6 && idx2 == expected_idx2
        fprintf('Test 2 PASSED\n');
    else
        fprintf('Test 2 FAILED\n');
    end

    fprintf('Done.\n');
end