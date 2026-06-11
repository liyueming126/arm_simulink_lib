function arm_add_q15()
clc;
fprintf('=== Testing arm_add_q15 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int16([0, 100, -100, 32767, -32768, 1, -1]);
y = int16([0, 50, 50, 1, -1, 32767, 32767]);
z = arm_add_q15_mex(x, y);
fprintf('Test 1 - Basic vector:\n  x: ['); fprintf('%d ', x); fprintf(']\n  y: ['); fprintf('%d ', y); fprintf(']\n  z: ['); fprintf('%d ', z); fprintf(']\n');
ref = int16([0, 150, -50, 32767, -32768, 32767, 32766]);
if isequal(z, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: ['); fprintf('%d ', ref); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 2: saturation
x = int16([32767, 32767, -32768, -32768, 20000, -20000]);
y = int16([1, 32767, -1, -32768, 20000, -20000]);
z = arm_add_q15_mex(x, y);
fprintf('Test 2 - Saturation:\n  x: ['); fprintf('%d ', x); fprintf(']\n  y: ['); fprintf('%d ', y); fprintf(']\n  z: ['); fprintf('%d ', z); fprintf(']\n');
ref = int16([32767, 32767, -32768, -32768, 32767, -32768]);
if isequal(z, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: random large vector
rng(42);
n = 10000;
x = int16(randi([-32768, 32767], n, 1, 'int16'));
y = int16(randi([-32768, 32767], n, 1, 'int16'));
tic; z = arm_add_q15_mex(x, y); t = toc;
fprintf('Test 3 - Random %d-element vector: %.3f ms\n', n, t*1000);
s = double(x) + double(y);
ref = int16(max(min(s, 32767), -32768));
if isequal(z, ref)
    fprintf('  PASS\n\n');
else
    mismatches = find(z ~= ref, 5, 'first');
    fprintf('  FAIL at indices: ['); fprintf('%d ', mismatches); fprintf(']\n');
    for k = mismatches(:)', fprintf('    x(%d)=%d, y(%d)=%d, z=%d, expected=%d\n', k, x(k), k, y(k), z(k), ref(k)); end
    errors = errors + 1;
end

% Test 4: matrix input
x = int16([1 2; 3 4]);
y = int16([10 20; 30 40]);
z = arm_add_q15_mex(x, y);
fprintf('Test 4 - Matrix:\n  x:\n'); disp(x); fprintf('  y:\n'); disp(y); fprintf('  z:\n'); disp(z);
ref = int16([11 22; 33 44]);
if isequal(z, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end