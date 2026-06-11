function arm_rms_f32_test()
    fprintf('Testing arm_rms_f32...\n');

    x = single([1.0, 2.0, 3.0, 4.0]);
    val = arm_rms_f32_mex(x);
    expected = sqrt(mean(x.^2));
    if abs(val - expected) < 1e-6
        fprintf('Test 1 PASSED\n');
    else
        fprintf('Test 1 FAILED\n');
    end

    x2 = single([0.0, 5.0, 10.0]);
    val2 = arm_rms_f32_mex(x2);
    expected2 = sqrt(mean(x2.^2));
    if abs(val2 - expected2) < 1e-6
        fprintf('Test 2 PASSED\n');
    else
        fprintf('Test 2 FAILED\n');
    end

    fprintf('Done.\n');
end