function arm_iir_lattice_q31()
    disp('Testing arm_iir_lattice_q31'); compile_mex;
    numStages = 1; pk = [0.0]; pv = [1.0, 0.0]; src = [0.25, -0.25];
    out = arm_iir_lattice_q31_mex(numStages, pk, pv, src, 2);
    assert(length(out) == 2, 'Output length');
    disp('Test 1 PASSED'); disp('ALL TESTS PASSED');
end
function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_iir_lattice_q31_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file'), mex(mex_src); end
end