function arm_sub_q31()
clc;
fprintf('=== Testing arm_sub_q31 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int32([0, 100000, -100000, 2147483647, -2147483648, 1, -1]);
y = int32([0, 50000, 50000, 1, -1, 2147483647, 2147483647]);
z = arm_sub_q31_mex(x, y);
fprintf('Test 1 - Basic vector:\n  x: ['); fprintf('%d ', x); fprintf(']\n  y: ['); fprintf('%d ', y); fprintf(']\n  z: ['); fprintf('%d ', z); fprintf(']\n');
ref = int32([0, 50000, -150000, 2147483646, -2147483647, -2147483646, -2147483648]);
if isequal(z, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: saturation
x = int32([2147483647, -2147483648, 2000000000, -2000000000]);
y = int32([-1, 1, -2000000000, 2000000000]);
z = arm_sub_q31_mex(x, y);
fprintf('Test 2 - Saturation:\n  x: ['); fprintf('%d ', x); fprintf(']\n  y: ['); fprintf('%d ', y); fprintf(']\n  z: ['); fprintf('%d ', z); fprintf(']\n');
ref = int32([2147483647, -2147483648, 2147483647, -2147483648]);
if isequal(z, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: random large vector
rng(42);
n = 10000;
x = int32(randi([-2147483648, 2147483647], n, 1));
y = int32(randi([-2147483648, 2147483647], n, 1));
tic; z = arm_sub_q31_mex(x, y); t = toc;
fprintf('Test 3 - Random %d-element vector: %.3f ms\n', n, t*1000);
s = double(x) - double(y);
ref = int32(max(min(s, 2147483647.0), -2147483648.0));
if isequal(z, ref)
    fprintf('  PASS\n\n');
else
    mismatches = find(z ~= ref, 5, 'first');
    fprintf('  FAIL at indices: ['); fprintf('%d ', mismatches); fprintf(']\n');
    for k = mismatches(:)', fprintf('    x(%d)=%d, y(%d)=%d, z=%d, expected=%d\n', k, x(k), k, y(k), z(k), ref(k)); end
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end