function arm_offset_q7()
clc;
fprintf('=== Testing arm_offset_q7 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int8([0, 64, -64, 127, -128]);
offset = int8(50);
y = arm_offset_q7_mex(x, offset);
fprintf('Test 1 - offset=%d:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
ref = int8([50, 114, -14, 127, -78]);
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: ['); fprintf('%d ', ref); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 2: negative offset
offset = int8(-50);
y = arm_offset_q7_mex(x, offset);
fprintf('Test 2 - offset=%d:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
ref = int8([-50, 14, -114, 77, -128]);
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: saturation
x = int8([127, -128, 100, -100]);
offset = int8(50);
y = arm_offset_q7_mex(x, offset);
fprintf('Test 3 - offset=%d (saturation):\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
ref = int8([127, -78, 127, -50]);
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: random large vector
rng(42);
n = 10000;
x = int8(randi([-128, 127], n, 1, 'int8'));
offset = int8(randi([-128, 127]));
tic; y = arm_offset_q7_mex(x, offset); t = toc;
fprintf('Test 4 - Random %d-element, offset=%d: %.3f ms\n', n, offset, t*1000);
s = int16(x) + int16(offset);
ref = int8(max(min(s, 127), -128));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    mismatches = find(y ~= ref, 5, 'first');
    fprintf('  FAIL at indices: ['); fprintf('%d ', mismatches); fprintf(']\n');
    for k = mismatches(:)', fprintf('    x(%d)=%d, z=%d, expected=%d\n', k, x(k), y(k), ref(k)); end
    errors = errors + 1;
end

% Test 5: matrix input
x = int8([1 2; 3 4]);
offset = int8(10);
y = arm_offset_q7_mex(x, offset);
fprintf('Test 5 - Matrix:\n  x:\n'); disp(x); fprintf('  y:\n'); disp(y);
ref = int8([11 12; 13 14]);
if isequal(y, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end