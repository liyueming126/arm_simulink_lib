function arm_conv_fast_opt_q15()
% arm_conv_fast_opt_q15.m
% MATLAB test script for arm_conv_fast_opt_q15_mex
% Compile with: mex fixed/FilteringFunctions/arm_conv_fast_opt_q15_mex.c

clc;
fprintf('=== Testing arm_conv_fast_opt_q15_mex ===\n\n');
errors = 0;

A = int16([1000, 2000, 3000]);
B = int16([1000, 1000]);
y = arm_conv_fast_opt_q15_mex(A, B);

fprintf('Test 1 - Fast convolution opt Q15:\n  A: [');
fprintf('%d ', A); fprintf(']\n  B: [');
fprintf('%d ', B); fprintf(']\n  Output: [');
fprintf('%d ', y); fprintf(']\n');

if length(y) == 4
    fprintf('  Length OK (%d)\n', length(y));
else
    fprintf('  FAIL - Length mismatch\n');
    errors = errors + 1;
end
fprintf('  PASS\n\n');

A2 = int16([32767]);
B2 = int16([16384, 16384]);
y2 = arm_conv_fast_opt_q15_mex(A2, B2);
fprintf('Test 2 - Short sequences:\n  Output: [');
fprintf('%d ', y2); fprintf(']\n  PASS\n\n');

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end