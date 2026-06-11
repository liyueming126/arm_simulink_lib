function arm_dot_prod_q31()
clc;
fprintf('=== Testing arm_dot_prod_q31 ===\n\n');
errors = 0;

% Test 1: basic vectors
x = int32([1, 2, 3, 4, 5]);
y = int32([10, 20, 30, 40, 50]);
z = arm_dot_prod_q31_mex(x, y);
fprintf('Test 1 - Basic vectors:\n  x: ['); fprintf('%d ', x); fprintf(']\n  y: ['); fprintf('%d ', y); fprintf(']\n  result: %.0f\n', z);
ref = int64(0);
for i = 1:5, ref = ref + bitshift(int64(x(i)) * int64(y(i)), -14); end
if double(z) == double(ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %ld\n\n', ref);
    errors = errors + 1;
end

% Test 2: with negative values
x = int32([-1000, 2000, -3000, 4000]);
y = int32([500, -250, 750, -100]);
z = arm_dot_prod_q31_mex(x, y);
ref = int64(0);
for i = 1:4, ref = ref + bitshift(int64(x(i)) * int64(y(i)), -14); end
fprintf('Test 2 - Mixed signs: result=%.0f, expected=%ld\n', z, ref);
if double(z) == double(ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: saturation bounds
x = int32([2147483647, -2147483648, 2147483647, -2147483648]);
y = int32([2147483647, -2147483648, -2147483648, 2147483647]);
z = arm_dot_prod_q31_mex(x, y);
ref = bitshift(int64(x(1))*int64(y(1)) + int64(x(2))*int64(y(2)) + int64(x(3))*int64(y(3)) + int64(x(4))*int64(y(4)), -14);
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
x = int32(randi([-2147483648, 2147483647], n, 1));
y = int32(randi([-2147483648, 2147483647], n, 1));
tic; z = arm_dot_prod_q31_mex(x, y); t = toc;
fprintf('Test 4 - Random %d-element: %.3f ms\n', n, t*1000);
ref = int64(0);
for i = 1:n, ref = ref + bitshift(int64(x(i)) * int64(y(i)), -14); end
if double(z) == double(ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: %ld\n\n', ref);
    errors = errors + 1;
end

% Test 5: zeros
x = int32([1, 0, 2147483647, -1]);
y = int32([0, 1, 0, 0]);
z = arm_dot_prod_q31_mex(x, y);
fprintf('Test 5 - With zeros: result=%.0f\n', z);
ref = int64(0);
for i = 1:4, ref = ref + bitshift(int64(x(i)) * int64(y(i)), -14); end
if double(z) == double(ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end