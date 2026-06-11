function arm_biquad_cascade_df2T_init_f32_test
    disp('Testing arm_biquad_cascade_df2T_init_f32 ...');

    numStages = uint8(3);
    coeffs = single(randn(1, 15));
    state = arm_biquad_cascade_df2T_init_f32_mex(numStages, coeffs);

    if all(state(:) == 0)
        disp('  Test 1 PASSED (state buffer zeroed)');
    else
        disp('  Test 1 FAILED');
    end

    if length(state) == 2 * numStages
        disp('  Test 2 PASSED (state buffer correct length)');
    else
        disp('  Test 2 FAILED');
    end

    disp('arm_biquad_cascade_df2T_init_f32 tests completed.');
end