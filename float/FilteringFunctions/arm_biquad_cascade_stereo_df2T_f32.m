function arm_biquad_cascade_stereo_df2T_f32_test
    disp('Testing arm_biquad_cascade_stereo_df2T_f32 ...');

    % Test 1: Identity, interleaved stereo
    numStages = uint8(1);
    coeffs = single([1.0, 0.0, 0.0, 0.0, 0.0]);
    x = single([1.0, 10.0, 2.0, 20.0, 3.0, 30.0]); % L,R,L,R,L,R
    y = arm_biquad_cascade_stereo_df2T_f32_mex(numStages, coeffs, x);
    err = max(abs(y - x));
    if err < 1e-6
        disp('  Test 1 PASSED (identity stereo)');
    else
        disp(['  Test 1 FAILED (max error = ' num2str(err) ')']);
    end

    % Test 2: Known coefficients, interleaved
    numStages = uint8(1);
    coeffs = single([0.5, 0.0, 0.0, 0.0, 0.0]);
    x = single([2.0, 4.0, 6.0, 8.0]);
    y = arm_biquad_cascade_stereo_df2T_f32_mex(numStages, coeffs, x);
    y_expected = single([1.0, 2.0, 3.0, 4.0]);
    err = max(abs(y - y_expected));
    if err < 1e-6
        disp('  Test 2 PASSED (gain 0.5 stereo)');
    else
        disp(['  Test 2 FAILED (max error = ' num2str(err) ')']);
    end

    disp('arm_biquad_cascade_stereo_df2T_f32 tests completed.');
end