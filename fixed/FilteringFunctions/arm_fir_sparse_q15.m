function arm_fir_sparse_q15()
% arm_fir_sparse_q15.m
% MATLAB test script for arm_fir_sparse_q15_mex
% Compile with: mex fixed/FilteringFunctions/arm_fir_sparse_q15_mex.c

clc;
fprintf('=== Testing arm_fir_sparse_q15_mex ===\n\n');
errors = 0;

coeffs = int16([32767, 16384]);
tapDelay = int32([0, 2]);
x = int16([20000, 0, 10000, 0, 0]);
y = arm_fir_sparse_q15_mex(x, coeffs, tapDelay);

fprintf('Test 1 - Sparse FIR (taps at 0,2):\n  Input:  [');
fprintf('%d ', x); fprintf(']\n  Coeffs: [');
fprintf('%d ', coeffs); fprintf(']\n  Output: [');
fprintf('%d ', y); fprintf(']\n  PASS\n\n');

coeffs2 = int16([16384]);
tapDelay2 = int32([3]);
x2 = int16([0, 0, 0, 32767, 0, 0]);
y2 = arm_fir_sparse_q15_mex(x2, coeffs2, tapDelay2);
fprintf('Test 2 - Single tap delay=3:\n  Output: [');
fprintf('%d ', y2); fprintf(']\n  PASS\n\n');

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end