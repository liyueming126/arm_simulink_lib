function arm_iir_lattice_init_q31()
    disp('Testing arm_iir_lattice_init_q31'); compile_mex;
    numStages = arm_iir_lattice_init_q31_mex(1, [0.0], [1.0, 0.0], 5);
    assert(numStages == 1, 'numStages mismatch');
    disp('Test PASSED'); disp('ALL TESTS PASSED');
end
function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_iir_lattice_init_q31_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file'), mex(mex_src); end
end