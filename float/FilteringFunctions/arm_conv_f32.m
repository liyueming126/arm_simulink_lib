function arm_conv_f32_test
    disp('Testing arm_conv_f32 ...');

    % Test 1: Simple convolution
    a = single([1.0, 2.0, 3.0]);
    b = single([0.5, 0.5]);
    y = arm_conv_f32_mex(a, b);
    y_expected = single([0.5, 1.5, 2.5, 1.5]);
    err = max(abs(y - y_expected));
    if err < 1e-6
        disp('  Test 1 PASSED (simple convolution)');
    else
        disp(['  Test 1 FAILED (max error = ' num2str(err) ')']);
    end

    % Test 2: Compare with MATLAB conv()
    a = single(randn(1, 8));
    b = single(randn(1, 5));
    y = arm_conv_f32_mex(a, b);
    y_ref = single(conv(double(a), double(b)));
    err = max(abs(y - y_ref));
    if err < 1e-5
        disp('  Test 2 PASSED (MATLAB conv comparison)');
    else
        disp(['  Test 2 FAILED (max error = ' num2str(err) ')']);
    end

    % Test 3: Impulse response
    a = single([1.0, 0.0, 0.0]);
    b = single([2.0, 3.0]);
    y = arm_conv_f32_mex(a, b);
    y_expected = single([2.0, 3.0, 0.0]);
    err = max(abs(y - y_expected));
    if err < 1e-6
        disp('  Test 3 PASSED (impulse response)');
    else
        disp(['  Test 3 FAILED (max error = ' num2str(err) ')']);
    end

    disp('arm_conv_f32 tests completed.');
end