function arm_mult_q15()
clc;
fprintf('=== Testing arm_mult_q15 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int16([0, 32767, -32768, 16384, -16384, 1000, -1000]);
y = int16([0, 32767, 1, 16384, 16384, 1000, 1000]);
z = arm_mult_q15_mex(x, y);
fprintf('Test 1 - Basic vector:\n  x: ['); fprintf('%d ', x); fprintf(']\n  y: ['); fprintf('%d ', y); fprintf(']\n  z: ['); fprintf('%d ', z); fprintf(']\n');
ref = int16(zeros(size(x)));
for i = 1:numel(x)
    p = int32(x(i)) * int32(y(i));
    p = idivide(p, int32(32768), 'floor');
    p = max(min(p, int32(32767)), int32(-32768));
    ref(i) = int16(p);
end
if isequal(z, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n  expected: ['); fprintf('%d ', ref); fprintf(']\n  actual:   ['); fprintf('%d ', z); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 2: saturation
x = int16([32767, 32767, -32768, 23170]);
y = int16([32767, 16384, -32768, 23170]);
z = arm_mult_q15_mex(x, y);
fprintf('Test 2 - Saturation:\n  x: ['); fprintf('%d ', x); fprintf(']\n  y: ['); fprintf('%d ', y); fprintf(']\n  z: ['); fprintf('%d ', z); fprintf(']\n');
% CMSIS: (int32_t)a * b >> 15, then saturate to Q15 range
ref = int16(zeros(size(x)));
for i = 1:numel(x)
    p = int32(x(i)) * int32(y(i));
    p = idivide(p, int32(32768), 'floor');
    p = max(min(p, int32(32767)), int32(-32768));
    ref(i) = int16(p);
end
if isequal(z, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n  expected: ['); fprintf('%d ', ref); fprintf(']\n  actual:   ['); fprintf('%d ', z); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 3: random large vector
rng(42);
n = 10000;
x = int16(randi([-32768, 32767], n, 1, 'int16'));
y = int16(randi([-32768, 32767], n, 1, 'int16'));
tic; z = arm_mult_q15_mex(x, y); t = toc;
fprintf('Test 3 - Random %d-element vector: %.3f ms\n', n, t*1000);
% Use int32 reference with truncation to match C arithmetic right shift
ref = int16(zeros(n, 1));
for i = 1:n
    p = int32(x(i)) * int32(y(i));
    p = idivide(p, int32(32768), 'floor');
    if p > 32767, p = int32(32767);
    elseif p < -32768, p = int32(-32768); end
    ref(i) = int16(p);
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
x = int16([32767 16384; 8192 4096]);
y = int16([32767 32767; 32767 32767]);
z = arm_mult_q15_mex(x, y);
fprintf('Test 4 - Matrix:\n  x:\n'); disp(x); fprintf('  y:\n'); disp(y); fprintf('  z:\n'); disp(z);
ref = int16([32766 16383; 8191 4095]);
z_flat = z(:); ref_flat = ref(:);
if isequal(z, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  expected:\n'); disp(ref); fprintf('  actual:\n'); disp(z); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end