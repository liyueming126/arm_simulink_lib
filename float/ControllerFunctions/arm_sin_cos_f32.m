function arm_sin_cos_f32()
clc;
fprintf('=== Testing arm_sin_cos_f32 ===\n\n');
errors = 0;
tol = 1e-5;

% Test 1: theta = 0 -> sin=0, cos=1
theta = single(0.0);
res = arm_sin_cos_f32_mex(theta);
sinVal = res(1); cosVal = res(2);
fprintf('Test 1 - theta=0 deg:\n');
fprintf('  sin=%.10f, cos=%.10f\n', sinVal, cosVal);
if abs(sinVal) < tol && abs(cosVal - 1.0) < tol
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: theta = 90 deg -> sin=1, cos=0
theta = single(90.0);
res = arm_sin_cos_f32_mex(theta);
sinVal = res(1); cosVal = res(2);
fprintf('Test 2 - theta=90 deg:\n');
fprintf('  sin=%.10f, cos=%.10f\n', sinVal, cosVal);
if abs(sinVal - 1.0) < tol && abs(cosVal) < tol
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: theta = 180 deg -> sin=0, cos=-1
theta = single(180.0);
res = arm_sin_cos_f32_mex(theta);
sinVal = res(1); cosVal = res(2);
fprintf('Test 3 - theta=180 deg:\n');
fprintf('  sin=%.10f, cos=%.10f\n', sinVal, cosVal);
if abs(sinVal) < tol && abs(cosVal + 1.0) < tol
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: theta = 45 deg -> sin=cos=sqrt(2)/2 ~= 0.70710678
theta = single(45.0);
res = arm_sin_cos_f32_mex(theta);
sinVal = res(1); cosVal = res(2);
expected = single(sqrt(2.0)/2.0);
fprintf('Test 4 - theta=45 deg:\n');
fprintf('  sin=%.10f (expected %.10f), cos=%.10f (expected %.10f)\n', sinVal, expected, cosVal, expected);
if abs(sinVal - expected) < tol && abs(cosVal - expected) < tol
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 5: identity sin^2+cos^2=1 for various angles
angles = single([0, 30, 60, 120, 150, 210, 270, 330, 360]);
fprintf('Test 5 - sin^2+cos^2=1 identity:\n');
maxErr = 0;
for i = 1:length(angles)
    res = arm_sin_cos_f32_mex(angles(i));
    s = res(1); c = res(2);
    err = abs(s*s + c*c - 1.0);
    if err > maxErr, maxErr = err; end
    fprintf('  theta=%.1f: sin=%.6f, cos=%.6f, err=%.2e\n', angles(i), s, c, err);
end
if maxErr < tol
    fprintf('  PASS (max err = %.2e)\n\n', maxErr);
else
    fprintf('  FAIL (max err = %.2e)\n\n', maxErr);
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end