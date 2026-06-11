function arm_mat_init_q15()
clc;
fprintf('=== Testing arm_mat_init_q15 ===\n\n');
errors = 0;

data = int16([1 2 3 4 5 6]);
C = arm_mat_init_q15_mex(data, int32(2), int32(3));
fprintf('Test 1 - 2x3 from vector:\n  C:\n'); disp(C);
ref = int16([1 3 5; 2 4 6]);
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

data = int16([100 200; 300 400]);
C = arm_mat_init_q15_mex(data, int32(2), int32(2));
fprintf('Test 2 - 2x2 from matrix:\n  C:\n'); disp(C);
ref = data;
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end