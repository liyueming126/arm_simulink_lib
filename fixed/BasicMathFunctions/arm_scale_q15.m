function arm_scale_q15()
clc;
fprintf('=== Testing arm_scale_q15 ===\n\n');
errors = 0;

% Test 1: unity scale, shift=0
x = int16([0, 32767, -32768, 16384, -16384, 1000, -1000]);
y = arm_scale_q15_mex(x, int16(32767), int8(0));
fprintf('Test 1 - scale=0.99997, shift=0:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
% kShift = 15 - 0 = 15. For each element: (elem * 32767) >> 15 (arithmetic shift = floor division)
kShift = 15 - 0;
s = idivide(int32(x) * int32(32767), int32(2^kShift), 'floor');
ref = int16(max(min(s, 32767), -32768));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n  expected: ['); fprintf('%d ', ref); fprintf(']\n  actual:   ['); fprintf('%d ', y); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 2: scale=0.5, shift=0
y = arm_scale_q15_mex(x, int16(16384), int8(0));
fprintf('Test 2 - scale=0.5, shift=0:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
kShift = 15 - 0;
s = idivide(int32(x) * int32(16384), int32(2^kShift), 'floor');
ref = int16(max(min(s, 32767), -32768));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n  expected: ['); fprintf('%d ', ref); fprintf(']\n  actual:   ['); fprintf('%d ', y); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 3: scale=2.0, shift=14
y = arm_scale_q15_mex(x, int16(16384), int8(14));
fprintf('Test 3 - scale=2.0, shift=14:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
kShift = 15 - 14;
s = idivide(int32(x) * int32(16384), int32(2^kShift), 'floor');
ref = int16(max(min(s, 32767), -32768));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n  expected: ['); fprintf('%d ', ref); fprintf(']\n  actual:   ['); fprintf('%d ', y); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 4: random large vector
rng(42);
n = 10000;
x = int16(randi([-32768, 32767], n, 1, 'int16'));
tic; y = arm_scale_q15_mex(x, int16(16384), int8(2)); t = toc;
fprintf('Test 4 - Random %d-element: %.3f ms\n', n, t*1000);
% kShift = 15 - 2 = 13. result = floor((double(x) * 16384) / 2^13) with saturation
kShift = 15 - 2;
s = idivide(int32(x) * int32(16384), int32(2^kShift), 'floor');
ref = int16(max(min(s, 32767), -32768));
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