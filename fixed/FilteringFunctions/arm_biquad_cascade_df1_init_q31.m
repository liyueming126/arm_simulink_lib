function arm_biquad_cascade_df1_init_q31()
    disp('Testing arm_biquad_cascade_df1_init_q31');
    compile_mex;
    coeffs = [0.5, 0, 0, 0, 0];
    [nStages, postShift] = arm_biquad_cascade_df1_init_q31_mex(1, coeffs);
    assert(nStages == 1, 'numStages mismatch');
    disp('Test PASSED: Init ok');
    disp('ALL TESTS PASSED');
end

function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_biquad_cascade_df1_init_q31_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file')
        mex(mex_src);
    end
end