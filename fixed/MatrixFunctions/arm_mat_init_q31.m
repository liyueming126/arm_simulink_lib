function arm_mat_init_q31()
clc;
fprintf('=== Testing arm_mat_init_q31 ===\n\n');
errors = 0;

data = int32([1 2 3 4 5 6]);
C = arm_mat_init_q31_mex(data, int32(3), int32(2));
fprintf('Test 1 - 3x2 from vector:\n  C:\n'); disp(C);
ref = int32([1 4; 2 5; 3 6]);
if isequal(C, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

data = int32([100 200; 300 400]);
C = arm_mat_init_q31_mex(data, int32(2), int32(2));
fprintf('Test 2 - 2x2 pass-through:\n  C:\n'); disp(C);
if isequal(C, data), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end