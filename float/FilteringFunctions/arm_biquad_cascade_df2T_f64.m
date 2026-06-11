function arm_biquad_cascade_df2T_f64_test
    disp('Testing arm_biquad_cascade_df2T_f64 ...');

    % Test 1: Identity
    numStages = uint8(1);
    coeffs = double([1.0, 0.0, 0.0, 0.0, 0.0]);
    x = double([1.0, 2.0, 3.0]);
    y = arm_biquad_cascade_df2T_f64_mex(numStages, coeffs, x);
    err = max(abs(y - x));
    if err < 1e-15
        disp('  Test 1 PASSED (identity)');
    else
        disp(['  Test 1 FAILED (max error = ' num2str(err) ')']);
    end

    % Test 2: Compare with MATLAB filter
    numStages = uint8(1);
    coeffs = double([0.3, 0.2, 0.1, -0.4, 0.2]);
    x = double(randn(1, 10));
    y = arm_biquad_cascade_df2T_f64_mex(numStages, coeffs, x);
    b = [coeffs(1), coeffs(2), coeffs(3)];
    a = [1, coeffs(4), coeffs(5)];
    y_ref = filter(b, a, x);
    err = max(abs(y - y_ref));
    if err < 1e-14
        disp('  Test 2 PASSED (MATLAB filter comparison)');
    else
        disp(['  Test 2 FAILED (max error = ' num2str(err) ')']);
    end

    disp('arm_biquad_cascade_df2T_f64 tests completed.');
end