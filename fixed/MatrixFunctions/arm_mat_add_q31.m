function arm_mat_add_q31()
clc;
fprintf('=== Testing arm_mat_add_q31 ===\n\n');
errors = 0;

A = int32([1 2; 3 4]);
B = int32([10 20; 30 40]);
C = arm_mat_add_q31_mex(A, B);
fprintf('Test 1 - Basic 2x2:\n  C:\n'); disp(C);
ref = int32(max(min(int64(A)+int64(B), int64(2147483647)), int64(-2147483648)));
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  expected:\n'); disp(ref); fprintf('  actual:\n'); disp(C); errors = errors + 1; end

A = int32([2147483647 2147483647; -2147483648 -2147483648]);
B = int32([1 2147483647; -1 -2147483648]);
C = arm_mat_add_q31_mex(A, B);
fprintf('Test 2 - Saturation:\n  C:\n'); disp(C);
ref = int32(max(min(int64(A)+int64(B), int64(2147483647)), int64(-2147483648)));
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  expected:\n'); disp(ref); fprintf('  actual:\n'); disp(C); errors = errors + 1; end

rng(42);
A = int32(randi([-2147483648, 2147483647], 3, 5, 'int32'));
B = int32(randi([-2147483648, 2147483647], 3, 5, 'int32'));
tic; C = arm_mat_add_q31_mex(A, B); t = toc;
fprintf('Test 3 - Random 3x5: %.3f ms\n', t*1000);
s = int64(A) + int64(B);
ref = int32(max(min(s, int64(2147483647)), int64(-2147483648)));
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n  expected:\n'); disp(ref); fprintf('  actual:\n'); disp(C); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end