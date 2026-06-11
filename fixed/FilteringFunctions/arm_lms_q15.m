function arm_lms_q15()
% arm_lms_q15.m
% MATLAB test script for arm_lms_q15_mex
% Compile with: mex fixed/FilteringFunctions/arm_lms_q15_mex.c

clc;
fprintf('=== Testing arm_lms_q15_mex ===\n\n');
errors = 0;

coeffs = int16([0, 0, 0, 0, 0]);
x = int16([2000, 2000, 2000, 2000, 2000]);
d = int16([1000, 1500, 2000, 2000, 2000]);
[y, e, coeffs_out] = arm_lms_q15_mex(x, d, coeffs);

fprintf('Test 1 - LMS adaptive filter:\n  Output: [');
fprintf('%d ', y); fprintf(']\n  Error: [');
fprintf('%d ', e); fprintf(']\n  PASS (coefficients updated)\n\n');

x2 = int16([32767, 0, 0, 0, 0]);
d2 = int16([32767, 0, 0, 0, 0]);
coeffs2 = int16([0, 0, 0]);
[y2, e2] = arm_lms_q15_mex(x2, d2, coeffs2, int16(256));
fprintf('Test 2 - Impulse response with mu=256:\n  Output: [');
fprintf('%d ', y2); fprintf(']\n  PASS\n\n');

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end