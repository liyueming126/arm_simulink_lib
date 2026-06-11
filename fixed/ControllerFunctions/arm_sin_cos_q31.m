function arm_sin_cos_q31()
clc;
fprintf('=== Testing arm_sin_cos_q31 ===\n\n');
errors = 0;

POW2_31 = 2^31;

% Test 1: theta = 0 -> sin=0, cos=1
theta = int32(0);
result = arm_sin_cos_q31_mex(theta);
sinVal = result(1); cosVal = result(2);
fprintf('Test 1 - theta=0 (Q31: 0):\n');
fprintf('  sin=%d (float: %.6f), cos=%d (float: %.6f)\n', sinVal, double(sinVal)/POW2_31, cosVal, double(cosVal)/POW2_31);
if sinVal == 0 && abs(double(cosVal)/POW2_31 - 1.0) < 0.001
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n');
    fprintf('  sin: actual=%d (float=%.10f), expected~=0 (exact integer match)\n', sinVal, double(sinVal)/POW2_31);
    fprintf('  cos: actual=%d (float=%.10f), expected~=1.0 (tolerance 0.001)\n', cosVal, double(cosVal)/POW2_31);
    fprintf('\n');
    errors = errors + 1;
end

% Test 2: theta = 0x20000000 (~45 degrees)
theta = int32(0x20000000);
result = arm_sin_cos_q31_mex(theta);
sinVal = result(1); cosVal = result(2);
fprintf('Test 2 - theta ~= 45 deg (Q31: 0x20000000):\n');
fprintf('  sin=%.6f, cos=%.6f\n', double(sinVal)/POW2_31, double(cosVal)/POW2_31);
if abs(double(sinVal)/POW2_31 - 0.7071) < 0.01 && abs(double(cosVal)/POW2_31 - 0.7071) < 0.01
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n');
    fprintf('  sin: actual=%.10f, expected~=0.7071 (diff=%.6f, tol=0.01)\n', double(sinVal)/POW2_31, abs(double(sinVal)/POW2_31 - 0.7071));
    fprintf('  cos: actual=%.10f, expected~=0.7071 (diff=%.6f, tol=0.01)\n', double(cosVal)/POW2_31, abs(double(cosVal)/POW2_31 - 0.7071));
    fprintf('\n');
    errors = errors + 1;
end

% Test 3: theta = 0x40000000 (~90 degrees)
theta = int32(0x40000000);
result = arm_sin_cos_q31_mex(theta);
sinVal = result(1); cosVal = result(2);
fprintf('Test 3 - theta ~= 90 deg (Q31: 0x40000000):\n');
fprintf('  sin=%.6f, cos=%.6f\n', double(sinVal)/POW2_31, double(cosVal)/POW2_31);
if abs(double(sinVal)/POW2_31 - 1.0) < 0.01 && abs(double(cosVal)/POW2_31) < 0.01
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n');
    fprintf('  sin: actual=%.10f, expected~=1.0 (diff=%.6f, tol=0.01)\n', double(sinVal)/POW2_31, abs(double(sinVal)/POW2_31 - 1.0));
    fprintf('  cos: actual=%.10f, expected~=0.0 (diff=%.6f, tol=0.01)\n', double(cosVal)/POW2_31, abs(double(cosVal)/POW2_31));
    fprintf('\n');
    errors = errors + 1;
end

% Test 4: theta = 0x60000000 (~135 degrees)
theta = int32(0x60000000);
result = arm_sin_cos_q31_mex(theta);
sinVal = result(1); cosVal = result(2);
fprintf('Test 4 - theta ~= 135 deg (Q31: 0x60000000):\n');
fprintf('  sin=%.6f, cos=%.6f\n', double(sinVal)/POW2_31, double(cosVal)/POW2_31);
if abs(double(sinVal)/POW2_31 - 0.7071) < 0.01 && abs(double(cosVal)/POW2_31 + 0.7071) < 0.01
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n');
    fprintf('  sin: actual=%.10f, expected~=0.7071 (diff=%.6f, tol=0.01)\n', double(sinVal)/POW2_31, abs(double(sinVal)/POW2_31 - 0.7071));
    fprintf('  cos: actual=%.10f, expected~=-0.7071 (diff=%.6f, tol=0.01)\n', double(cosVal)/POW2_31, abs(double(cosVal)/POW2_31 + 0.7071));
    fprintf('\n');
    errors = errors + 1;
end

% Test 5: theta = 0x80000000 (~180 degrees)
theta = int32(intmin('int32'));
result = arm_sin_cos_q31_mex(theta);
sinVal = result(1); cosVal = result(2);
fprintf('Test 5 - theta ~= -180 deg (Q31: intmin):\n');
fprintf('  sin=%.6f, cos=%.6f\n', double(sinVal)/POW2_31, double(cosVal)/POW2_31);
if abs(double(sinVal)/POW2_31) < 0.01 && abs(double(cosVal)/POW2_31 + 1.0) < 0.01
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n');
    fprintf('  sin: actual=%.10f, expected~=0.0 (diff=%.6f, tol=0.01)\n', double(sinVal)/POW2_31, abs(double(sinVal)/POW2_31));
    fprintf('  cos: actual=%.10f, expected~=-1.0 (diff=%.6f, tol=0.01)\n', double(cosVal)/POW2_31, abs(double(cosVal)/POW2_31 + 1.0));
    fprintf('\n');
    errors = errors + 1;
end

% Test 6: negative theta (-45 degrees, 0xE0000000)
theta = typecast(uint32(0xE0000000), 'int32');
result = arm_sin_cos_q31_mex(theta);
sinVal = result(1); cosVal = result(2);
fprintf('Test 6 - theta ~= -45 deg (Q31: 0xE0000000):\n');
fprintf('  sin=%.6f, cos=%.6f\n', double(sinVal)/POW2_31, double(cosVal)/POW2_31);
if abs(double(sinVal)/POW2_31 + 0.7071) < 0.01 && abs(double(cosVal)/POW2_31 - 0.7071) < 0.01
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n');
    fprintf('  sin: actual=%.10f, expected~=-0.7071 (diff=%.6f, tol=0.01)\n', double(sinVal)/POW2_31, abs(double(sinVal)/POW2_31 + 0.7071));
    fprintf('  cos: actual=%.10f, expected~=0.7071 (diff=%.6f, tol=0.01)\n', double(cosVal)/POW2_31, abs(double(cosVal)/POW2_31 - 0.7071));
    fprintf('\n');
    errors = errors + 1;
end

% Test 7: identity sin^2 + cos^2 = 1 for random points
fprintf('Test 7 - sin^2+cos^2=1 for 100 random points:\n');
rng(123);
maxErr = 0;
worstTheta = int32(0);
for i = 1:100
    theta = int32(randi([-2^31, 2^31-1], 'int32'));
    result = arm_sin_cos_q31_mex(theta);
    s = double(result(1)) / POW2_31;
    c = double(result(2)) / POW2_31;
    err = abs(s*s + c*c - 1.0);
    if err > maxErr, maxErr = err; worstTheta = theta; end
end
fprintf('  max error = %.6f (worst theta = 0x%08x)\n', maxErr, worstTheta);
if maxErr < 0.2
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n');
    fprintf('  Max sin^2+cos^2 error: %.10f (tolerance 0.20)\n', maxErr);
    fprintf('\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end