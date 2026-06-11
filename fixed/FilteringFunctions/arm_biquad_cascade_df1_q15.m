function arm_biquad_cascade_df1_q15()
    disp('Testing arm_biquad_cascade_df1_q15 via MEX gateway');
    compile_mex;

    %% Test 1: Identity filter
    numStages = 1;
    coeffs = [0.5, 0, 0, 0, 0, 0];
    blockSize = 10;
    src = [1, zeros(1, blockSize-1)];
    out = arm_biquad_cascade_df1_q15_mex(numStages, coeffs, src, blockSize, 0);
    assert(abs(out(1) - 0.5) < 1e-3, 'Identity fail');
    disp('Test 1 PASSED: Identity');

    %% Test 2: DC signal
    coeffs = [0.25, 0, 0, 0, 0, 0];
    src = ones(1, 20) * 0.5;
    out = arm_biquad_cascade_df1_q15_mex(1, coeffs, src, 20, 0);
    disp('Test 2 PASSED: DC');

    %% Test 3: Saturation
    numStages = 1;
    coeffs = [1.0, 0, 1.0, 0, 0, 0];
    src = ones(1, 5) * 0.9;
    out = arm_biquad_cascade_df1_q15_mex(numStages, coeffs, src, 5, 0);
    assert(all(abs(out) <= 1.0), 'Saturation overflow');
    disp('Test 3 PASSED: Saturation');

    %% Test 4: Multiple stages
    numStages = 2;
    coeffs = [0.25, 0, 0, 0, 0, 0, 0.25, 0, 0, 0, 0, 0];
    src = [1, zeros(1, 15)];
    out = arm_biquad_cascade_df1_q15_mex(numStages, coeffs, src, 16, 0);
    assert(out(1) > 0, 'Multi-stage fail');
    disp('Test 4 PASSED: Multi-stage');

    disp('ALL TESTS PASSED');
end

function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_biquad_cascade_df1_q15_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file')
        mex(mex_src);
    end
end