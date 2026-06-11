function arm_mat_mult_fast_q15()
clc;
fprintf('=== Testing arm_mat_mult_fast_q15 ===\n\n');
errors = 0;

% Test 1: Q15 0.5 * 0.5 = 0.25
A = int16([16384, 0; 0, 16384; 8192, 8192]);  % [0.5, 0; 0, 0.5; 0.25, 0.25]
B = int16([16384, 0, 0; 0, 16384, 0]);         % [0.5, 0, 0; 0, 0.5, 0]
C = arm_mat_mult_fast_q15_mex(A, B);
fprintf('Test 1 - 3x2 * 2x3 = 3x3:\n  C:\n'); disp(C);
% Expected: [0.5*0.5, 0; 0, 0.5*0.5; 0.25*0.5, 0.25*0.5]
% = [0.25, 0, 0; 0, 0.25, 0; 0.125, 0.125, 0] in Q15
% 0.25*32768=8192, 0.125*32768=4096
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

% Test 2: Identity * 2x2
A = int16([32767, 0; 0, 32767]);  % ~1.0 in Q15
B = int16([8192, 4096; -16384, 16384]);  % [0.25, 0.125; -0.5, 0.5]
C = arm_mat_mult_fast_q15_mex(A, B);
fprintf('Test 2 - Identity * 2x2:\n  C:\n'); disp(C);
% Identity * B should equal B (in Q15, with truncation)
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

% Test 3: Random values (small, should produce 0 in Q15)
rng(42);
A = int16(randi([-100, 100], 4, 3, 'int16'));
B = int16(randi([-100, 100], 3, 6, 'int16'));
tic; C = arm_mat_mult_fast_q15_mex(A, B); t = toc;
fprintf('Test 3 - Random 4x3 * 3x6: %.3f ms\n', t*1000);
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