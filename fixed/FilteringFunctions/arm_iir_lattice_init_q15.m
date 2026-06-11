function arm_iir_lattice_init_q15()
    disp('Testing arm_iir_lattice_init_q15'); compile_mex;
    numStages = arm_iir_lattice_init_q15_mex(2, [0.5, 0.25], [1.0, 0.5, 0.25], 10);
    assert(numStages == 2, 'numStages mismatch');
    disp('Test PASSED'); disp('ALL TESTS PASSED');
end
function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_iir_lattice_init_q15_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file'), mex(mex_src); end
end