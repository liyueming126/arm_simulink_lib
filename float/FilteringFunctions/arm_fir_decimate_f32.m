function arm_fir_decimate_f32_test
    disp('Testing arm_fir_decimate_f32 ...');

    % Test 1: Basic decimation
    M = uint8(2);
    coeffs = single([0.5, 0.5]);
    x = single([1.0, 2.0, 3.0, 4.0, 5.0, 6.0]);
    y = arm_fir_decimate_f32_mex(M, coeffs, x);
    y_expected = single([1.5, 3.5, 5.5]);
    err = max(abs(y - y_expected));
    if err < 1e-6
        disp('  Test 1 PASSED (basic decimation)');
    else
        disp(['  Test 1 FAILED (max error = ' num2str(err) ')']);
    end

    % Test 2: Decimate by 3 with single coefficient (identity)
    M = uint8(3);
    coeffs = single([1.0]);
    x = single([10, 20, 30, 40, 50, 60]);
    y = arm_fir_decimate_f32_mex(M, coeffs, x);
    y_expected = single([10, 40]);
    err = max(abs(y - y_expected));
    if err < 1e-6
        disp('  Test 2 PASSED (decimate by 3)');
    else
        disp(['  Test 2 FAILED (max error = ' num2str(err) ')']);
    end

    disp('arm_fir_decimate_f32 tests completed.');
end