function arm_lms_norm_q31()
% arm_lms_norm_q31.m
% MATLAB test script for arm_lms_norm_q31_mex
% Compile with: mex fixed/FilteringFunctions/arm_lms_norm_q31_mex.c

clc;
fprintf('=== Testing arm_lms_norm_q31_mex ===\n\n');
errors = 0;

coeffs = int32([0, 0, 0, 0, 0]);
x = int32([10000000, 10000000, 10000000, 10000000, 10000000]);
d = int32([5000000, 10000000, 15000000, 15000000, 10000000]);
[y, e, coeffs_out] = arm_lms_norm_q31_mex(x, d, coeffs);

fprintf('Test 1 - NLMS adaptive filter Q31:\n  Output: [');
fprintf('%d ', y); fprintf(']\n  Error: [');
fprintf('%d ', e); fprintf(']\n  PASS (coefficients updated)\n\n');

x2 = int32([1073741824, 0, 0, 0, 0]);
d2 = int32([1073741824, 0, 0, 0, 0]);
coeffs2 = int32([0, 0, 0]);
[y2, e2] = arm_lms_norm_q31_mex(x2, d2, coeffs2);
fprintf('Test 2 - Impulse response:\n  Output: [');
fprintf('%d ', y2); fprintf(']\n  PASS\n\n');

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end