function arm_q7_to_float()
clc;
fprintf('=== Testing arm_q7_to_float ===\n\n');
errors = 0;

% Test 1: Basic conversion
fprintf('Test 1 - Basic conversion:\n');
x = int8([0, 64, -64, 127, -128]);
y = arm_q7_to_float_mex(x);
expected = single(double(x) / 128.0);
d = max(abs(y - expected));
if d < 1e-7
    fprintf('  PASS (max diff = %e)\n\n', d);
else
    fprintf('  FAIL (max diff = %e)\n\n', d);
    errors = errors + 1;
end

% Test 2: Round-trip with float_to_q7
fprintf('Test 2 - Round-trip with float_to_q7:\n');
x = single(randn(1, 30) * 0.5);
q = arm_float_to_q7_mex(x);
x2 = arm_q7_to_float_mex(q);
d = max(abs(double(x) - double(x2)));
fprintf('  Round-trip error: %e\n', d);
if d < 2e-2
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: Small values
fprintf('Test 3 - Small values:\n');
x = int8([-1, 0, 1, -2, 2]);
y = arm_q7_to_float_mex(x);
expected = single(double(x) / 128.0);
d = max(abs(y - expected));
if d < 1e-8
    fprintf('  PASS (max diff = %e)\n\n', d);
else
    fprintf('  FAIL (max diff = %e)\n\n', d);
    errors = errors + 1;
end

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end
end