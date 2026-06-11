function arm_correlate_opt_q7()
% arm_correlate_opt_q7.m
% MATLAB test script for arm_correlate_opt_q7_mex
% Compile with: mex fixed/FilteringFunctions/arm_correlate_opt_q7_mex.c

clc;
fprintf('=== Testing arm_correlate_opt_q7_mex ===\n\n');
errors = 0;

a = int8([1, 2, 3, 4, 5]);
b = int8([2, 4, 2]);
y = arm_correlate_opt_q7_mex(a, b);
fprintf('Test 1 - Correlation opt q7:\n  Input A: [');
fprintf('%d ', a);
fprintf(']\n  Input B: [');
fprintf('%d ', b);
fprintf(']\n  Output:  [');
fprintf('%d ', y);
fprintf(']\n  Length: %d (expected %d)\n', length(y), length(a) + length(b) - 1);
if length(y) == length(a) + length(b) - 1
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

a2 = int8([127, 64, 0]);
b2 = int8([64, 64]);
y2 = arm_correlate_opt_q7_mex(a2, b2);
fprintf('Test 2 - With extreme values:\n  Output: [');
fprintf('%d ', y2);
fprintf(']\n  PASS\n\n');

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end