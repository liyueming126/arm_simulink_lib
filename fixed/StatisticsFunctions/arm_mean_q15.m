function arm_mean_q15()
clc;
fprintf('=== Testing arm_mean_q15 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int16([0, 100, -100, 500, -500]);
z = arm_mean_q15_mex(x);
fprintf('Test 1 - Basic: z=%d\n', z);
ref = int16(idivide(sum(int32(x)), int32(length(x)), 'fix'));
if z == ref
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %d\n\n', ref);
    errors = errors + 1;
end

% Test 2: constant
x = int16(ones(1,100) * 100);
z = arm_mean_q15_mex(x);
fprintf('Test 2 - Constant 100: z=%d\n', z);
if z == int16(100)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: saturation bounds
x = int16([32767, 32767, -32768, -32768]);
z = arm_mean_q15_mex(x);
fprintf('Test 3 - Bounds: z=%d\n', z);
ref = int16(idivide(sum(int32(x)), int32(length(x)), 'fix'));
if z == ref
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %d\n\n', ref);
    errors = errors + 1;
end

% Test 4: random vector
rng(42);
n = 1000;
x = int16(randi([-32768, 32767], n, 1, 'int16'));
tic; z = arm_mean_q15_mex(x); t = toc;
fprintf('Test 4 - Random %d-element: %.3f ms\n', n, t*1000);
ref = int16(idivide(sum(int32(x)), int32(n), 'fix'));
if z == ref
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %d\n\n', ref);
    errors = errors + 1;
end

% Test 5: zeros
x = int16(zeros(1,10));
z = arm_mean_q15_mex(x);
fprintf('Test 5 - All zeros: z=%d\n', z);
if z == int16(0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end