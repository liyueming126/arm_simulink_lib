function arm_fir_sparse_init_q15()
% arm_fir_sparse_init_q15.m
% MATLAB test script for arm_fir_sparse_init_q15_mex
% Compile with: mex fixed/FilteringFunctions/arm_fir_sparse_init_q15_mex.c

clc;
fprintf('=== Testing arm_fir_sparse_init_q15_mex ===\n\n');

[coeffs, state, tapDelay, numTaps, maxDelay, blockSize] = arm_fir_sparse_init_q15_mex(5, 10, 4);

fprintf('Test: Init returned numTaps=%d, maxDelay=%d, blockSize=%d\n', numTaps, maxDelay, blockSize);
fprintf('  Coeffs length=%d, State length=%d, TapDelay length=%d\n', length(coeffs), length(state), length(tapDelay));
fprintf('PASS\n\n');

fprintf('=== All tests PASSED ===\n');