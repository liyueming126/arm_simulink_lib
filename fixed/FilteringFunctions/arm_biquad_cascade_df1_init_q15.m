function arm_biquad_cascade_df1_init_q15()
    disp('Testing arm_biquad_cascade_df1_init_q15');
    compile_mex;

    numStages = 2;
    coeffs = zeros(1, 12);
    coeffs(1) = 0.5;
    [nStages, postShift] = arm_biquad_cascade_df1_init_q15_mex(numStages, coeffs);
    assert(nStages == 2, 'numStages mismatch');
    assert(postShift == 0, 'postShift mismatch');
    disp('Test PASSED: Init ok');

    disp('ALL TESTS PASSED');
end

function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_biquad_cascade_df1_init_q15_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file')
        mex(mex_src);
    end
end