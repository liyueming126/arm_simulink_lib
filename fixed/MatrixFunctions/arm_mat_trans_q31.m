function arm_mat_trans_q31()
clc;
fprintf('=== Testing arm_mat_trans_q31 ===\n\n');
errors = 0;

A = int32([1 2 3; 4 5 6]);
C = arm_mat_trans_q31_mex(A);
fprintf('Test 1 - 2x3 transpose:\n  C:\n'); disp(C);
ref = int32([1 4; 2 5; 3 6]);
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

rng(42);
A = int32(randi([-2147483648, 2147483647], 5, 2, 'int32'));
tic; C = arm_mat_trans_q31_mex(A); t = toc;
fprintf('Test 2 - Random 5x2: %.3f ms\n', t*1000);
ref = A';
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end