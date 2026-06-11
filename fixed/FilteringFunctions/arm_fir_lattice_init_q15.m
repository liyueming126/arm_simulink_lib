function arm_fir_lattice_init_q15()
    disp('Testing arm_fir_lattice_init_q15'); compile_mex;
    numStages = arm_fir_lattice_init_q15_mex(3, [0.5, 0.25, 0.125]);
    assert(numStages == 3, 'numStages mismatch');
    disp('Test PASSED'); disp('ALL TESTS PASSED');
end
function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_fir_lattice_init_q15_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file'), mex(mex_src); end
end