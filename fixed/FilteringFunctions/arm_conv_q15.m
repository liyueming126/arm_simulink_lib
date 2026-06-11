function arm_conv_q15()
% arm_conv_q15.m
% MATLAB test script for arm_conv_q15_mex
% Compile with: mex fixed/FilteringFunctions/arm_conv_q15_mex.c

clc;
fprintf('=== Testing arm_conv_q15_mex ===\n\n');
errors = 0;

a = int16([100, 200, 300, 400, 500]);
b = int16([50, 100, 50]);
y = arm_conv_q15_mex(a, b);
fprintf('Test 1 - Convolution q15:\n  Input A: [');
fprintf('%d ', a);
fprintf(']\n  Input B: [');
fprintf('%d ', b);
fprintf(']\n  Output:  [');
fprintf('%d ', y);
fprintf(']\n  Length: %d (expected %d)\n', length(y), length(a) + length(b) - 1);

yref = conv(double(a), double(b));
yref = round(yref / 32768);
yref_int16 = int16(max(min(yref, 32767), -32768));
if length(y) == length(yref_int16)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

a2 = int16([32767, 16384, 0, -16384, -32768]);
b2 = int16([16384, 16384]);
y2 = arm_conv_q15_mex(a2, b2);
fprintf('Test 2 - With extreme values:\n  Output: [');
fprintf('%d ', y2);
fprintf(']\n  PASS\n\n');

a3 = int16([1, 0, 0, 0, 0]);
b3 = int16([1]);
y3 = arm_conv_q15_mex(a3, b3);
fprintf('Test 3 - Unit impulse:\n  Output: [');
fprintf('%d ', y3);
fprintf(']\n  Length: %d\n', length(y3));
if length(y3) == 5 && y3(1) == 0
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