function arm_power_q31()
clc;
fprintf('=== Testing arm_power_q31 ===\n\n');
errors = 0;

% Test 1: small values
x = int32([1, 2, 3, 4, 5]);
z = arm_power_q31_mex(x);
fprintf('Test 1 - Small values: x=[1 2 3 4 5], result=%.0f\n', z);
ref = int64(0);
for i = 1:5, ref = ref + (int64(x(i)) * int64(x(i))) / int64(16384); end
if abs(z - double(ref)) <= 5
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %ld\n\n', ref);
    errors = errors + 1;
end

% Test 2: saturation values
x = int32([100000, 200000, -100000]);
z = arm_power_q31_mex(x);
ref = int64(0);
for i = 1:3, ref = ref + (int64(x(i)) * int64(x(i))) / int64(16384); end
fprintf('Test 2 - Medium values: result=%.0f\n', z);
if abs(z - double(ref)) <= 5
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %ld\n\n', ref);
    errors = errors + 1;
end

% Test 3: random vector
rng(42);
n = 1000;
x = int32(randi([-500000, 500000], n, 1, 'int32'));
tic; z = arm_power_q31_mex(x); t = toc;
fprintf('Test 3 - Random %d-element: %.3f ms\n', n, t*1000);
ref = int64(0);
for i = 1:n, ref = ref + (int64(x(i)) * int64(x(i))) / int64(16384); end
if abs(z - double(ref)) <= n
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: zeros
x = int32(zeros(1,100));
z = arm_power_q31_mex(x);
fprintf('Test 4 - Zeros: result=%.0f\n', z);
if z == 0
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end