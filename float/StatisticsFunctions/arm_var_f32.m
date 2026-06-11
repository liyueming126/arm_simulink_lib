function arm_var_f32_test()
    fprintf('Testing arm_var_f32...\n');

    x = single([1.0, 2.0, 3.0, 4.0, 5.0]);
    val = arm_var_f32_mex(x);
    expected = var(x);
    if abs(val - expected) < 1e-5
        fprintf('Test 1 PASSED\n');
    else
        fprintf('Test 1 FAILED\n');
    end

    x2 = single([10.0, 20.0, 30.0, 40.0]);
    val2 = arm_var_f32_mex(x2);
    expected2 = var(x2);
    if abs(val2 - expected2) < 1e-5
        fprintf('Test 2 PASSED\n');
    else
        fprintf('Test 2 FAILED\n');
    end

    fprintf('Done.\n');
end