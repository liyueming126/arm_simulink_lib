function arm_fir_lattice_f32_test
    disp('Testing arm_fir_lattice_f32 ...');

    % Test 1: Identity (single stage with k=0)
    coeffs = single([0.0]);
    x = single([1.0, 2.0, 3.0]);
    y = arm_fir_lattice_f32_mex(coeffs, x);
    err = max(abs(y - x));
    if err < 1e-6
        disp('  Test 1 PASSED (identity lattice)');
    else
        disp(['  Test 1 FAILED (max error = ' num2str(err) ')']);
    end

    % Test 2: Simple 2-stage lattice
    coeffs = single([0.5, 0.3]);
    x = single([1.0, 0.0, 0.0]);
    y = arm_fir_lattice_f32_mex(coeffs, x);
    % For impulse, y[0] = 1.0, y[1] = k1 + k1*k2 = 0.5 + 0.15 = 0.65, ...
    if abs(y(1) - 1.0) < 1e-6
        disp('  Test 2 PASSED (2-stage lattice)');
    else
        disp(['  Test 2 FAILED (y(1) = ' num2str(y(1)) ')']);
    end

    disp('arm_fir_lattice_f32 tests completed.');
end