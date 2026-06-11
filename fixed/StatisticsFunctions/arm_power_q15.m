function arm_power_q15()
clc;
fprintf('=== Testing arm_power_q15 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int16([1, 2, 3, 4, 5]);
z = arm_power_q15_mex(x);
fprintf('Test 1 - Basic: x=[1 2 3 4 5], result=%.0f\n', z);
ref = sum(int32(x).^2);
if abs(z - double(ref)) < 1
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %d\n\n', ref);
    errors = errors + 1;
end

% Test 2: saturation bounds
x = int16([32767, -32768, 32767]);
z = arm_power_q15_mex(x);
ref = int64(32767)^2 + int64(-32768)^2 + int64(32767)^2;
fprintf('Test 2 - Bounds: result=%.0f, expected=%ld\n', z, ref);
if abs(z - double(ref)) < 1
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: random large vector
rng(42);
n = 1000;
x = int16(randi([-32768, 32767], n, 1, 'int16'));
tic; z = arm_power_q15_mex(x); t = toc;
fprintf('Test 3 - Random %d-element: %.3f ms\n', n, t*1000);
ref = int64(0);
for i = 1:n, ref = ref + int64(int32(x(i)))^2; end
if abs(z - double(ref)) < 1
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: zeros
x = int16(zeros(1,100));
z = arm_power_q15_mex(x);
fprintf('Test 4 - Zeros: result=%.0f\n', z);
if z == 0
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end