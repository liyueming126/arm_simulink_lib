function arm_fir_decimate_q31()
    disp('Testing arm_fir_decimate_q31');
    compile_mex;

    M = 2; numTaps = 2; coeffs = [0.5, 0.5];
    src = [1, -1, 1, -1];
    out = arm_fir_decimate_q31_mex(M, numTaps, coeffs, src, 4);
    assert(length(out) == 2, 'Output length mismatch');
    disp('Test 1 PASSED');

    M = 1; numTaps = 1; coeffs = [1.0];
    src = [0.25, 0.25, 0.25, 0.25];
    out = arm_fir_decimate_q31_mex(M, numTaps, coeffs, src, 4);
    assert(all(abs(out - 0.25) < 1e-5), 'Pass-through fail');
    disp('Test 2 PASSED');

    disp('ALL TESTS PASSED');
end

function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_fir_decimate_q31_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file')
        mex(mex_src);
    end
end