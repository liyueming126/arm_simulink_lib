function arm_fir_interpolate_init_f32_test
    disp('Testing arm_fir_interpolate_init_f32 ...');

    L = uint8(2);
    numTaps = uint16(4);
    blockSize = uint32(5);
    coeffs = single([0.25, 0.5, 0.25, 0.0]);

    [status, state] = arm_fir_interpolate_init_f32_mex(L, numTaps, coeffs, blockSize);

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

    % Test with numTaps not multiple of L
    L2 = uint8(3);
    [status2, ~] = arm_fir_interpolate_init_f32_mex(L2, numTaps, coeffs, blockSize);
    if status2 ~= 0
        disp('  Test 3 PASSED (detected invalid numTaps/L)');
    else
        disp('  Test 3 FAILED');
    end

    disp('arm_fir_interpolate_init_f32 tests completed.');
end