function arm_add_q7()
clc;
fprintf('=== Testing arm_add_q7 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int8([0, 64, -64, 127, -128, 1, -1]);
y = int8([0, 50, 50, 1, -1, 127, 127]);
z = arm_add_q7_mex(x, y);
fprintf('Test 1 - Basic vector:\n  x: ['); fprintf('%d ', x); fprintf(']\n  y: ['); fprintf('%d ', y); fprintf(']\n  z: ['); fprintf('%d ', z); fprintf(']\n');
ref = int8([0, 114, -14, 127, -128, 127, 126]);
if isequal(z, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: ['); fprintf('%d ', ref); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 2: saturation
x = int8([127, 127, -128, -128, 64, -64]);
y = int8([1, 127, -1, -128, 64, -64]);
z = arm_add_q7_mex(x, y);
fprintf('Test 2 - Saturation:\n  x: ['); fprintf('%d ', x); fprintf(']\n  y: ['); fprintf('%d ', y); fprintf(']\n  z: ['); fprintf('%d ', z); fprintf(']\n');
ref = int8([127, 127, -128, -128, 127, -128]);
if isequal(z, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: random large vector
rng(42);
n = 10000;
x = int8(randi([-128, 127], n, 1, 'int8'));
y = int8(randi([-128, 127], n, 1, 'int8'));
tic; z = arm_add_q7_mex(x, y); t = toc;
fprintf('Test 3 - Random %d-element vector: %.3f ms\n', n, t*1000);
s = int16(x) + int16(y);
ref = int8(max(min(s, 127), -128));
if isequal(z, ref)
    fprintf('  PASS\n\n');
else
    mismatches = find(z ~= ref, 5, 'first');
    fprintf('  FAIL at indices: ['); fprintf('%d ', mismatches); fprintf(']\n');
    for k = mismatches(:)', fprintf('    x(%d)=%d, y(%d)=%d, z=%d, expected=%d\n', k, x(k), k, y(k), z(k), ref(k)); end
    errors = errors + 1;
end

% Test 4: matrix input
x = int8([1 2; 3 4]);
y = int8([10 20; 30 40]);
z = arm_add_q7_mex(x, y);
fprintf('Test 4 - Matrix:\n  x:\n'); disp(x); fprintf('  y:\n'); disp(y); fprintf('  z:\n'); disp(z);
ref = int8([11 22; 33 44]);
if isequal(z, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end