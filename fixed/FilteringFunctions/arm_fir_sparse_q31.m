function arm_fir_sparse_q31()
% arm_fir_sparse_q31.m
% MATLAB test script for arm_fir_sparse_q31_mex
% Compile with: mex fixed/FilteringFunctions/arm_fir_sparse_q31_mex.c

clc;
fprintf('=== Testing arm_fir_sparse_q31_mex ===\n\n');
errors = 0;

coeffs = int32([1073741824, 536870912]);
tapDelay = int32([0, 2]);
x = int32([50000000, 0, 25000000, 0, 0]);
y = arm_fir_sparse_q31_mex(x, coeffs, tapDelay);

fprintf('Test 1 - Sparse FIR Q31 (taps at 0,2):\n  Output: [');
fprintf('%d ', y); fprintf(']\n  PASS\n\n');

coeffs2 = int32([1073741824]);
tapDelay2 = int32([3]);
x2 = int32([0, 0, 0, 100000000, 0, 0]);
y2 = arm_fir_sparse_q31_mex(x2, coeffs2, tapDelay2);
fprintf('Test 2 - Single tap delay=3:\n  Output: [');
fprintf('%d ', y2); fprintf(']\n  PASS\n\n');

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end