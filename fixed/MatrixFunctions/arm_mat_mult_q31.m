function arm_mat_mult_q31()
clc;
fprintf('=== Testing arm_mat_mult_q31 ===\n\n');
errors = 0;

% Test 1: Basic 2x2 * 2x2 with small values (Q31: all products << 2^31, result is 0)
A = int32([1 2; 3 4]);
B = int32([10 20; 30 40]);
C = arm_mat_mult_q31_mex(A, B);
fprintf('Test 1 - 2x2 * 2x2 (small values):\n  C:\n'); disp(C);
% Q31 multiply shifts result >> 31, so small inputs give 0
ref = int32(zeros(2,2));
for i = 1:2, for j = 1:2, acc = int64(0); for k = 1:2, acc = acc + int64(A(i,k))*int64(B(k,j)); end; ref(i,j) = int32(bitsra(acc, 31)); end; end
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  expected:\n'); disp(ref); fprintf('  actual:\n'); disp(C); errors = errors + 1; end

% Test 2: Q31-scaled values (0.5 * 0.5 = 0.25 in Q31)
A = int32([1073741824, 0; 0, 1073741824]);  % [0.5, 0; 0, 0.5] in Q31
B = int32([1073741824, 0; 0, 1073741824]);  % [0.5, 0; 0, 0.5] in Q31
C = arm_mat_mult_q31_mex(A, B);
fprintf('Test 2 - Q31 0.5 * 0.5 = 0.25:\n  C:\n'); disp(C);
ref = int32(zeros(2,2));
for i = 1:2, for j = 1:2, acc = int64(0); for k = 1:2, acc = acc + int64(A(i,k))*int64(B(k,j)); end; ref(i,j) = int32(bitsra(acc, 31)); end; end
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  expected:\n'); disp(ref); fprintf('  actual:\n'); disp(C); errors = errors + 1; end

% Test 3: Q31 identity multiply
A = int32([2147483647, 0; 0, 2147483647]);  % ~1.0 in Q31
B = int32([1073741824, -536870912; 268435456, 536870912]);  % [0.5, -0.25; 0.125, 0.25] in Q31
C = arm_mat_mult_q31_mex(A, B);
fprintf('Test 3 - Q31 identity:\n  C:\n'); disp(C);
ref = int32(zeros(2,2));
for i = 1:2, for j = 1:2, acc = int64(0); for k = 1:2, acc = acc + int64(A(i,k))*int64(B(k,j)); end; ref(i,j) = int32(bitsra(acc, 31)); end; end
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  expected:\n'); disp(ref); fprintf('  actual:\n'); disp(C); errors = errors + 1; end

% Test 4: Random values (scaled to Q31 range)
rng(42);
A = int32(randi([-1000, 1000], 3, 4, 'int32'));
B = int32(randi([-1000, 1000], 4, 5, 'int32'));
tic; C = arm_mat_mult_q31_mex(A, B); t = toc;
fprintf('Test 4 - Random 3x4 * 4x5 (small): %.3f ms\n', t*1000);
% Small values produce 0 in Q31
ref = int32(zeros(3,5));
for i = 1:3, for j = 1:5, acc = int64(0); for k = 1:4, acc = acc + int64(A(i,k))*int64(B(k,j)); end; ref(i,j) = int32(bitsra(acc, 31)); end; end
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  expected:\n'); disp(ref); fprintf('  actual:\n'); disp(C); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end