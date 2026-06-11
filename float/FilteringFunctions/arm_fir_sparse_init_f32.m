function arm_fir_sparse_init_f32_test
    disp('Testing arm_fir_sparse_init_f32 ...');

    numTaps = uint16(2);
    coeffs = single([0.5, 0.3]);
    tapDelays = int32([0, 2]);
    maxDelay = uint16(3);
    blockSize = uint32(4);

    state = arm_fir_sparse_init_f32_mex(numTaps, coeffs, tapDelays, maxDelay, blockSize);

    if all(state(:) == 0)
        disp('  Test 1 PASSED (state buffer zeroed)');
    else
        disp('  Test 1 FAILED');
    end

    expectedLen = maxDelay + blockSize;
    if length(state) == expectedLen
        disp('  Test 2 PASSED (state buffer correct length)');
    else
        disp('  Test 2 FAILED');
    end

    disp('arm_fir_sparse_init_f32 tests completed.');
end