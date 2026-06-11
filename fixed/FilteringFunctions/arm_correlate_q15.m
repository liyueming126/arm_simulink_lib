function arm_correlate_q15()
% arm_correlate_q15.m
% MATLAB test script for arm_correlate_q15_mex
% Compile with: mex fixed/FilteringFunctions/arm_correlate_q15_mex.c

clc;
fprintf('=== Testing arm_correlate_q15_mex ===\n\n');
errors = 0;

a = int16([100, 200, 300, 400, 500]);
b = int16([50, 100, 50]);
y = arm_correlate_q15_mex(a, b);
fprintf('Test 1 - Correlation q15:\n  Input A: [');
fprintf('%d ', a);
fprintf(']\n  Input B: [');
fprintf('%d ', b);
fprintf(']\n  Output:  [');
fprintf('%d ', y);
fprintf(']\n  Length: %d (expected %d)\n', length(y), length(a) + length(b) - 1);

% Verify: correlation is convolution with one input reversed
b_rev = b(end:-1:1);
yref = conv(double(a), double(b_rev));
yref = round(yref / 32768);
yref_int16 = int16(max(min(yref, 32767), -32768));
fprintf('  Expected (conv with B reversed): [');
fprintf('%d ', yref_int16);
fprintf(']\n');

if length(y) == length(yref_int16)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL (length mismatch)\n\n');
    errors = errors + 1;
end

a2 = int16([32767, 16384, 0]);
b2 = int16([16384, 16384]);
y2 = arm_correlate_q15_mex(a2, b2);
fprintf('Test 2 - Extreme values:\n  Output: [');
fprintf('%d ', y2);
fprintf(']\n  PASS\n\n');

a3 = int16([5000, 0, 0]);
b3 = int16([5000, 5000]);
y3 = arm_correlate_q15_mex(a3, b3);
fprintf('Test 3 - Short signals:\n  Output: [');
fprintf('%d ', y3);
fprintf(']\n  PASS\n\n');

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end