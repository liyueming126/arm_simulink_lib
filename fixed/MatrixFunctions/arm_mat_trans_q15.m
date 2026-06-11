function arm_mat_trans_q15()
clc;
fprintf('=== Testing arm_mat_trans_q15 ===\n\n');
errors = 0;

A = int16([1 2 3; 4 5 6]);
C = arm_mat_trans_q15_mex(A);
fprintf('Test 1 - 2x3 transpose:\n  A:\n'); disp(A); fprintf('  C:\n'); disp(C);
ref = int16([1 4; 2 5; 3 6]);
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

A = int16([100 200]);
C = arm_mat_trans_q15_mex(A);
fprintf('Test 2 - 1x2 to 2x1:\n  C:\n'); disp(C);
ref = int16([100; 200]);
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

rng(42);
A = int16(randi([-32768, 32767], 4, 6, 'int16'));
tic; C = arm_mat_trans_q15_mex(A); t = toc;
fprintf('Test 3 - Random 4x6: %.3f ms\n', t*1000);
ref = A';
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

A = int16([-32768 32767; 0 -1]);
C = arm_mat_trans_q15_mex(A);
fprintf('Test 4 - Extreme values:\n  C:\n'); disp(C);
ref = A';
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end