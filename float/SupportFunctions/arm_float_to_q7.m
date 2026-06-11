function arm_float_to_q7()
clc;
fprintf('=== Testing arm_float_to_q7 ===\n\n');
errors = 0;

% Test 1: Basic conversion
fprintf('Test 1 - Basic conversion:\n');
x = single([0, 0.5, -0.5, 1, -1, 0.25]);
y = arm_float_to_q7_mex(x);
ref = int8(round(double(x) * 128));
if all(y == ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: Saturation
fprintf('Test 2 - Saturation:\n');
x = single([2.0, -2.0]);
y = arm_float_to_q7_mex(x);
if y(1) == 127 && y(2) == -128
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL (got %d %d)\n\n', y(1), y(2));
    errors = errors + 1;
end

% Test 3: Edge values
fprintf('Test 3 - Edge values:\n');
x = single([1.0, -1.0, 0.9921875, -0.9921875]);
y = arm_float_to_q7_mex(x);
ref = int8(round(double(x) * 128));
if all(y == ref)
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