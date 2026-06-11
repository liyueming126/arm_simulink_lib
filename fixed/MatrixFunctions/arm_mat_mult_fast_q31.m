function arm_mat_mult_fast_q31()
clc;
fprintf('=== Testing arm_mat_mult_fast_q31 ===\n\n');
errors = 0;

% Test 1: Q31-scaled multiply (0.5 * 0.5 = 0.25)
A = int32([1073741824, 0; 0, 1073741824]);  % 0.5 in Q31
B = int32([1073741824, 0; 0, 1073741824]);  % 0.5 in Q31
C = arm_mat_mult_fast_q31_mex(A, B);
fprintf('Test 1 - Q31 0.5 * 0.5 = 0.25:\n  C:\n'); disp(C);
ref = int32(zeros(size(C)));
for i = 1:size(A,1)
    for j = 1:size(B,2)
        acc = int64(0);
        for k = 1:size(A,2)
            acc = acc + int64(A(i,k)) * int64(B(k,j));
        end
        ref(i,j) = int32(bitsra(acc, 31));
    end
end
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  Expected:\n'); disp(ref); fprintf('  Actual:\n'); disp(C); errors = errors + 1; end

% Test 2: Identity
A = int32([2147483647, 0; 0, 2147483647]);  % ~1.0 in Q31
B = int32([536870912, 268435456; -536870912, 1073741824]);  % [0.25, 0.125; -0.25, 0.5]
C = arm_mat_mult_fast_q31_mex(A, B);
fprintf('Test 2 - Identity:\n  C:\n'); disp(C);
ref = int32(zeros(size(C)));
for i = 1:size(A,1)
    for j = 1:size(B,2)
        acc = int64(0);
        for k = 1:size(A,2)
            acc = acc + int64(A(i,k)) * int64(B(k,j));
        end
        ref(i,j) = int32(bitsra(acc, 31));
    end
end
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  Expected:\n'); disp(ref); fprintf('  Actual:\n'); disp(C); errors = errors + 1; end

% Test 3: Random Q31 values produce non-zero result
rng(42);
A = int32(randi([-1000, 1000], 3, 4, 'int32'));
B = int32(randi([-1000, 1000], 4, 5, 'int32'));
tic; C = arm_mat_mult_fast_q31_mex(A, B); t = toc;
fprintf('Test 3 - Random 3x4 * 4x5: %.3f ms\n', t*1000);
% Small values give 0 in Q31
ref = int32(zeros(size(C)));
for i = 1:size(A,1)
    for j = 1:size(B,2)
        acc = int64(0);
        for k = 1:size(A,2)
            acc = acc + int64(A(i,k)) * int64(B(k,j));
        end
        ref(i,j) = int32(bitsra(acc, 31));
    end
end
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  Expected:\n'); disp(ref); fprintf('  Actual:\n'); disp(C); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end