function arm_lms_norm_init_f32_test
    disp('Testing arm_lms_norm_init_f32 ...');

    numTaps = uint16(3);
    coeffs = single([0.2, 0.4, 0.3]);
    mu = single(0.05);
    blockSize = uint32(4);

    state = arm_lms_norm_init_f32_mex(numTaps, coeffs, mu, blockSize);

    if all(state(:) == 0)
        disp('  Test 1 PASSED (state buffer zeroed)');
    else
        disp('  Test 1 FAILED');
    end

    expectedLen = numTaps + blockSize - 1;
    if length(state) == expectedLen
        disp('  Test 2 PASSED (state buffer correct length)');
    else
        disp('  Test 2 FAILED');
    end

    disp('arm_lms_norm_init_f32 tests completed.');
end