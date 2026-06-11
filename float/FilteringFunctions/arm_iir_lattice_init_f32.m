function arm_iir_lattice_init_f32_test
    disp('Testing arm_iir_lattice_init_f32 ...');

    numStages = uint16(3);
    k = single([0.5, 0.3, 0.2]);
    v = single([0.8, 0.5, 0.3, 0.1]);
    blockSize = uint32(5);

    state = arm_iir_lattice_init_f32_mex(numStages, k, v, blockSize);

    if all(state(:) == 0)
        disp('  Test 1 PASSED (state buffer zeroed)');
    else
        disp('  Test 1 FAILED');
    end

    expectedLen = numStages + blockSize;
    if length(state) == expectedLen
        disp('  Test 2 PASSED (state buffer correct length)');
    else
        disp('  Test 2 FAILED');
    end

    disp('arm_iir_lattice_init_f32 tests completed.');
end