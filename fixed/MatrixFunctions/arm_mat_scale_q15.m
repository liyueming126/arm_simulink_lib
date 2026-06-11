function arm_mat_scale_q15()
clc;
fprintf('=== Testing arm_mat_scale_q15 ===\n\n');
errors = 0;

A = int16([0 32767; -32768 16384]);
C = arm_mat_scale_q15_mex(A, int16(32767), int32(0));
fprintf('Test 1 - scale=0.99997, shift=0:\n  C:\n'); disp(C);
ref = int16([0 32766; -32767 16383]);
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

C = arm_mat_scale_q15_mex(A, int16(16384), int32(0));
fprintf('Test 2 - scale=0.5, shift=0:\n  C:\n'); disp(C);
ref = int16([0 16383; -16384 8192]);
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

rng(42);
A = int16(randi([-32768, 32767], 3, 5, 'int16'));
tic; C = arm_mat_scale_q15_mex(A, int16(16384), int32(2)); t = toc;
fprintf('Test 3 - Random 3x5: %.3f ms\n', t*1000);
kShift = 15 - 2;
s = (double(A) .* 16384.0) ./ (2.0^kShift);
ref = int16(max(min(floor(s), 32767), -32768));
if isequal(C, ref), fprintf('  PASS\n\n'); else mismatches = find(C ~= ref, 5); fprintf('  FAIL at indices: ['); fprintf('%d ', mismatches); fprintf(']\n\n'); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end