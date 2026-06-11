function arm_shift_q31()
clc;
fprintf('=== Testing arm_shift_q31 ===\n\n');
errors = 0;

% Test 1: left shift by 2
x = int32([0, 100000, -100000, 1000000000, -1000000000, 2147483647, -2147483648]);
y = arm_shift_q31_mex(x, int8(2));
fprintf('Test 1 - Left shift by 2:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
s = double(x) * (2^2);
ref = int32(max(min(s, 2147483647.0), -2147483648.0));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: right shift by 2
y = arm_shift_q31_mex(x, int8(-2));
fprintf('Test 2 - Right shift by 2:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
ref = bitshift(x, -2);
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: zero shift
y = arm_shift_q31_mex(x, int8(0));
fprintf('Test 3 - Zero shift:\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
if isequal(y, x)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: random large vector
rng(42);
n = 10000;
x = int32(randi([-2147483648, 2147483647], n, 1));
tic; y = arm_shift_q31_mex(x, int8(3)); t = toc;
fprintf('Test 4 - Random %d-element, left shift 3: %.3f ms\n', n, t*1000);
s = double(x) * (2^3);
ref = int32(max(min(s, 2147483647.0), -2147483648.0));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    mismatches = find(y ~= ref, 5, 'first');
    fprintf('  FAIL at indices: ['); fprintf('%d ', mismatches); fprintf(']\n');
    for k = mismatches(:)', fprintf('    x(%d)=%d, y=%d, expected=%d\n', k, x(k), y(k), ref(k)); end
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end