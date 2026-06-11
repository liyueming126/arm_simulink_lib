function arm_mat_cmplx_mult_q31()
clc;
fprintf('=== Testing arm_mat_cmplx_mult_q31 ===\n\n');
errors = 0;

% Data stored interleaved: each complex column uses 2 real columns
% Test 1: Q31 2x2 * 2x2 real-only (0.5*0.5=0.25)
A = int32([1073741824, 0, 536870912, 0; 268435456, 0, 134217728, 0]);  % [0.5, 0.25; 0.125, 0.0625]
B = int32([1073741824, 0, 536870912, 0; 268435456, 0, 134217728, 0]);  % [0.5, 0.25; 0.125, 0.0625]
C = arm_mat_cmplx_mult_q31_mex(A, B);
fprintf('Test 1 - 2x2 * 2x2 real:\n  C:\n'); disp(C);
% Expected:
% [1,1]=0.5*0.5+0.25*0.125=0.28125  -> 0x24000000 = 603979776
% [1,2]=0.5*0.25+0.25*0.0625=0.140625  -> 0x12000000 = 301989888
% [2,1]=0.125*0.5+0.0625*0.125=0.0703125 -> 0x09000000 = 150994944
% [2,2]=0.125*0.25+0.0625*0.0625=0.03515625 -> 0x04800000 = 75497472
% Interleaved output: [603979776, 0, 301989888, 0; 150994944, 0, 75497472, 0]
ref = int32(zeros(size(C)));
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
        ref(p, 2*q-1) = int32(bitsra(acc_real, 31));
        ref(p, 2*q) = int32(bitsra(acc_imag, 31));
    end
end
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  Expected:\n'); disp(ref); fprintf('  Actual:\n'); disp(C); errors = errors + 1; end

% Test 2: Identity 2x1 * 1x1
A = int32([2147483647, 0; 0, 0]);  % 2x1 complex: [~1.0; 0]
B = int32([536870912, 0]);          % 1x1 complex: [0.25]
C = arm_mat_cmplx_mult_q31_mex(A, B);
fprintf('Test 2 - Identity:\n  C:\n'); disp(C);
% Expected: [1.0*0.25; 0] = [0.25; 0] in Q31: [536870912; 0]
% Interleaved: [536870912, 0; 0, 0]
ref = int32(zeros(size(C)));
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
        ref(p, 2*q-1) = int32(bitsra(acc_real, 31));
        ref(p, 2*q) = int32(bitsra(acc_imag, 31));
    end
end
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  Expected:\n'); disp(ref); fprintf('  Actual:\n'); disp(C); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end