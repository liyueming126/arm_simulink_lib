function arm_mean_q7()
clc;
fprintf('=== Testing arm_mean_q7 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int8([0, 10, -10, 50, -50]);
z = arm_mean_q7_mex(x);
fprintf('Test 1 - Basic: z=%d\n', z);
ref = int8(idivide(sum(int32(x)), int32(length(x)), 'fix'));
if z == ref
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %d\n\n', ref);
    errors = errors + 1;
end

% Test 2: constant
x = int8(ones(1,100) * 10);
z = arm_mean_q7_mex(x);
fprintf('Test 2 - Constant 10: z=%d\n', z);
if z == int8(10)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: saturation bounds
x = int8([127, 127, -128, -128]);
z = arm_mean_q7_mex(x);
fprintf('Test 3 - Bounds: z=%d\n', z);
ref = int8(idivide(sum(int32(x)), int32(length(x)), 'fix'));
if z == ref
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %d\n\n', ref);
    errors = errors + 1;
end

% Test 4: random vector
rng(42);
n = 1000;
x = int8(randi([-128, 127], n, 1, 'int8'));
tic; z = arm_mean_q7_mex(x); t = toc;
fprintf('Test 4 - Random %d-element: %.3f ms\n', n, t*1000);
ref = int8(idivide(sum(int32(x)), int32(n), 'fix'));
if z == ref
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %d\n\n', ref);
    errors = errors + 1;
end

% Test 5: zeros
x = int8(zeros(1,10));
z = arm_mean_q7_mex(x);
fprintf('Test 5 - All zeros: z=%d\n', z);
if z == int8(0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end