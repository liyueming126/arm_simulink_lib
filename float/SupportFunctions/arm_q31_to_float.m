function arm_q31_to_float()
clc;
fprintf('=== Testing arm_q31_to_float ===\n\n');
errors = 0;

% Test 1: Basic conversion
fprintf('Test 1 - Basic conversion:\n');
x = int32([0, 1073741824, -1073741824, 2147483647, -2147483648]);
y = arm_q31_to_float_mex(x);
expected = single(double(x) / 2147483648.0);
d = max(abs(y - expected));
if d < 1e-7
    fprintf('  PASS (max diff = %e)\n\n', d);
else
    fprintf('  FAIL (max diff = %e)\n\n', d);
    errors = errors + 1;
end

% Test 2: Round-trip with float_to_q31
fprintf('Test 2 - Round-trip with float_to_q31:\n');
x = single(randn(1, 50) * 0.8);
q = arm_float_to_q31_mex(x);
x2 = arm_q31_to_float_mex(q);
d = max(abs(double(x) - double(x2)));
fprintf('  Round-trip error: %e\n', d);
if d < 2e-4
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: Small values
fprintf('Test 3 - Small values:\n');
x = int32([-1, 0, 1, 1000, -1000]);
y = arm_q31_to_float_mex(x);
expected = single(double(x) / 2147483648.0);
d = max(abs(y - expected));
if d < 1e-10
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