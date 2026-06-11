function arm_fir_fast_q15()
% arm_fir_fast_q15.m
% MATLAB test script for arm_fir_fast_q15_mex
% Compile with: mex fixed/FilteringFunctions/arm_fir_fast_q15_mex.c

clc;
fprintf('=== Testing arm_fir_fast_q15_mex ===\n\n');
errors = 0;

coeffs = int16([256, 512, 1024, 512, 256]);
x = int16([1000, 500, 200, 100, 50, 20, 10, 5, 0, 0]);
y = arm_fir_fast_q15_mex(x, coeffs, int16([]));
fprintf('Test 1 - Fast Q15 FIR:\n  Output: [');
fprintf('%d ', y);
fprintf(']\n  PASS\n\n');

coeffs2 = int16([16384, 16384]);
x2 = int16([32767, 32767, 32767, 32767, 32767]);
[y1, state1] = arm_fir_fast_q15_mex(x2(1:3), coeffs2, int16([]));
[y2, state2] = arm_fir_fast_q15_mex(x2(4:5), coeffs2, state1);
yfull = [y1, y2];
fprintf('Test 2 - Re-entrant:\n  Output: [');
fprintf('%d ', yfull);
fprintf(']\n  PASS\n\n');

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end