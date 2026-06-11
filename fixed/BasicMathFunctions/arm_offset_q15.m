function arm_offset_q15()
clc;
fprintf('=== Testing arm_offset_q15 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int16([0, 100, -100, 32767, -32768]);
offset = int16(500);
y = arm_offset_q15_mex(x, offset);
fprintf('Test 1 - offset=%d:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
s = double(x) + double(offset);
ref = int16(max(min(s, 32767), -32768));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n  Expected: ['); fprintf('%d ', ref); fprintf(']\n  Got:      ['); fprintf('%d ', y); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 2: negative offset
offset = int16(-500);
y = arm_offset_q15_mex(x, offset);
fprintf('Test 2 - offset=%d:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
s = double(x) + double(offset);
ref = int16(max(min(s, 32767), -32768));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n  Expected: ['); fprintf('%d ', ref); fprintf(']\n  Got:      ['); fprintf('%d ', y); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 3: saturation
x = int16([32767, -32768, 30000, -30000]);
offset = int16(1000);
y = arm_offset_q15_mex(x, offset);
fprintf('Test 3 - offset=%d (saturation):\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
s = double(x) + double(offset);
ref = int16(max(min(s, 32767), -32768));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n  Expected: ['); fprintf('%d ', ref); fprintf(']\n  Got:      ['); fprintf('%d ', y); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 4: random large vector
rng(42);
n = 10000;
x = int16(randi([-32768, 32767], n, 1, 'int16'));
offset = int16(randi([-32768, 32767]));
tic; y = arm_offset_q15_mex(x, offset); t = toc;
fprintf('Test 4 - Random %d-element, offset=%d: %.3f ms\n', n, offset, t*1000);
s = double(x) + double(offset);
ref = int16(max(min(s, 32767), -32768));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    mismatches = find(y ~= ref, 5, 'first');
    fprintf('  FAIL at indices: ['); fprintf('%d ', mismatches); fprintf(']\n');
    for k = mismatches(:)', fprintf('    x(%d)=%d, z=%d, expected=%d\n', k, x(k), y(k), ref(k)); end
    errors = errors + 1;
end

% Test 5: matrix input
x = int16([1 2; 3 4]);
offset = int16(100);
y = arm_offset_q15_mex(x, offset);
fprintf('Test 5 - Matrix:\n  x:\n'); disp(x); fprintf('  y:\n'); disp(y);
s = double(x(:)) + double(offset);
ref = int16(max(min(s, 32767), -32768));
ref = reshape(ref, size(x));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n  Expected:\n'); disp(ref); fprintf('  Got:\n'); disp(y);
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end