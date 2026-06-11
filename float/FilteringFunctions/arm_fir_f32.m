function arm_fir_f32_test
    disp('Testing arm_fir_f32 ...');

    % Test 1: Simple low-pass filter coefficients
    coeffs = single([0.25, 0.5, 0.25]);
    x = single([1.0, 0.0, 0.0, 0.0, 0.0]);
    y = arm_fir_f32_mex(coeffs, x);
    y_expected = single([0.25, 0.5, 0.25, 0.0, 0.0]);
    err = max(abs(y - y_expected));
    if err < 1e-6
        disp('  Test 1 PASSED (impulse response)');
    else
        disp(['  Test 1 FAILED (max error = ' num2str(err) ')']);
    end

    % Test 2: DC signal
    coeffs = single([0.2, 0.6, 0.2]);
    x = single(ones(1, 6));
    y = arm_fir_f32_mex(coeffs, x);
    gain = sum(double(coeffs));
    y_expected = single(ones(1, 6) * gain);
    err = max(abs(y - y_expected));
    if err < 1e-6
        disp('  Test 2 PASSED (DC signal)');
    else
        disp(['  Test 2 FAILED (max error = ' num2str(err) ')']);
    end

    % Test 3: Compare with MATLAB filter()
    coeffs = single([0.1, 0.3, 0.4, 0.2]);
    x = single(randn(1, 10));
    y = arm_fir_f32_mex(coeffs, x);
    y_ref = filter(double(coeffs), 1, double(x));
    y_ref = single(y_ref);
    err = max(abs(y - y_ref));
    if err < 1e-5
        disp('  Test 3 PASSED (MATLAB filter comparison)');
    else
        disp(['  Test 3 FAILED (max error = ' num2str(err) ')']);
    end

    disp('arm_fir_f32 tests completed.');
end