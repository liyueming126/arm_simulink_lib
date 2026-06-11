function arm_conv_partial_opt_q15()
% arm_conv_partial_opt_q15.m
% MATLAB test script for arm_conv_partial_opt_q15_mex
% Compile with: mex fixed/FilteringFunctions/arm_conv_partial_opt_q15_mex.c

clc;
fprintf('=== Testing arm_conv_partial_opt_q15_mex ===\n\n');
errors = 0;

A = int16([1000, 2000, 3000, 4000]);
B = int16([1000, 1000]);
y = arm_conv_partial_opt_q15_mex(A, B, uint32(1), uint32(3));

fprintf('Test 1 - Partial convolution (start=1, 3 points):\n  A: [');
fprintf('%d ', A); fprintf(']\n  B: [');
fprintf('%d ', B); fprintf(']\n  Output: [');
fprintf('%d ', y); fprintf(']\n  PASS\n\n');

A2 = int16([20000, 10000]);
B2 = int16([16384, 8192]);
y2 = arm_conv_partial_opt_q15_mex(A2, B2, uint32(0), uint32(3));
fprintf('Test 2 - Full range:\n  Output: [');
fprintf('%d ', y2); fprintf(']\n  PASS\n\n');

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end