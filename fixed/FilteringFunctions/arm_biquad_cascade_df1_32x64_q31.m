function arm_biquad_cascade_df1_32x64_q31()
    disp('Testing arm_biquad_cas_df1_32x64_q31');
    compile_mex;

    numStages = 1;
    coeffs = [0.25, 0, 0, 0, 0];
    src = [0.5, zeros(1,9)];
    out = arm_biquad_cascade_df1_32x64_q31_mex(numStages, coeffs, src, 10, 0);
    assert(abs(out(1) - 0.125) < 1e-5, 'Impulse fail');
    disp('Test 1 PASSED: Impulse');

    coeffs = [0.5, 0, 0, 0, 0];
    src = ones(1,10)*0.125;
    out = arm_biquad_cascade_df1_32x64_q31_mex(1, coeffs, src, 10, 0);
    assert(all(abs(out) < 0.3), 'DC fail');
    disp('Test 2 PASSED: DC');

    disp('ALL TESTS PASSED');
end

function compile_mex
    mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_biquad_cascade_df1_32x64_q31_mex.c');
    if ~exist([mex_src(1:end-2) '.' mexext], 'file')
        mex(mex_src);
    end
end