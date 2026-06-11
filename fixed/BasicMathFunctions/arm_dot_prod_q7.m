function arm_dot_prod_q7()
clc;
fprintf('=== Testing arm_dot_prod_q7 ===\n\n');
errors = 0;

% Test 1: basic vectors
x = int8([1, 2, 3, 4, 5]);
y = int8([10, 20, 30, 40, 50]);
z = arm_dot_prod_q7_mex(x, y);
fprintf('Test 1 - Basic vectors:\n  x: ['); fprintf('%d ', x); fprintf(']\n  y: ['); fprintf('%d ', y); fprintf(']\n  result: %.0f\n', z);
ref = int32(sum(int16(x) .* int16(y)));
if double(z) == double(ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %d\n\n', ref);
    errors = errors + 1;
end

% Test 2: with negative values
x = int8([-10, 20, -30, 40]);
y = int8([5, -2, 7, -1]);
z = arm_dot_prod_q7_mex(x, y);
ref = int32(sum(int16(x) .* int16(y)));
fprintf('Test 2 - Mixed signs: result=%.0f, expected=%d\n', z, ref);
if double(z) == double(ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: saturation bounds
x = int8([127, -128, 127, -128]);
y = int8([127, -128, -128, 127]);
z = arm_dot_prod_q7_mex(x, y);
ref = int64(int16(x(1))*int16(y(1)) + int16(x(2))*int16(y(2)) + int16(x(3))*int16(y(3)) + int16(x(4))*int16(y(4)));
fprintf('Test 3 - Bounds: result=%.0f\n', z);
if double(z) == double(ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %ld\n\n', ref);
    errors = errors + 1;
end

% Test 4: random large vector
rng(42);
n = 1000;
x = int8(randi([-128, 127], n, 1, 'int8'));
y = int8(randi([-128, 127], n, 1, 'int8'));
tic; z = arm_dot_prod_q7_mex(x, y); t = toc;
fprintf('Test 4 - Random %d-element: %.3f ms\n', n, t*1000);
ref = int64(0);
for i = 1:n, ref = ref + int64(int16(x(i)) * int16(y(i))); end
if double(z) == double(ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %ld\n\n', ref);
    errors = errors + 1;
end

% Test 5: zeros
x = int8([1, 0, 127, -1]);
y = int8([0, 1, 0, 0]);
z = arm_dot_prod_q7_mex(x, y);
fprintf('Test 5 - With zeros: result=%.0f\n', z);
ref = int64(0);
for i = 1:4, ref = ref + int64(int16(x(i)) * int16(y(i))); end
if double(z) == double(ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end