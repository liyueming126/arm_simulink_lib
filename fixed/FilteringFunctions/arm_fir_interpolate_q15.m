function arm_fir_interpolate_q15()
    disp('Testing arm_fir_interpolate_q15');
    L = 2; numTaps = 4; coeffs = [0.25, 0, 0.25, 0]; src = [1, 0.5];
    out = arm_fir_interpolate_q15_mex(L, numTaps, coeffs, src, 2);
    out = arm_fir_interpolate_q15_mex(L, numTaps, coeffs, src, 2);
    assert(length(out) == 4, 'Output length mismatch');
    disp('Test 1 PASSED');
    L = 1; numTaps = 1; coeffs = [1.0]; src = [0.5, -0.5];
    out = arm_fir_interpolate_q15_mex(L, numTaps, coeffs, src, 2);
    % Transpose out to match src orientation if needed
    if size(out,1) ~= size(src,1), out = out'; end
    assert(all(abs(out - src) < 1e-2), 'Pass-through fail');
    disp('Test 2 PASSED'); disp('ALL TESTS PASSED');
end