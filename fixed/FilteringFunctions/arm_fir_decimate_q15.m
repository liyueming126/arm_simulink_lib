function arm_fir_decimate_q15()
    disp('Testing arm_fir_decimate_q15');
    compile_mex;

    M = 2; numTaps = 4;
    coeffs = [0.25, 0.25, 0.25, 0.25];
    src = [1, -1, 1, -1, 1, -1, 1, -1];
    out = arm_fir_decimate_q15_mex(M, numTaps, coeffs, src, 8);
    assert(length(out) == 4, 'Output length mismatch');
    disp('Test 1 PASSED');

    M = 1; numTaps = 1;
    coeffs = [1.0];
    src = [0.5, 0.5, 0.5, 0.5];
    out = arm_fir_decimate_q15_mex(M, numTaps, coeffs, src, 4);
    assert(all(abs(out - 0.5) < 1e-2), 'Pass-through fail');
    disp('Test 2 PASSED');

    M = 3; numTaps = 3;
    coeffs = [0.333, 0.333, 0.333];
    src = ones(1,9)*0.5;
    out = arm_fir_decimate_q15_mex(M, numTaps, coeffs, src, 9);
    assert(length(out) == 3, 'Decimate factor 3 fail');
    disp('Test 3 PASSED');

    disp('ALL TESTS PASSED');
end

function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_fir_decimate_q15_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file')
        mex(mex_src);
    end
end