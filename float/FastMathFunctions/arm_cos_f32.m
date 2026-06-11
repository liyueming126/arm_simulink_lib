function arm_cos_f32()
clc;
fprintf('=== Testing arm_cos_f32 ===\n\n');
errors = 0;
tol = 1e-5;

% Test 1: cos(0) = 1
x = single(0.0);
y = arm_cos_f32_mex(x);
fprintf('Test 1 - cos(0):\n');
fprintf('  cos(0)=%.10f (expected 1.0)\n', y);
if abs(y - 1.0) < tol
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: cos(pi/2) = 0
x = single(pi/2);
y = arm_cos_f32_mex(x);
fprintf('Test 2 - cos(pi/2):\n');
fprintf('  cos(pi/2)=%.10f (expected ~0)\n', y);
if abs(y) < tol
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: cos(pi) = -1
x = single(pi);
y = arm_cos_f32_mex(x);
fprintf('Test 3 - cos(pi):\n');
fprintf('  cos(pi)=%.10f (expected -1.0)\n', y);
if abs(y + 1.0) < tol
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: cos(2*pi) = 1
x = single(2*pi);
y = arm_cos_f32_mex(x);
fprintf('Test 4 - cos(2*pi):\n');
fprintf('  cos(2*pi)=%.10f (expected 1.0)\n', y);
if abs(y - 1.0) < tol
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 5: cos(-pi/3) = 0.5
x = single(-pi/3);
y = arm_cos_f32_mex(x);
expected = single(0.5);
fprintf('Test 5 - cos(-pi/3):\n');
fprintf('  cos(-pi/3)=%.10f (expected %.10f)\n', y, expected);
if abs(y - expected) < tol
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 6: random values
angles = single([0.1, 1.0, 2.0, 3.0, 4.0, 5.0, 10.0, 100.0]);
fprintf('Test 6 - Various values:\n');
maxErr = 0;
for i = 1:length(angles)
    y = arm_cos_f32_mex(angles(i));
    expected = cos(double(angles(i)));
    err = abs(double(y) - expected);
    if err > maxErr, maxErr = err; end
    fprintf('  cos(%.4f)=%.10f (expected %.10f) err=%.2e\n', angles(i), y, expected, err);
end
if maxErr < tol
    fprintf('  PASS (max err = %.2e)\n\n', maxErr);
else
    fprintf('  FAIL (max err = %.2e)\n\n', maxErr);
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end