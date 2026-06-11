function arm_mat_scale_q31()
clc;
fprintf('=== Testing arm_mat_scale_q31 ===\n\n');
errors = 0;

A = int32([0 1073741824; -1073741824 536870912]);
C = arm_mat_scale_q31_mex(A, int32(2147483647), int32(-1));
fprintf('Test 1 - scale=~1.0, shift=-1:\n  C:\n'); disp(C);
% Q31 multiply truncates, expect max error <= 1 LSB
in2 = bitsra(int64(A) .* int64(2147483647), 32);
out = in2 .* int64(1);
ref = int32(max(min(out, int64(2147483647)), int64(-2147483648)));
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  expected:\n'); disp(ref); fprintf('  actual:\n'); disp(C); errors = errors + 1; end

C = arm_mat_scale_q31_mex(A, int32(1073741824), int32(0));
fprintf('Test 2 - scale=0.5, shift=0:\n  C:\n'); disp(C);
in2 = bitsra(int64(A) .* int64(1073741824), 32);
out = in2 .* int64(2);
ref = int32(max(min(out, int64(2147483647)), int64(-2147483648)));
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  expected:\n'); disp(ref); fprintf('  actual:\n'); disp(C); errors = errors + 1; end

rng(42);
A = int32(randi([-2147483648, 2147483647], 3, 3, 'int32'));
tic; C = arm_mat_scale_q31_mex(A, int32(1073741824), int32(-1)); t = toc;
fprintf('Test 3 - Random 3x3 with shift=-1: %.3f ms\n', t*1000);
in2 = bitsra(int64(A) .* int64(1073741824), 32);
out = in2 .* int64(1);
ref = int32(max(min(out, int64(2147483647)), int64(-2147483648)));
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  expected:\n'); disp(ref); fprintf('  actual:\n'); disp(C); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end