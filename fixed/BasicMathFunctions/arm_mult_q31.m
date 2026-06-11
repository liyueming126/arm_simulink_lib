function arm_mult_q31()
clc;
fprintf('=== Testing arm_mult_q31 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int32([0, 2147483647, -2147483648, 1073741824, -1073741824, 1000, -1000]);
y = int32([0, 2147483647, 1, 1073741824, 1073741824, 1000, 1000]);
z = arm_mult_q31_mex(x, y);
fprintf('Test 1 - Basic vector:\n  x: ['); fprintf('%d ', x); fprintf(']\n  y: ['); fprintf('%d ', y); fprintf(']\n  z: ['); fprintf('%d ', z); fprintf(']\n');
% Use int64 reference to avoid double precision loss
ref = int32(zeros(size(z)));
for i = 1:length(x)
    p = int64(x(i)) * int64(y(i));
    p = bitshift(p, -31);
    if p > int64(2147483647), p = int64(2147483647);
    elseif p < int64(-2147483648), p = int64(-2147483648); end
    ref(i) = int32(p);
end
if isequal(z, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: saturation
x = int32([2147483647, 2147483647, -2147483648, 1518500250]);
y = int32([2147483647, 1073741824, -2147483648, 1518500250]);
z = arm_mult_q31_mex(x, y);
fprintf('Test 2 - Saturation:\n  x: ['); fprintf('%d ', x); fprintf(']\n  y: ['); fprintf('%d ', y); fprintf(']\n  z: ['); fprintf('%d ', z); fprintf(']\n');
ref = int32(zeros(size(z)));
for i = 1:length(x)
    p = int64(x(i)) * int64(y(i));
    p = bitshift(p, -31);
    if p > int64(2147483647), p = int64(2147483647);
    elseif p < int64(-2147483648), p = int64(-2147483648); end
    ref(i) = int32(p);
end
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
tic; z = arm_mult_q31_mex(x, y); t = toc;
fprintf('Test 3 - Random %d-element vector: %.3f ms\n', n, t*1000);
ref = int32(zeros(size(z)));
for i = 1:n
    p = int64(x(i)) * int64(y(i));
    p = bitshift(p, -31);
    if p > int64(2147483647), p = int64(2147483647);
    elseif p < int64(-2147483648), p = int64(-2147483648); end
    ref(i) = int32(p);
end
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