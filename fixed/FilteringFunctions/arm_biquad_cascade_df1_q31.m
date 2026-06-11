function arm_biquad_cascade_df1_q31()
    disp('Testing arm_biquad_cascade_df1_q31 via MEX gateway');
    compile_mex;

    %% Test 1: Identity
    numStages = 1;
    coeffs = [0.5, 0, 0, 0, 0];
    src = [1, zeros(1, 9)];
    out = arm_biquad_cascade_df1_q31_mex(numStages, coeffs, src, 10, 0);
    assert(abs(out(1) - 0.5) < 1e-6, 'Identity fail');
    disp('Test 1 PASSED: Identity');

    %% Test 2: DC gain
    coeffs = [0.25, 0, 0, 0, 0];
    src = ones(1, 10) * 0.25;
    out = arm_biquad_cascade_df1_q31_mex(1, coeffs, src, 10, 0);
    disp('Test 2 PASSED: DC');

    %% Test 3: Saturation
    coeffs = [1.0, 0, 0.8, 0, 0];
    src = ones(1, 5) * 0.9;
    out = arm_biquad_cascade_df1_q31_mex(1, coeffs, src, 5, 0);
    assert(all(abs(out) <= 1.0), 'Overflow');
    disp('Test 3 PASSED: Saturation');

    disp('ALL TESTS PASSED');
end

function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_biquad_cascade_df1_q31_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file')
        mex(mex_src);
    end
end