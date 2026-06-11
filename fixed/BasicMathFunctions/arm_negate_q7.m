function arm_negate_q7()
clc;
fprintf('=== Testing arm_negate_q7 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int8([0, 64, -64, 127, -127, -128, 1, -1]);
y = arm_negate_q7_mex(x);
fprintf('Test 1 - Basic vector:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
ref = int8([0, -64, 64, -127, 127, 127, -1, 1]);
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: ['); fprintf('%d ', ref); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 2: random large vector
rng(42);
n = 10000;
x = int8(randi([-128, 127], n, 1, 'int8'));
tic; y = arm_negate_q7_mex(x); t = toc;
fprintf('Test 2 - Random %d-element vector: %.3f ms\n', n, t*1000);
% Reference: -128 -> 127 (saturate), all others: -val
ref = int8(zeros(size(x)));
for i = 1:numel(x)
    if x(i) == -128
        ref(i) = 127;
    else
        ref(i) = -x(i);
    end
end
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    mismatches = find(y ~= ref, 5, 'first');
    fprintf('  FAIL at indices: ['); fprintf('%d ', mismatches); fprintf(']\n');
    for k = mismatches(:)', fprintf('    x(%d)=%d, y=%d, expected=%d\n', k, x(k), y(k), ref(k)); end
    errors = errors + 1;
end

% Test 3: matrix input
x = int8([1 -2 3; -4 5 -6]);
y = arm_negate_q7_mex(x);
fprintf('Test 3 - 2x3 matrix:\n  Input:\n'); disp(x); fprintf('  Output:\n'); disp(y);
ref = int8([-1 2 -3; 4 -5 6]);
if isequal(y, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

% Test 4: saturation edge case
x = int8([intmin('int8'), intmax('int8')]);
y = arm_negate_q7_mex(x);
fprintf('Test 4 - Saturation (0x80 -> 0x7F): Input: %d, Output: %d\n', x(1), y(1));
if y(1) == intmax('int8')
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end