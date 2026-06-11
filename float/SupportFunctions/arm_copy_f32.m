function arm_copy_f32()
clc;
fprintf('=== Testing arm_copy_f32 ===\n\n');
errors = 0;

% Test 1: Basic copy
fprintf('Test 1 - Basic copy:\n');
x = single(randn(1, 10));
y = arm_copy_f32_mex(x);
d = max(abs(x - y));
if d < 1e-6
    fprintf('  PASS (max diff = %e)\n\n', d);
else
    fprintf('  FAIL (max diff = %e)\n\n', d);
    errors = errors + 1;
end

% Test 2: Large vector
fprintf('Test 2 - Large vector (10000 elements):\n');
x = single(linspace(-100, 100, 10000));
y = arm_copy_f32_mex(x);
d = max(abs(x - y));
if d < 1e-6
    fprintf('  PASS (max diff = %e)\n\n', d);
else
    fprintf('  FAIL (max diff = %e)\n\n', d);
    errors = errors + 1;
end

% Test 3: Single element
fprintf('Test 3 - Single element:\n');
x = single(3.14159);
y = arm_copy_f32_mex(x);
d = abs(x - y);
if d < 1e-6
    fprintf('  PASS (diff = %e)\n\n', d);
else
    fprintf('  FAIL (diff = %e)\n\n', d);
    errors = errors + 1;
end

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end
end