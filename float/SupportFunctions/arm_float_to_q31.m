function arm_float_to_q31()
clc;
fprintf('=== Testing arm_float_to_q31 ===\n\n');
errors = 0;

% Test 1: Basic conversion
fprintf('Test 1 - Basic conversion:\n');
x = single([0, 0.5, -0.5, 1, -1, 0.25, -0.25]);
y = arm_float_to_q31_mex(x);
% Reference: CMSIS does: (q31_t)(pSrc[n] * 2147483648)
ref = int32(round(double(x) * 2147483648));
if all(y == ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: Saturation
fprintf('Test 2 - Saturation:\n');
x = single([2.0, -2.0]);
y = arm_float_to_q31_mex(x);
if y(1) == int32(2147483647) && y(2) == int32(-2147483648)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL (got %d %d)\n\n', y(1), y(2));
    errors = errors + 1;
end

% Test 3: Values within range
fprintf('Test 3 - Random values:\n');
x = single(randn(1, 100) * 0.3);
y = arm_float_to_q31_mex(x);
ref = int32(round(double(x) * 2147483648));
if all(y == ref)
    fprintf('  PASS\n\n');
else
    d = sum(y ~= ref);
    fprintf('  %d mismatches out of 100\n\n', d);
    errors = errors + 1;
end

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end
end