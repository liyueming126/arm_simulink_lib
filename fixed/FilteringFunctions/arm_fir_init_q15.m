function arm_fir_init_q15()
% arm_fir_init_q15.m
% MATLAB test script for arm_fir_init_q15_mex
% Compile with: mex fixed/FilteringFunctions/arm_fir_init_q15_mex.c

clc;
fprintf('=== Testing arm_fir_init_q15_mex ===\n\n');
errors = 0;

% Test 1: initialize with 4 taps
coeffs = int16([100, 200, 300, 400]);
state = arm_fir_init_q15_mex(4, coeffs, 10);
fprintf('Test 1 - Init 4 taps, blockSize=10:\n  State length: %d (expected 14)\n', length(state));

if length(state) == 14 && all(state == 0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: initialize with 6 taps
coeffs2 = int16([256, 512, 1024, 512, 256, 0]);
state2 = arm_fir_init_q15_mex(6, coeffs2, 5);
fprintf('Test 2 - Init 6 taps, blockSize=5:\n  State length: %d (expected 11)\n', length(state2));
if length(state2) == 11 && all(state2 == 0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: odd numTaps should error
try
    arm_fir_init_q15_mex(3, int16([1,2,3]), 10);
    fprintf('Test 3 - Odd taps: FAIL (should error)\n');
    errors = errors + 1;
catch
    fprintf('Test 3 - Odd taps: PASS (caught error)\n\n');
end

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end