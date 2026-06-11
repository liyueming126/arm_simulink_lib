function arm_biquad_cascade_df1_init_f32_test
    disp('Testing arm_biquad_cascade_df1_init_f32 ...');

    numStages = uint8(2);
    coeffs = single([0.5, 0.3, 0.2, -0.1, 0.05, 0.4, 0.3, 0.1, -0.2, 0.1]);
    state = arm_biquad_cascade_df1_init_f32_mex(numStages, coeffs);

    if all(state(:) == 0)
        disp('  Test 1 PASSED (state buffer zeroed)');
    else
        disp('  Test 1 FAILED');
    end

    expectedLen = 4 * numStages;
    if length(state) == expectedLen
        disp('  Test 2 PASSED (state buffer correct length)');
    else
        disp('  Test 2 FAILED');
    end

    disp('arm_biquad_cascade_df1_init_f32 tests completed.');
end