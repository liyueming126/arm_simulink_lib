function arm_sin_f32()
clc;
fprintf('=== Testing arm_sin_f32 ===\n\n');
errors = 0;
tol = 1e-5;

% Test 1: sin(0) = 0
x = single(0.0);
y = arm_sin_f32_mex(x);
fprintf('Test 1 - sin(0):\n');
fprintf('  sin(0)=%.10f (expected 0.0)\n', y);
if abs(y) < tol
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: sin(pi/2) = 1
x = single(pi/2);
y = arm_sin_f32_mex(x);
fprintf('Test 2 - sin(pi/2):\n');
fprintf('  sin(pi/2)=%.10f (expected 1.0)\n', y);
if abs(y - 1.0) < tol
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: sin(pi) = 0
x = single(pi);
y = arm_sin_f32_mex(x);
fprintf('Test 3 - sin(pi):\n');
fprintf('  sin(pi)=%.10f (expected ~0)\n', y);
if abs(y) < tol
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: sin(3*pi/2) = -1
x = single(3*pi/2);
y = arm_sin_f32_mex(x);
fprintf('Test 4 - sin(3*pi/2):\n');
fprintf('  sin(3*pi/2)=%.10f (expected -1.0)\n', y);
if abs(y + 1.0) < tol
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 5: sin(pi/6) = 0.5
x = single(pi/6);
y = arm_sin_f32_mex(x);
expected = single(0.5);
fprintf('Test 5 - sin(pi/6):\n');
fprintf('  sin(pi/6)=%.10f (expected %.10f)\n', y, expected);
if abs(y - expected) < tol
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 6: various values
angles = single([0.1, 1.0, 2.0, 3.0, 4.0, 5.0, 10.0, 100.0]);
fprintf('Test 6 - Various values:\n');
maxErr = 0;
for i = 1:length(angles)
    y = arm_sin_f32_mex(angles(i));
    expected = sin(double(angles(i)));
    err = abs(double(y) - expected);
    if err > maxErr, maxErr = err; end
    fprintf('  sin(%.4f)=%.10f (expected %.10f) err=%.2e\n', angles(i), y, expected, err);
end
if maxErr < tol
    fprintf('  PASS (max err = %.2e)\n\n', maxErr);
else
    fprintf('  FAIL (max err = %.2e)\n\n', maxErr);
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end