function arm_mat_cmplx_mult_q15()
clc;
fprintf('=== Testing arm_mat_cmplx_mult_q15 ===\n\n');
errors = 0;

% Complex data stored interleaved: each complex column uses 2 real columns
% Data is stored in column-major order (MATLAB convention)
% Output matrix has rowsA rows and colsB columns (real)

% Test 1: Q15 0.5 * 0.5 = 0.25, 2x2 * 2x1 complex
A = int16([16384, 0, 8192, 0; 4096, 0, 2048, 0]);  % 2x2 complex: [0.5, 0.25; 0.125, 0.0625]
B = int16([9830, 0; 6554, 0]);                      % 2x1 complex: [0.3; 0.2]
C = arm_mat_cmplx_mult_q15_mex(A, B);
fprintf('Test 1 - 2x2 * 2x1:\n  C:\n'); disp(C);
% Expected: [0.5*0.3 + 0.25*0.2 = 0.2; 0.125*0.3 + 0.0625*0.2 = 0.05] = [0.2; 0.05]
% In Q15: [6554; 1638], interleaved format: 2 rows, 2 cols: [6554, 0; 1638, 0]
ref = int16(zeros(size(C)));
numRowsA = size(A, 1);
numColsA_cplx = size(A, 2) / 2;
numColsB_cplx = size(B, 2) / 2;
for p = 1:numRowsA
    for q = 1:numColsB_cplx
        acc_real = int64(0);
        acc_imag = int64(0);
        for r = 1:numColsA_cplx
            a_real = int64(A(p, 2*r-1));
            a_imag = int64(A(p, 2*r));
            b_real = int64(B(r, 2*q-1));
            b_imag = int64(B(r, 2*q));
            acc_real = acc_real + a_real * b_real - a_imag * b_imag;
            acc_imag = acc_imag + a_real * b_imag + a_imag * b_real;
        end
        p_real = idivide(acc_real, int64(32768), 'floor');
        p_real = max(min(p_real, int64(32767)), int64(-32768));
        p_imag = idivide(acc_imag, int64(32768), 'floor');
        p_imag = max(min(p_imag, int64(32767)), int64(-32768));
        ref(p, 2*q-1) = int16(p_real);
        ref(p, 2*q) = int16(p_imag);
    end
end
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  Expected:\n'); disp(ref); fprintf('  Actual:\n'); disp(C); errors = errors + 1; end

% Test 2: Identity 2x1 * 1x1 (scalar)
A = int16([32767, 0; 0, 0]);  % 2x1 complex: [1.0; 0]
B = int16([8192, 0]);         % 1x1 complex: [0.25]
C = arm_mat_cmplx_mult_q15_mex(A, B);
fprintf('Test 2 - Identity:\n  C:\n'); disp(C);
% Expected: [1.0*0.25; 0] = [0.25; 0] in Q15: [8192; 0]
% Interleaved: [8192, 0; 0, 0]
ref = int16(zeros(size(C)));
numRowsA = size(A, 1);
numColsA_cplx = size(A, 2) / 2;
numColsB_cplx = size(B, 2) / 2;
for p = 1:numRowsA
    for q = 1:numColsB_cplx
        acc_real = int64(0);
        acc_imag = int64(0);
        for r = 1:numColsA_cplx
            a_real = int64(A(p, 2*r-1));
            a_imag = int64(A(p, 2*r));
            b_real = int64(B(r, 2*q-1));
            b_imag = int64(B(r, 2*q));
            acc_real = acc_real + a_real * b_real - a_imag * b_imag;
            acc_imag = acc_imag + a_real * b_imag + a_imag * b_real;
        end
        p_real = idivide(acc_real, int64(32768), 'floor');
        p_real = max(min(p_real, int64(32767)), int64(-32768));
        p_imag = idivide(acc_imag, int64(32768), 'floor');
        p_imag = max(min(p_imag, int64(32767)), int64(-32768));
        ref(p, 2*q-1) = int16(p_real);
        ref(p, 2*q) = int16(p_imag);
    end
end
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  Expected:\n'); disp(ref); fprintf('  Actual:\n'); disp(C); errors = errors + 1; end

% Test 3: Q15 2x2 * 2x2 real-only
A = int16([16384, 0, 8192, 0; 4096, 0, 2048, 0]);  % [0.5, 0.25; 0.125, 0.0625]
B = int16([9830, 0, 19661, 0; 6554, 0, 13107, 0]);  % [0.3, 0.6; 0.2, 0.4]
C = arm_mat_cmplx_mult_q15_mex(A, B);
fprintf('Test 3 - 2x2 * 2x2 real:\n  C:\n'); disp(C);
% Expected:
% [1,1]=0.5*0.3+0.25*0.2=0.2, [1,2]=0.5*0.6+0.25*0.4=0.4
% [2,1]=0.125*0.3+0.0625*0.2=0.05, [2,2]=0.125*0.6+0.0625*0.4=0.1
% Q15: [6554, 0, 13107, 0; 1638, 0, 3277, 0]
ref = int16(zeros(size(C)));
numRowsA = size(A, 1);
numColsA_cplx = size(A, 2) / 2;
numColsB_cplx = size(B, 2) / 2;
for p = 1:numRowsA
    for q = 1:numColsB_cplx
        acc_real = int64(0);
        acc_imag = int64(0);
        for r = 1:numColsA_cplx
            a_real = int64(A(p, 2*r-1));
            a_imag = int64(A(p, 2*r));
            b_real = int64(B(r, 2*q-1));
            b_imag = int64(B(r, 2*q));
            acc_real = acc_real + a_real * b_real - a_imag * b_imag;
            acc_imag = acc_imag + a_real * b_imag + a_imag * b_real;
        end
        p_real = idivide(acc_real, int64(32768), 'floor');
        p_real = max(min(p_real, int64(32767)), int64(-32768));
        p_imag = idivide(acc_imag, int64(32768), 'floor');
        p_imag = max(min(p_imag, int64(32767)), int64(-32768));
        ref(p, 2*q-1) = int16(p_real);
        ref(p, 2*q) = int16(p_imag);
    end
end
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  Expected:\n'); disp(ref); fprintf('  Actual:\n'); disp(C); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end