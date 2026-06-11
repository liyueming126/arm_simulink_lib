function arm_fir_lattice_init_q31()
    disp('Testing arm_fir_lattice_init_q31'); compile_mex;
    numStages = arm_fir_lattice_init_q31_mex(2, [0.5, 0.25]);
    assert(numStages == 2, 'numStages mismatch');
    disp('Test PASSED'); disp('ALL TESTS PASSED');
end
function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_fir_lattice_init_q31_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file'), mex(mex_src); end
end