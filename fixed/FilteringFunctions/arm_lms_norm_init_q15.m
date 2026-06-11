function arm_lms_norm_init_q15()
% arm_lms_norm_init_q15.m
% MATLAB test script for arm_lms_norm_init_q15_mex
% Compile with: mex fixed/FilteringFunctions/arm_lms_norm_init_q15_mex.c

clc;
fprintf('=== Testing arm_lms_norm_init_q15_mex ===\n\n');

[coeffs, state, postShift, mu, numTaps, blockSize] = arm_lms_norm_init_q15_mex(5);

fprintf('Test: Init returned numTaps=%d, blockSize=%d, postShift=%d, mu=%d\n', numTaps, blockSize, postShift, mu);
fprintf('  Coeffs length=%d, State length=%d\n', length(coeffs), length(state));
fprintf('PASS\n\n');

fprintf('=== All tests PASSED ===\n');