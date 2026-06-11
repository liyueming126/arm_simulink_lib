function arm_mat_sub_q15()
clc;
fprintf('=== Testing arm_mat_sub_q15 ===\n\n');
errors = 0;

A = int16([100 200; 300 400]);
B = int16([10 20; 30 40]);
C = arm_mat_sub_q15_mex(A, B);
fprintf('Test 1 - Basic 2x2:\n  C:\n'); disp(C);
ref = int16([90 180; 270 360]);
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

A = int16([32767 -32768; 0 1]);
B = int16([-32768 32767; 0 2]);
C = arm_mat_sub_q15_mex(A, B);
fprintf('Test 2 - Saturation:\n  C:\n'); disp(C);
ref = int16([32767 -32768; 0 -1]);
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

rng(42);
A = int16(randi([-32768, 32767], 5, 3, 'int16'));
B = int16(randi([-32768, 32767], 5, 3, 'int16'));
tic; C = arm_mat_sub_q15_mex(A, B); t = toc;
fprintf('Test 3 - Random 5x3: %.3f ms\n', t*1000);
s = double(A) - double(B);
ref = int16(max(min(s, 32767), -32768));
if isequal(C, ref), fprintf('  PASS\n\n'); else mismatches = find(C ~= ref, 5); fprintf('  FAIL at indices: ['); fprintf('%d ', mismatches); fprintf(']\n\n'); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end