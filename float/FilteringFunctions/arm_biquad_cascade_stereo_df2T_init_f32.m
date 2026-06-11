function arm_biquad_cascade_stereo_df2T_init_f32_test
    disp('Testing arm_biquad_cascade_stereo_df2T_init_f32 ...');

    numStages = uint8(2);
    coeffs = single(randn(1, 10));
    state = arm_biquad_cascade_stereo_df2T_init_f32_mex(numStages, coeffs);

    if all(state(:) == 0)
        disp('  Test 1 PASSED (state buffer zeroed)');
    else
        disp('  Test 1 FAILED');
    end

    if length(state) == 4 * numStages
        disp('  Test 2 PASSED (state buffer correct length)');
    else
        disp('  Test 2 FAILED');
    end

    disp('arm_biquad_cascade_stereo_df2T_init_f32 tests completed.');
end