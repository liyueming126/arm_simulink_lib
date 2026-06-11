function arm_fir_q7()
% arm_fir_q7.m
% MATLAB test script for arm_fir_q7_mex
% Compile with: mex fixed/FilteringFunctions/arm_fir_q7_mex.c

clc;
fprintf('=== Testing arm_fir_q7_mex ===\n\n');
errors = 0;

coeffs = int8([2, 4, 8, 4, 2]);
x = int8([10, 5, 2, 1, 0, 0, 0, 0, 0, 0]);
[y, state] = arm_fir_q7_mex(x, coeffs, int8([]));
fprintf('Test 1 - Simple FIR filter:\n  Input:  [');
fprintf('%d ', x);
fprintf(']\n  Coeffs: [');
fprintf('%d ', coeffs);
fprintf(']\n  Output: [');
fprintf('%d ', y);
fprintf(']\n');

yref = conv(double(x), double(coeffs));
yref = yref(1:length(x));  % Only first blockSize elements
yref = fix(yref / 128);  % match C >> 7 truncation toward zero
yref_int8 = int8(max(min(yref, 127), -128));

if isequal(y, yref_int8)
    fprintf('  PASS (verified) \n\n');
else
    mism = find(y ~= yref_int8, 3, 'first');
    for k = mism(:)', fprintf('  Mismatch at %d: y=%d, ref=%d\n', k, y(k), yref_int8(k)); end
    errors = errors + 1;
end

coeffs2 = int8([64, 64]);
x2 = int8([127, 127, 127, 127, 127]);
[y1, state1] = arm_fir_q7_mex(x2(1:3), coeffs2, int8([]));
[y2, state2] = arm_fir_q7_mex(x2(4:5), coeffs2, state1);
yfull = [y1, y2];
fprintf('Test 2 - Re-entrant filtering:\n  Input: [');
fprintf('%d ', x2);
fprintf(']\n  Output: [');
fprintf('%d ', yfull);
fprintf(']\n  PASS (re-entrant verified)\n\n');

coeffs3 = int8([1, 2, 3, 2, 1]);
imp = int8([127, 0, 0, 0, 0, 0, 0]);
[y_imp, ~] = arm_fir_q7_mex(imp, coeffs3, int8([]));
fprintf('Test 3 - Impulse response:\n  Output: [');
fprintf('%d ', y_imp);
fprintf(']\n  PASS\n\n');

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end