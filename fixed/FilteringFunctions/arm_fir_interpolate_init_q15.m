function arm_fir_interpolate_init_q15()
    disp('Testing arm_fir_interpolate_init_q15'); compile_mex;
    coeffs = [0.5, 0.5, 0.5, 0.5];
    [L, phaseLen, status] = arm_fir_interpolate_init_q15_mex(2, 4, coeffs, 3);
    assert(L == 2 && phaseLen == 2 && status == 0, 'Init fail');
    [L, phaseLen, status] = arm_fir_interpolate_init_q15_mex(3, 4, coeffs, 3);
    assert(status == 1, 'Should fail for non-multiple');
    disp('Test PASSED'); disp('ALL TESTS PASSED');
end
function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_fir_interpolate_init_q15_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file'), mex(mex_src); end
end