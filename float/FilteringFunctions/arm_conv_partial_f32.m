function arm_conv_partial_f32_test
    disp('Testing arm_conv_partial_f32 ...');

    % Test 1: Partial convolution, first 3 points
    a = single([1.0, 2.0, 3.0]);
    b = single([0.5, 0.5]);
    [y, status] = arm_conv_partial_f32_mex(a, b, uint32(0), uint32(3));
    y_expected = single([0.5, 1.5, 2.5]);
    err = max(abs(y(1:3) - y_expected));
    if err < 1e-6 && status == 0
        disp('  Test 1 PASSED (partial conv first 3 points)');
    else
        disp(['  Test 1 FAILED (max error = ' num2str(err) ', status = ' num2str(status) ')']);
    end

    % Test 2: Full convolution (partial with all points)
    a = single(randn(1, 6));
    b = single(randn(1, 4));
    [y_full, status_full] = arm_conv_partial_f32_mex(a, b, uint32(0), uint32(9));
    y_ref = single(conv(double(a), double(b)));
    err = max(abs(y_full(1:9) - y_ref));
    if err < 1e-5 && status_full == 0
        disp('  Test 2 PASSED (full conv via partial)');
    else
        disp(['  Test 2 FAILED (max error = ' num2str(err) ')']);
    end

    % Test 3: Invalid range
    [y3, status3] = arm_conv_partial_f32_mex(a, b, uint32(0), uint32(20));
    if status3 ~= 0
        disp('  Test 3 PASSED (detected invalid range)');
    else
        disp('  Test 3 FAILED');
    end

    disp('arm_conv_partial_f32 tests completed.');
end