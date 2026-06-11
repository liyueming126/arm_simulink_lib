function arm_scale_q7()
clc;
fprintf('=== Testing arm_scale_q7 ===\n\n');
errors = 0;

% Test 1: unity scale, shift=0
x = int8([0, 127, -128, 64, -64, 10, -10]);
y = arm_scale_q7_mex(x, int8(127), int8(0));
fprintf('Test 1 - scaleFract=127, shift=0:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
% kShift = 7 - 0 = 7. For each element: (elem * 127) >> 7 (arithmetic shift)
kShift = 7 - 0;
s = idivide(int16(x) * int16(127), int16(2^kShift), 'floor');
ref = int8(max(min(s, 127), -128));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n  expected: ['); fprintf('%d ', ref); fprintf(']\n  actual:   ['); fprintf('%d ', y); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 2: scale=0.5, shift=0 (kShift=7, scaleFract=64)
y = arm_scale_q7_mex(x, int8(64), int8(0));
fprintf('Test 2 - scaleFract=64, shift=0:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
kShift = 7 - 0;
s = idivide(int16(x) * int16(64), int16(2^kShift), 'floor');
ref = int8(max(min(s, 127), -128));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n  expected: ['); fprintf('%d ', ref); fprintf(']\n  actual:   ['); fprintf('%d ', y); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 3: scale=2.0, shift=6 (kShift=1)
y = arm_scale_q7_mex(x, int8(64), int8(6));
fprintf('Test 3 - scaleFract=64, shift=6:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
kShift = 7 - 6;
s = idivide(int16(x) * int16(64), int16(2^kShift), 'floor');
ref = int8(max(min(s, 127), -128));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n  expected: ['); fprintf('%d ', ref); fprintf(']\n  actual:   ['); fprintf('%d ', y); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 4: random large vector
rng(42);
n = 10000;
x = int8(randi([-128, 127], n, 1, 'int8'));
tic; y = arm_scale_q7_mex(x, int8(64), int8(2)); t = toc;
fprintf('Test 4 - Random %d-element: %.3f ms\n', n, t*1000);
% kShift = 7 - 2 = 5. result = fix(int16(x) * 64 / 2^5) with saturation
kShift = 7 - 2;
s = idivide(int16(x) * int16(64), int16(2^kShift), 'floor');
ref = int8(max(min(s, 127), -128));
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