function arm_fir_lattice_q31()
    disp('Testing arm_fir_lattice_q31'); compile_mex;
    numStages = 1; coeffs = [0.0]; src = [0.25, -0.25, 0.25];
    out = arm_fir_lattice_q31_mex(numStages, coeffs, src, 3);
    assert(abs(out(1) - 0.25) < 1e-5, 'Identity fail');
    disp('Test PASSED'); disp('ALL TESTS PASSED');
end
function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_fir_lattice_q31_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file'), mex(mex_src); end
end