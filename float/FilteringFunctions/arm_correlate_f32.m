function arm_correlate_f32_test
    disp('Testing arm_correlate_f32 ...');

    % Test 1: Autocorrelation of impulse
    a = single([1.0, 0.0, 0.0]);
    b = single([1.0, 0.0, 0.0]);
    y = arm_correlate_f32_mex(a, b);
    y_expected = single([0.0, 0.0, 1.0, 0.0, 0.0]);
    err = max(abs(y - y_expected));
    if err < 1e-6
        disp('  Test 1 PASSED (autocorrelation of impulse)');
    else
        disp(['  Test 1 FAILED (max error = ' num2str(err) ')']);
    end

    % Test 2: Compare with MATLAB xcorr
    a = single(randn(1, 6));
    b = single(randn(1, 4));
    y = arm_correlate_f32_mex(a, b);
    y_ref = single(xcorr(double(a), double(b)));
    % xcorr output length is 2*max(len)-1 = 2*6-1 = 11
    if length(y_ref) == 11
        err = max(abs(y - y_ref));
        if err < 1e-5
            disp('  Test 2 PASSED (MATLAB xcorr comparison)');
        else
            disp(['  Test 2 FAILED (max error = ' num2str(err) ')']);
        end
    else
        disp(['  Test 2 SKIPPED (length mismatch: ' num2str(length(y)) ' vs ' num2str(length(y_ref)) ')']);
    end

    % Test 3: Correlation of identical signals should peak at center
    a = single([1.0, 2.0, 3.0, 2.0, 1.0]);
    b = a;
    y = arm_correlate_f32_mex(a, b);
    [~, mid] = max(y);
    if mid == 5
        disp('  Test 3 PASSED (peak at center for autocorrelation)');
    else
        disp(['  Test 3 FAILED (peak at ' num2str(mid) ', expected 5)']);
    end

    disp('arm_correlate_f32 tests completed.');
end