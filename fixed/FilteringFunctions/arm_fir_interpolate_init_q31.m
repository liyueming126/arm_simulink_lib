function arm_fir_interpolate_init_q31()
    disp('Testing arm_fir_interpolate_init_q31'); compile_mex;
    coeffs = [0.5, 0.5];
    [L, phaseLen, status] = arm_fir_interpolate_init_q31_mex(2, 2, coeffs, 3);
    assert(L == 2 && status == 0, 'Init fail');
    disp('Test PASSED'); disp('ALL TESTS PASSED');
end
function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_fir_interpolate_init_q31_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file'), mex(mex_src); end
end