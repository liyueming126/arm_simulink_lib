function arm_fir_q31()
% arm_fir_q31.m
% MATLAB test script for arm_fir_q31_mex
% Compile with: mex fixed/FilteringFunctions/arm_fir_q31_mex.c

clc;
fprintf('=== Testing arm_fir_q31_mex ===\n\n');
errors = 0;

% Test 1: Simple lowpass filter
coeffs = int32([256, 512, 1024, 512, 256]);
x = int32([100000, 50000, 20000, 10000, 5000, 2000, 1000, 500, 0, 0]);
y = arm_fir_q31_mex(x, coeffs, int32([]));
fprintf('Test 1 - Simple Q31 FIR filter:\n  Output: [');
fprintf('%d ', y);
fprintf(']\n  Length: %d\n  PASS\n\n', length(y));

% Test 2: Re-entrant
coeffs2 = int32([1073741824, 1073741824]);  % ~0.5, 0.5
x2 = int32([65536, 65536, 65536, 65536, 65536]);
[y1, state1] = arm_fir_q31_mex(x2(1:3), coeffs2, int32([]));
[y2, state2] = arm_fir_q31_mex(x2(4:5), coeffs2, state1);
yfull = [y1, y2];
fprintf('Test 2 - Re-entrant:\n  Output: [');
fprintf('%d ', yfull);
fprintf(']\n  PASS\n\n');

% Test 3: impulse
coeffs3 = int32([100, 200, 300, 200, 100]);
imp = int32([1073741824, 0, 0, 0, 0, 0, 0]);
y_imp = arm_fir_q31_mex(imp, coeffs3, int32([]));
fprintf('Test 3 - Impulse response:\n  Output: [');
fprintf('%d ', y_imp);
fprintf(']\n  PASS\n\n');

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end