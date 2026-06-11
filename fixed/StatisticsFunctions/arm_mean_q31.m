function arm_mean_q31()
clc;
fprintf('=== Testing arm_mean_q31 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int32([0, 1000, -1000, 500000, -500000]);
z = arm_mean_q31_mex(x);
fprintf('Test 1 - Basic: z=%d\n', z);
ref = int32(idivide(int64(sum(int64(x))), int64(length(x)), 'fix'));
if z == ref
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %d\n\n', ref);
    errors = errors + 1;
end

% Test 2: constant
x = int32(ones(1,100) * 1000);
z = arm_mean_q31_mex(x);
fprintf('Test 2 - Constant 1000: z=%d\n', z);
if z == int32(1000)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: saturation bounds
x = int32([2147483647, 2147483647, -2147483648, -2147483648]);
z = arm_mean_q31_mex(x);
fprintf('Test 3 - Bounds: z=%d\n', z);
ref = int32(idivide(int64(sum(int64(x))), int64(length(x)), 'fix'));
if z == ref
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %d\n\n', ref);
    errors = errors + 1;
end

% Test 4: random vector
rng(42);
n = 1000;
x = int32(randi([-2147483648, 2147483647], n, 1, 'int32'));
tic; z = arm_mean_q31_mex(x); t = toc;
fprintf('Test 4 - Random %d-element: %.3f ms\n', n, t*1000);
ref = int32(idivide(int64(sum(int64(x))), int64(n), 'fix'));
if z == ref
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %d\n\n', ref);
    errors = errors + 1;
end

% Test 5: zeros
x = int32(zeros(1,10));
z = arm_mean_q31_mex(x);
fprintf('Test 5 - All zeros: z=%d\n', z);
if z == int32(0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end