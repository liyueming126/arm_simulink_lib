function arm_conv_partial_opt_q7()
% arm_conv_partial_opt_q7.m
% MATLAB test script for arm_conv_partial_opt_q7_mex
% Compile with: mex fixed/FilteringFunctions/arm_conv_partial_opt_q7_mex.c

clc;
fprintf('=== Testing arm_conv_partial_opt_q7_mex ===\n\n');
errors = 0;

a = int8([1, 2, 3, 4, 5]);
b = int8([5, 10, 5]);
y = arm_conv_partial_opt_q7_mex(a, b, 0, 3);
fprintf('Test 1 - Partial conv opt (first 3 outputs):\n  Input A: [');
fprintf('%d ', a);
fprintf(']\n  Input B: [');
fprintf('%d ', b);
fprintf(']\n  Output:  [');
fprintf('%d ', y);
fprintf(']\n  Length: %d (expected 3)\n', length(y));
if length(y) == 3
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

a2 = int8([1, 2, 3, 4, 5]);
b2 = int8([5, 10, 5]);
y2 = arm_conv_partial_opt_q7_mex(a2, b2, 2, 3);
fprintf('Test 2 - Partial conv opt (middle 3 outputs):\n  Output: [');
fprintf('%d ', y2);
fprintf(']\n  PASS\n\n');

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end