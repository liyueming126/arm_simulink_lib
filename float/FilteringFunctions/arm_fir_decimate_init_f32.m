function arm_fir_decimate_init_f32_test
    disp('Testing arm_fir_decimate_init_f32 ...');

    numTaps = uint16(3);
    M = uint8(2);
    blockSize = uint32(6);
    coeffs = single([0.25, 0.5, 0.25]);

    [status, state] = arm_fir_decimate_init_f32_mex(numTaps, M, coeffs, blockSize);

    if status == 0
        disp('  Test 1 PASSED (init success)');
    else
        disp('  Test 1 FAILED');
    end

    if all(state(:) == 0)
        disp('  Test 2 PASSED (state buffer zeroed)');
    else
        disp('  Test 2 FAILED');
    end

    % Test with invalid block size
    blockSize2 = uint32(5);
    [status2, ~] = arm_fir_decimate_init_f32_mex(numTaps, M, coeffs, blockSize2);
    if status2 ~= 0
        disp('  Test 3 PASSED (detected invalid blockSize)');
    else
        disp('  Test 3 FAILED');
    end

    disp('arm_fir_decimate_init_f32 tests completed.');
end