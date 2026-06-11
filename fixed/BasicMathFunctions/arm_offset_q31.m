function arm_offset_q31()
clc;
fprintf('=== Testing arm_offset_q31 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int32([0, 100000, -100000, 2147483647, -2147483648]);
offset = int32(50000);
y = arm_offset_q31_mex(x, offset);
fprintf('Test 1 - offset=%d:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
ref = int32(max(min(double(x) + double(offset), 2147483647.0), -2147483648.0));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    mismatches = find(y ~= ref, 5, 'first');
    fprintf('  FAIL\n');
    for k = mismatches(:)', fprintf('    x(%d)=%d, y=%d, expected=%d\n', k, x(k), y(k), ref(k)); end
    fprintf('\n');
    errors = errors + 1;
end

% Test 2: saturation
x = int32([2147483647, -2147483648, 2147483646, -2147483647]);
offset = int32(10);
y = arm_offset_q31_mex(x, offset);
fprintf('Test 2 - offset=%d (saturation):\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
ref = int32(max(min(double(x) + double(offset), 2147483647.0), -2147483648.0));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    mismatches = find(y ~= ref, 5, 'first');
    fprintf('  FAIL\n');
    for k = mismatches(:)', fprintf('    x(%d)=%d, y=%d, expected=%d\n', k, x(k), y(k), ref(k)); end
    fprintf('\n');
    errors = errors + 1;
end

% Test 3: random large vector
rng(42);
n = 10000;
x = int32(randi([-2147483648, 2147483647], n, 1));
offset = int32(randi([-1000000, 1000000]));
tic; y = arm_offset_q31_mex(x, offset); t = toc;
fprintf('Test 3 - Random %d-element, offset=%d: %.3f ms\n', n, offset, t*1000);
s = double(x) + double(offset);
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