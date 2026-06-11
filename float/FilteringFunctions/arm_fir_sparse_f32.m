function arm_fir_sparse_f32_test
    disp('Testing arm_fir_sparse_f32 ...');

    % Test 1: Sparse FIR with single non-zero coefficient
    coeffs = single([1.0]);
    tapDelays = int32([0]);
    maxDelay = uint16(2);
    x = single([1.0, 2.0, 3.0]);
    y = arm_fir_sparse_f32_mex(coeffs, tapDelays, maxDelay, x);
    y_expected = single([1.0, 2.0, 3.0]);
    err = max(abs(y - y_expected));
    if err < 1e-6
        disp('  Test 1 PASSED (single tap, zero delay)');
    else
        disp(['  Test 1 FAILED (max error = ' num2str(err) ')']);
    end

    % Test 2: Two taps with delays
    coeffs = single([0.5, 0.5]);
    tapDelays = int32([0, 1]);
    maxDelay = uint16(3);
    x = single([1.0, 0.0, 0.0, 0.0]);
    y = arm_fir_sparse_f32_mex(coeffs, tapDelays, maxDelay, x);
    y_expected = single([0.5, 0.5, 0.0, 0.0]);
    err = max(abs(y - y_expected));
    if err < 1e-6
        disp('  Test 2 PASSED (two taps with delays)');
    else
        disp(['  Test 2 FAILED (max error = ' num2str(err) ')']);
    end

    disp('arm_fir_sparse_f32 tests completed.');
end