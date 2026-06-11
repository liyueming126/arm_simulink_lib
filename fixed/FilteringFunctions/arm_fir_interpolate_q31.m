function arm_fir_interpolate_q31()
    disp('Testing arm_fir_interpolate_q31'); compile_mex;
    L = 2; numTaps = 2; coeffs = [0.5, 0.5]; src = [0.5, -0.5];
    out = arm_fir_interpolate_q31_mex(L, numTaps, coeffs, src, 2);
    assert(length(out) == 4, 'Output length mismatch');
    disp('Test 1 PASSED'); disp('ALL TESTS PASSED');
end
function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_fir_interpolate_q31_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file'), mex(mex_src); end
end