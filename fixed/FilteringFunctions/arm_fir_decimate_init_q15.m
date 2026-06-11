function arm_fir_decimate_init_q15()
    disp('Testing arm_fir_decimate_init_q15'); compile_mex;
    coeffs = [0.5, 0.5, 0.5, 0.5];
    [numTaps, M, status] = arm_fir_decimate_init_q15_mex(4, 2, coeffs, 8);
    assert(numTaps == 4 && M == 2 && status == 0, 'Init fail');
    disp('Test 1 PASSED');
    [numTaps, M, status] = arm_fir_decimate_init_q15_mex(4, 3, coeffs, 8);
    assert(status == 1, 'Should fail for non-multiple');
    disp('Test 2 PASSED');
    disp('ALL TESTS PASSED');
end
function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_fir_decimate_init_q15_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file'), mex(mex_src); end
end