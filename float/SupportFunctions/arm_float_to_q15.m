function arm_float_to_q15()
clc;
fprintf('=== Testing arm_float_to_q15 ===\n\n');
errors = 0;

% Test 1: Basic conversion
fprintf('Test 1 - Basic conversion:\n');
x = single([0, 0.5, -0.5, 1, -1, 0.25, -0.25]);
y = arm_float_to_q15_mex(x);
expected = int16(round(double(x) * 32768));
actual = double(y);
exp_d = double(expected);
d = max(abs(actual - exp_d));
if d < 2
    fprintf('  PASS (max diff = %d)\n\n', d);
else
    fprintf('  FAIL (max diff = %d)\n\n', d);
    errors = errors + 1;
end

% Test 2: Saturation test
fprintf('Test 2 - Saturation:\n');
x = single([2.0, -2.0, 1.5, -1.5]);
y = arm_float_to_q15_mex(x);
if y(1) == 32767 && y(2) == -32768 && y(3) == 32767 && y(4) == -32768
    fprintf('  PASS (saturation works)\n\n');
else
    fprintf('  FAIL (got [%d %d %d %d])\n\n', y(1), y(2), y(3), y(4));
    errors = errors + 1;
end

% Test 3: Round-trip with q15_to_float
fprintf('Test 3 - Round-trip via division:\n');
x = single(randn(1, 20) * 0.5);
y = arm_float_to_q15_mex(x);
x2 = single(double(y) / 32768.0);
d = max(abs(x - x2));
fprintf('  Round-trip error: %e\n', d);
if d < 1e-4
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
end