function arm_fir_lattice_q15()
    disp('Testing arm_fir_lattice_q15'); compile_mex;
    numStages = 1; coeffs = [0.0]; src = [0.5, -0.5, 0.5, -0.5];
    out = arm_fir_lattice_q15_mex(numStages, coeffs, src, 4);
    assert(abs(out(1) - 0.5) < 1e-2, 'Identity fail');
    disp('Test 1 PASSED');
    numStages = 2; coeffs = [0.5, 0.25]; src = [0.5, zeros(1,3)];
    out = arm_fir_lattice_q15_mex(numStages, coeffs, src, 4);
    assert(length(out) == 4, 'Output length'); disp('Test 2 PASSED');
    disp('ALL TESTS PASSED');
end
function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_fir_lattice_q15_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file'), mex(mex_src); end
end