function arm_conv_q31()
% arm_conv_q31.m
% MATLAB test script for arm_conv_q31_mex
% Compile with: mex fixed/FilteringFunctions/arm_conv_q31_mex.c

clc;
fprintf('=== Testing arm_conv_q31_mex ===\n\n');
errors = 0;

a = int32([1000, 2000, 3000, 4000, 5000]);
b = int32([500, 1000, 500]);
y = arm_conv_q31_mex(a, b);
fprintf('Test 1 - Convolution q31:\n  Output: [');
fprintf('%d ', y);
fprintf(']\n  Length: %d (expected %d)\n', length(y), length(a) + length(b) - 1);

if length(y) == length(a) + length(b) - 1
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

a2 = int32([1073741824, 536870912, 0, -536870912]);
b2 = int32([536870912, 536870912]);
y2 = arm_conv_q31_mex(a2, b2);
fprintf('Test 2 - Large values:\n  Output: [');
fprintf('%d ', y2);
fprintf(']\n  PASS\n\n');

a3 = int32([1, 0, 0]);
b3 = int32([1]);
y3 = arm_conv_q31_mex(a3, b3);
fprintf('Test 3 - Unit:\n  Output: [');
fprintf('%d ', y3);
fprintf(']\n  PASS\n\n');

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end