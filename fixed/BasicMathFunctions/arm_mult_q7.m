function arm_mult_q7()
clc;
fprintf('=== Testing arm_mult_q7 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int8([0, 127, -128, 64, -64, 32, -32]);
y = int8([0, 127, 1, 64, 64, 32, 32]);
z = arm_mult_q7_mex(x, y);
fprintf('Test 1 - Basic vector:\n  x: ['); fprintf('%d ', x); fprintf(']\n  y: ['); fprintf('%d ', y); fprintf(']\n  z: ['); fprintf('%d ', z); fprintf(']\n');
% CMSIS: Q7 multiplication = (int16)a * b >> 7, saturate to [-128,127]
ref = int8([0, 126, -1, 32, -32, 8, -8]);
if isequal(z, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: ['); fprintf('%d ', ref); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 2: saturation
x = int8([127, 127, -128, 90]);
y = int8([127, 64, -128, 90]);
z = arm_mult_q7_mex(x, y);
fprintf('Test 2 - Saturation:\n  x: ['); fprintf('%d ', x); fprintf(']\n  y: ['); fprintf('%d ', y); fprintf(']\n  z: ['); fprintf('%d ', z); fprintf(']\n');
ref = int8([126, 63, 127, 63]);
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
tic; z = arm_mult_q7_mex(x, y); t = toc;
fprintf('Test 3 - Random %d-element vector: %.3f ms\n', n, t*1000);
% Use int16 reference with truncation to match C arithmetic right shift
ref = int8(zeros(n, 1));
for i = 1:n
    p = int16(x(i)) * int16(y(i));
    p = idivide(p, int16(128), 'floor');
    if p > 127, p = int16(127);
    elseif p < -128, p = int16(-128); end
    ref(i) = int8(p);
end
if isequal(z, ref)
    fprintf('  PASS\n\n');
else
    mismatches = find(z ~= ref, 5, 'first');
    fprintf('  FAIL at indices: ['); fprintf('%d ', mismatches); fprintf(']\n');
    for k = mismatches(:)', fprintf('    x(%d)=%d, y(%d)=%d, z=%d, expected=%d\n', k, x(k), k, y(k), z(k), ref(k)); end
    errors = errors + 1;
end

% Test 4: matrix input
x = int8([127 64; 32 16]);
y = int8([127 127; 127 127]);
z = arm_mult_q7_mex(x, y);
fprintf('Test 4 - Matrix:\n  x:\n'); disp(x); fprintf('  y:\n'); disp(y); fprintf('  z:\n'); disp(z);
ref = int8([126 63; 31 15]);
if isequal(z, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end