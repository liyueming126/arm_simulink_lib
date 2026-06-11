function arm_cfft_radix4_init_f32()
clc;
fprintf('=== Testing arm_cfft_radix4_init_f32 ===\n\n');
errors = 0;

% Test 1: Initialize for 16-point
fprintf('Test 1 - Init 16-point forward:\n');
S = arm_cfft_radix4_init_f32_mex(16, 0, 1);
if isstruct(S) && S.fftLen == 16
    fprintf('  PASS (fftLen=%d)\n\n', S.fftLen);
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: Invalid length
fprintf('Test 2 - Invalid length:\n');
try
    S = arm_cfft_radix4_init_f32_mex(32, 0, 1);
    fprintf('  FAIL (should error)\n\n');
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