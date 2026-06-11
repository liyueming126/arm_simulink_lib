function arm_negate_q31()
clc;
fprintf('=== Testing arm_negate_q31 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int32([0, 100000, -100000, 2147483647, -2147483647, -2147483648, 1, -1]);
y = arm_negate_q31_mex(x);
fprintf('Test 1 - Basic vector:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
ref = int32([0, -100000, 100000, -2147483647, 2147483647, 2147483647, -1, 1]);
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: random large vector
rng(42);
n = 10000;
x = int32(randi([-2147483648, 2147483647], n, 1));
tic; y = arm_negate_q31_mex(x); t = toc;
fprintf('Test 2 - Random %d-element vector: %.3f ms\n', n, t*1000);
ref = x;
ref(ref == intmin('int32')) = intmax('int32');
ref(ref ~= intmax('int32')) = -ref(ref ~= intmax('int32'));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    mismatches = find(y ~= ref, 5, 'first');
    fprintf('  FAIL at indices: ['); fprintf('%d ', mismatches); fprintf(']\n');
    for k = mismatches(:)', fprintf('    x(%d)=%d, y=%d, expected=%d\n', k, x(k), y(k), ref(k)); end
    errors = errors + 1;
end

% Test 3: saturation edge case
x = int32([intmin('int32'), intmax('int32')]);
y = arm_negate_q31_mex(x);
fprintf('Test 3 - Saturation (0x80000000 -> 0x7FFFFFFF): Input: %d, Output: %d\n', x(1), y(1));
if y(1) == intmax('int32')
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end