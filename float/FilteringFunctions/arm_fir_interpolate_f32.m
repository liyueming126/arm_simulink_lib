function arm_fir_interpolate_f32_test
    disp('Testing arm_fir_interpolate_f32 ...');

    % Test 1: Basic interpolation
    L = uint8(2);
    coeffs = single([0.5, 0.5]); % phaseLength=1
    x = single([1.0, 2.0, 3.0]);
    y = arm_fir_interpolate_f32_mex(L, coeffs, x);
    y_expected = single([0.5, 0.5, 1.0, 1.0, 1.5, 1.5]);
    err = max(abs(y - y_expected));
    if err < 1e-6
        disp('  Test 1 PASSED (basic interpolation)');
    else
        disp(['  Test 1 FAILED (max error = ' num2str(err) ')']);
    end

    % Test 2: Interpolate by 3, identity filter
    L = uint8(3);
    coeffs = single([1.0, 1.0, 1.0]); % phaseLength=1
    x = single([1, 2]);
    y = arm_fir_interpolate_f32_mex(L, coeffs, x);
    y_expected = single([1, 1, 1, 2, 2, 2]);
    err = max(abs(y - y_expected));
    if err < 1e-6
        disp('  Test 2 PASSED (interpolate by 3)');
    else
        disp(['  Test 2 FAILED (max error = ' num2str(err) ')']);
    end

    disp('arm_fir_interpolate_f32 tests completed.');
end