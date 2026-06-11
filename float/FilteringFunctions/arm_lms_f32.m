function arm_lms_f32_test
    disp('Testing arm_lms_f32 ...');

    % Test 1: Basic LMS - should converge towards reference
    mu = single(0.5);
    coeffs = single(zeros(1, 3));
    x = single([1.0, 0.0, 0.0, 0.0, 0.0]);
    ref = single([0.5, 0.0, 0.0, 0.0, 0.0]);
    [y, e] = arm_lms_f32_mex(mu, coeffs, x, ref);
    % First output should be 0 (coeffs initially zero)
    if abs(y(1)) < 1e-6
        disp('  Test 1 PASSED (initial output zero)');
    else
        disp(['  Test 1 FAILED (y(1) = ' num2str(y(1)) ')']);
    end

    % Test 2: Verify error = ref - out
    if abs(e(1) - (ref(1) - y(1))) < 1e-6
        disp('  Test 2 PASSED (error = ref - out)');
    else
        disp('  Test 2 FAILED');
    end

    disp('arm_lms_f32 tests completed.');
end