function arm_iir_lattice_q15()
    disp('Testing arm_iir_lattice_q15'); compile_mex;
    numStages = 1; pk = [0.0]; pv = [1.0, 0.0]; src = [0.5, -0.5, 0.5];
    out = arm_iir_lattice_q15_mex(numStages, pk, pv, src, 3);
    assert(length(out) == 3, 'Output length');
    disp('Test 1 PASSED');
    numStages = 2; pk = [0.0, 0.0]; pv = [0.0, 0.0, 1.0]; src = [0.5, 0.5];
    out = arm_iir_lattice_q15_mex(numStages, pk, pv, src, 2);
    assert(abs(out(1) - 0.5) < 1e-2, 'Pass-through fail');
    disp('Test 2 PASSED'); disp('ALL TESTS PASSED');
end
function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_iir_lattice_q15_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file'), mex(mex_src); end
end