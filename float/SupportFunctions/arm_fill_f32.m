function arm_fill_f32()
clc;
fprintf('=== Testing arm_fill_f32 ===\n\n');
errors = 0;

% Test 1: Fill with constant
fprintf('Test 1 - Fill with 3.14:\n');
y = arm_fill_f32_mex(3.14, 10);
expected = single(ones(1,10) * 3.14);
d = max(abs(y - expected));
if d < 1e-6
    fprintf('  PASS (max diff = %e)\n\n', d);
else
    fprintf('  FAIL (max diff = %e)\n\n', d);
    errors = errors + 1;
end

% Test 2: Fill with negative
fprintf('Test 2 - Fill with -2.5:\n');
y = arm_fill_f32_mex(-2.5, 5);
expected = single(ones(1,5) * (-2.5));
d = max(abs(y - expected));
if d < 1e-6
    fprintf('  PASS (max diff = %e)\n\n', d);
else
    fprintf('  FAIL (max diff = %e)\n\n', d);
    errors = errors + 1;
end

% Test 3: Fill zero
fprintf('Test 3 - Fill with 0:\n');
y = arm_fill_f32_mex(0, 8);
expected = single(zeros(1,8));
d = max(abs(y - expected));
if d < 1e-6
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