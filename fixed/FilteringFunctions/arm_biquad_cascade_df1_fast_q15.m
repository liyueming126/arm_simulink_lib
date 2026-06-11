function arm_biquad_cascade_df1_fast_q15()
    disp('Testing arm_biquad_cascade_df1_fast_q15');

    %% Test 1: Impulse
    numStages = 1;
    coeffs = [0.5, 0.5, 0, 0, 0, 0];
    src = [1, zeros(1,9)];
    out = arm_biquad_cascade_df1_fast_q15_mex(numStages, coeffs, src, 10, 0);
    assert(abs(out(1)-0.5) < 1e-2, 'Impulse fail');
    disp('Test 1 PASSED');

    %% Test 2: DC scaled
    coeffs = [0.125, 0.125, 0, 0, 0, 0];
    src = ones(1,10)*0.5;
    out = arm_biquad_cascade_df1_fast_q15_mex(1, coeffs, src, 10, 0);
    assert(all(abs(out) < 0.5), 'DC fail');
    disp('Test 2 PASSED');

    disp('ALL TESTS PASSED');
end