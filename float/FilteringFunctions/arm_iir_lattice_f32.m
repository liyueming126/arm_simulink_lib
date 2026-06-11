function arm_iir_lattice_f32_test
    disp('Testing arm_iir_lattice_f32 ...');

    % Test 1: All k=0, v = [1,0,...], should be identity
    k = single([0.0, 0.0]);
    v = single([1.0, 0.0, 0.0]);
    x = single([1.0, 2.0, 3.0]);
    y = arm_iir_lattice_f32_mex(k, v, x);
    err = max(abs(y - x));
    if err < 1e-6
        disp('  Test 1 PASSED (identity)');
    else
        disp(['  Test 1 FAILED (max error = ' num2str(err) ')']);
    end

    % Test 2: Single stage, k=0, v=[0.5, 0]
    k = single([0.0]);
    v = single([0.5, 0.0]);
    x = single([2.0, 4.0]);
    y = arm_iir_lattice_f32_mex(k, v, x);
    y_expected = single([1.0, 2.0]);
    err = max(abs(y - y_expected));
    if err < 1e-6
        disp('  Test 2 PASSED (gain 0.5)');
    else
        disp(['  Test 2 FAILED (max error = ' num2str(err) ')']);
    end

    disp('arm_iir_lattice_f32 tests completed.');
end