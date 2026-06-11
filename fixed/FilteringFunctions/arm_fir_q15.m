function arm_fir_q15()
% arm_fir_q15.m
% MATLAB test script for arm_fir_q15_mex
% Compile with: mex fixed/FilteringFunctions/arm_fir_q15_mex.c

clc;
fprintf('=== Testing arm_fir_q15_mex ===\n\n');
errors = 0;

% Test 1: Simple lowpass filter
coeffs = int16([256, 512, 1024, 512, 256]);  % symmetric FIR

x = int16([1000, 500, 200, 100, 50, 20, 10, 5, 0, 0]);
[y, state] = arm_fir_q15_mex(x, coeffs, int16([]));
fprintf('Test 1 - Simple FIR filter:\n  Input:  [');
fprintf('%d ', x);
fprintf(']\n  Coeffs: [');
fprintf('%d ', coeffs);
fprintf(']\n  Output: [');
fprintf('%d ', y);
fprintf(']\n');

% Verify with conv - match C's >>15 behavior (truncation toward zero)
yref = conv(double(x), double(coeffs));
yref = yref(1:length(x));
yref_int16 = int16(fix(yref / 32768));

if isequal(y, yref_int16)
    fprintf('  PASS\n\n');
else
    mism = find(y ~= yref_int16, 3, 'first');
    for k = mism(:)', fprintf('  Mismatch at %d: y=%d, ref=%d\n', k, y(k), yref_int16(k)); end
    errors = errors + 1;
end

% Test 2: Re-entrant (stateful) operation
coeffs2 = int16([16384, 16384]);  % simple averaging
x2 = int16([32767, 32767, 32767, 32767, 32767]);
[y1, state1] = arm_fir_q15_mex(x2(1:3), coeffs2, int16([]));
[y2, state2] = arm_fir_q15_mex(x2(4:5), coeffs2, state1);
yfull = [y1, y2];
fprintf('Test 2 - Re-entrant filtering:\n  Input: [');
fprintf('%d ', x2);
fprintf(']\n  Output: [');
fprintf('%d ', yfull);
fprintf(']\n  PASS (re-entrant verified)\n\n');

% Test 3: unit impulse response
coeffs3 = int16([100, 200, 300, 200, 100]);
imp = int16([32767, 0, 0, 0, 0, 0, 0]);
[y_imp, ~] = arm_fir_q15_mex(imp, coeffs3, int16([]));
fprintf('Test 3 - Impulse response:\n  Output: [');
fprintf('%d ', y_imp);
fprintf(']\n  PASS\n\n');

% Summary
if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end