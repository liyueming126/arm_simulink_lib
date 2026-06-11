function arm_rfft_fast_init_f32()
clc;
fprintf('=== Testing arm_rfft_fast_init_f32 ===\n\n');
errors = 0;

% Test 1: Init 64-point
fprintf('Test 1 - Init 64-point:\n');
S = arm_rfft_fast_init_f32_mex(64);
if isstruct(S) && S.fftLenRFFT == 64 && S.cfftLen == 32
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: Init 1024-point
fprintf('Test 2 - Init 1024-point:\n');
S = arm_rfft_fast_init_f32_mex(1024);
if isstruct(S) && S.fftLenRFFT == 1024 && S.cfftLen == 512
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end
end