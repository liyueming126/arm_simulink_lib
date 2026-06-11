function arm_fir_init_q31()
% arm_fir_init_q31.m
% MATLAB test script for arm_fir_init_q31_mex
% Compile with: mex fixed/FilteringFunctions/arm_fir_init_q31_mex.c

clc;
fprintf('=== Testing arm_fir_init_q31_mex ===\n\n');
errors = 0;

coeffs = int32([100, 200, 300, 400]);
state = arm_fir_init_q31_mex(4, coeffs, 10);
fprintf('Test 1 - Init 4 taps, blockSize=10:\n  State length: %d (expected 13)\n', length(state));
if length(state) == 13 && all(state == 0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

coeffs2 = int32([256, 512, 1024, 512, 256]);
state2 = arm_fir_init_q31_mex(5, coeffs2, 5);
fprintf('Test 2 - Init 5 taps, blockSize=5:\n  State length: %d (expected 9)\n', length(state2));
if length(state2) == 9 && all(state2 == 0)
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