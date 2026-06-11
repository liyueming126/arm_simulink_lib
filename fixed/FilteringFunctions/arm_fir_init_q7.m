function arm_fir_init_q7()
% arm_fir_init_q7.m
% MATLAB test script for arm_fir_init_q7_mex
% Compile with: mex fixed/FilteringFunctions/arm_fir_init_q7_mex.c

clc;
fprintf('=== Testing arm_fir_init_q7_mex ===\n\n');
errors = 0;

coeffs = int8([1, 2, 3, 4]);
state = arm_fir_init_q7_mex(4, coeffs, 10);
fprintf('Test 1 - Init 4 taps, blockSize=10:\n  State length: %d (expected 13)\n', length(state));

if length(state) == 13 && all(state == 0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

coeffs2 = int8([2, 4, 8, 4, 2, 0]);
state2 = arm_fir_init_q7_mex(6, coeffs2, 5);
fprintf('Test 2 - Init 6 taps, blockSize=5:\n  State length: %d (expected 10)\n', length(state2));
if length(state2) == 10 && all(state2 == 0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end