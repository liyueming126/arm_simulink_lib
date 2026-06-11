function arm_fir_lattice_init_f32_test
    disp('Testing arm_fir_lattice_init_f32 ...');

    numStages = uint16(4);
    coeffs = single([0.5, 0.3, 0.2, 0.1]);
    state = arm_fir_lattice_init_f32_mex(numStages, coeffs);

    if all(state(:) == 0)
        disp('  Test 1 PASSED (state buffer zeroed)');
    else
        disp('  Test 1 FAILED');
    end

    if length(state) == numStages
        disp('  Test 2 PASSED (state buffer correct length)');
    else
        disp('  Test 2 FAILED');
    end

    disp('arm_fir_lattice_init_f32 tests completed.');
end