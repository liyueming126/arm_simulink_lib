function arm_biquad_cascade_df1_f32_test
    disp('Testing arm_biquad_cascade_df1_f32 ...');

    % Test 1: Single biquad stage (identity-like: b0=1, others=0)
    numStages = uint8(1);
    coeffs = single([1.0, 0.0, 0.0, 0.0, 0.0]);
    x = single([1.0, 2.0, 3.0]);
    y = arm_biquad_cascade_df1_f32_mex(numStages, coeffs, x);
    err = max(abs(y - x));
    if err < 1e-6
        disp('  Test 1 PASSED (identity biquad)');
    else
        disp(['  Test 1 FAILED (max error = ' num2str(err) ')']);
    end

    % Test 2: Compare with MATLAB filter for simple biquad
    numStages = uint8(1);
    coeffs = single([0.2, 0.3, 0.2, -0.5, 0.1]);
    x = single(randn(1, 10));
    y = arm_biquad_cascade_df1_f32_mex(numStages, coeffs, x);
    b = double([coeffs(1), coeffs(2), coeffs(3)]);
    a = double([1, coeffs(4), coeffs(5)]);
    y_ref = filter(b, a, double(x));
    y_ref = single(y_ref);
    err = max(abs(y - y_ref));
    if err < 1e-5
        disp('  Test 2 PASSED (MATLAB filter comparison)');
    else
        disp(['  Test 2 FAILED (max error = ' num2str(err) ')']);
    end

    % Test 3: 2-stage cascade
    numStages = uint8(2);
    coeffs = single([0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0]);
    x = single([1.0, 2.0, 3.0]);
    y = arm_biquad_cascade_df1_f32_mex(numStages, coeffs, x);
    y_expected = single([0.25, 0.5, 0.75]);
    err = max(abs(y - y_expected));
    if err < 1e-6
        disp('  Test 3 PASSED (2-stage cascade)');
    else
        disp(['  Test 3 FAILED (max error = ' num2str(err) ')']);
    end

    disp('arm_biquad_cascade_df1_f32 tests completed.');
end