function arm_fir_init_f32_test
    disp('Testing arm_fir_init_f32 ...');

    numTaps = uint16(3);
    blockSize = uint32(5);
    coeffs = single([0.25, 0.5, 0.25]);

    [inst, state] = arm_fir_init_f32_mex(numTaps, coeffs, blockSize);

    if all(state(:) == 0)
        disp('  Test 1 PASSED (state buffer initialized to zero)');
    else
        disp('  Test 1 FAILED');
    end

    expectedLen = numTaps + blockSize - 1;
    if length(state) == expectedLen
        disp('  Test 2 PASSED (state buffer correct length)');
    else
        disp('  Test 2 FAILED');
    end

    disp('arm_fir_init_f32 tests completed.');
end