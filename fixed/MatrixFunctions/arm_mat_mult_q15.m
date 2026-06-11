function arm_mat_mult_q15()
clc;
fprintf('=== Testing arm_mat_mult_q15 ===\n\n');
errors = 0;

% Test 1: Q15 0.5 * small = 0 (after >>15, small values truncate to 0)
A = int16([1 2; 3 4; 5 6]);
B = int16([10 20 30; 40 50 60]);
C = arm_mat_mult_q15_mex(A, B);
fprintf('Test 1 - 3x2 * 2x3 = 3x3 (small):\n  C:\n'); disp(C);
ref = int16(zeros(size(C)));
for i = 1:size(A,1)
    for j = 1:size(B,2)
        acc = int64(0);
        for k = 1:size(A,2)
            acc = acc + int64(A(i,k)) * int64(B(k,j));
        end
        p = idivide(acc, int64(32768), 'floor');
        p = max(min(p, int64(32767)), int64(-32768));
        ref(i,j) = int16(p);
    end
end
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  Expected:\n'); disp(ref); fprintf('  Actual:\n'); disp(C); errors = errors + 1; end

% Test 2: Identity * matrix: 0.5 * 0.5 = 0.25 in Q15
A = int16([32767, 0, 0; 0, 32767, 0; 0, 0, 32767]);  % ~1.0 in Q15
B = int16([16384, 0, 0; 0, 16384, 0; 0, 0, 16384]);  % 0.5 in Q15
C = arm_mat_mult_q15_mex(A, B);
fprintf('Test 2 - Identity * 0.5:\n  C:\n'); disp(C);
ref = int16(zeros(size(C)));
for i = 1:size(A,1)
    for j = 1:size(B,2)
        acc = int64(0);
        for k = 1:size(A,2)
            acc = acc + int64(A(i,k)) * int64(B(k,j));
        end
        p = idivide(acc, int64(32768), 'floor');
        p = max(min(p, int64(32767)), int64(-32768));
        ref(i,j) = int16(p);
    end
end
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  Expected:\n'); disp(ref); fprintf('  Actual:\n'); disp(C); errors = errors + 1; end

% Test 3: Saturation 2x2 (max values)
A = int16([32767 32767; 32767 32767]);
B = int16([32767 32767; 32767 32767]);
C = arm_mat_mult_q15_mex(A, B);
fprintf('Test 3 - Saturation 2x2:\n  C:\n'); disp(C);
ref = int16(zeros(size(C)));
for i = 1:size(A,1)
    for j = 1:size(B,2)
        acc = int64(0);
        for k = 1:size(A,2)
            acc = acc + int64(A(i,k)) * int64(B(k,j));
        end
        p = idivide(acc, int64(32768), 'floor');
        p = max(min(p, int64(32767)), int64(-32768));
        ref(i,j) = int16(p);
    end
end
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  Expected:\n'); disp(ref); fprintf('  Actual:\n'); disp(C); errors = errors + 1; end

% Test 4: Random values (small, should give 0 in Q15)
rng(42);
A = int16(randi([-100, 100], 3, 4, 'int16'));
B = int16(randi([-100, 100], 4, 5, 'int16'));
tic; C = arm_mat_mult_q15_mex(A, B); t = toc;
fprintf('Test 4 - Random 3x4 * 4x5: %.3f ms\n', t*1000);
ref = int16(zeros(size(C)));
for i = 1:size(A,1)
    for j = 1:size(B,2)
        acc = int64(0);
        for k = 1:size(A,2)
            acc = acc + int64(A(i,k)) * int64(B(k,j));
        end
        p = idivide(acc, int64(32768), 'floor');
        p = max(min(p, int64(32767)), int64(-32768));
        ref(i,j) = int16(p);
    end
end
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  Expected:\n'); disp(ref); fprintf('  Actual:\n'); disp(C); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end