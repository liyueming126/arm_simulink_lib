function arm_rfft_init_f32()
clc;
fprintf('=== Testing arm_rfft_init_f32 ===\n\n');
errors = 0;

% Test 1: Init 64-point forward
fprintf('Test 1 - Init 64-point forward:\n');
S = arm_rfft_init_f32_mex(64, 0, 1);
if isstruct(S) && S.fftLenReal == 64 && S.fftLenBy2 == 32
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: Init 1024-point inverse
fprintf('Test 2 - Init 1024-point inverse:\n');
S = arm_rfft_init_f32_mex(1024, 1, 1);
if isstruct(S) && S.fftLenReal == 1024
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: Invalid length
fprintf('Test 3 - Invalid length:\n');
try
    S = arm_rfft_init_f32_mex(100, 0, 1);
    fprintf('  FAIL\n\n');
    errors = errors + 1;
catch
    fprintf('  PASS\n\n');
end

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end
end