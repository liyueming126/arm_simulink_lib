function arm_power_q7()
clc;
fprintf('=== Testing arm_power_q7 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int8([1, 2, 3, 4, 5]);
z = arm_power_q7_mex(x);
fprintf('Test 1 - Basic: x=[1 2 3 4 5], result=%d\n', z);
ref = int32(sum(double(x).^2));
if z == ref
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %d\n\n', ref);
    errors = errors + 1;
end

% Test 2: saturation bounds
x = int8([127, -128, 127]);
z = arm_power_q7_mex(x);
ref = int32(127)^2 + int32(-128)^2 + int32(127)^2;
fprintf('Test 2 - Bounds: result=%d, expected=%d\n', z, ref);
if z == ref
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: random large vector
rng(42);
n = 1000;
x = int8(randi([-128, 127], n, 1, 'int8'));
tic; z = arm_power_q7_mex(x); t = toc;
fprintf('Test 3 - Random %d-element: %.3f ms\n', n, t*1000);
ref = int32(0);
for i = 1:n, ref = ref + int32(int32(x(i)))^2; end
if z == ref
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: zeros
x = int8(zeros(1,100));
z = arm_power_q7_mex(x);
fprintf('Test 4 - Zeros: result=%d\n', z);
if z == 0
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end