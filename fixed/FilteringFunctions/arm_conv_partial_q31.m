function arm_conv_partial_q31()
% arm_conv_partial_q31.m
% MATLAB test script for arm_conv_partial_q31_mex
% Compile with: mex fixed/FilteringFunctions/arm_conv_partial_q31_mex.c

clc;
fprintf('=== Testing arm_conv_partial_q31_mex ===\n\n');
errors = 0;

a = int32([1000, 2000, 3000, 4000, 5000]);
b = int32([500, 1000, 500]);
y = arm_conv_partial_q31_mex(a, b, 2, 3);
fprintf('Test 1 - Partial conv q31, firstIndex=2, numPoints=3:\n  Output: [');
fprintf('%d ', y);
fprintf(']\n  PASS\n\n');

a2 = int32([1073741824, 536870912, 0]);
b2 = int32([536870912, 536870912]);
y2 = arm_conv_partial_q31_mex(a2, b2, 0, 2);
fprintf('Test 2 - First 2 points:\n  Output: [');
fprintf('%d ', y2);
fprintf(']\n  PASS\n\n');

try
    arm_conv_partial_q31_mex(a, b, 10, 3);
    fprintf('Test 3 - Out of range: FAIL\n');
    errors = errors + 1;
catch
    fprintf('Test 3 - Out of range: PASS\n\n');
end

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end