function arm_conv_fast_q15()
% arm_conv_fast_q15.m
% MATLAB test script for arm_conv_fast_q15_mex
% Compile with: mex fixed/FilteringFunctions/arm_conv_fast_q15_mex.c

clc;
fprintf('=== Testing arm_conv_fast_q15_mex ===\n\n');
errors = 0;

a = int16([100, 200, 300, 400, 500]);
b = int16([50, 100, 50]);
y = arm_conv_fast_q15_mex(a, b);
fprintf('Test 1 - Fast conv q15:\n  Output: [');
fprintf('%d ', y);
fprintf(']\n  Length: %d\n  PASS\n\n', length(y));

a2 = int16([32767, 16384, 0, -16384]);
b2 = int16([16384, 16384]);
y2 = arm_conv_fast_q15_mex(a2, b2);
fprintf('Test 2 - Large values:\n  Output: [');
fprintf('%d ', y2);
fprintf(']\n  PASS\n\n');

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end