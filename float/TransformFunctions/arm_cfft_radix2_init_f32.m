function arm_cfft_radix2_init_f32()
clc;
fprintf('=== Testing arm_cfft_radix2_init_f32 ===\n\n');
errors = 0;

% Test 1: Initialize for 16-point FFT
fprintf('Test 1 - Initialize 16-point forward FFT:\n');
S = arm_cfft_radix2_init_f32_mex(16, 0, 1);
if isstruct(S) && S.fftLen == 16 && S.ifftFlag == 0 && S.bitReverseFlag == 1
    fprintf('  PASS (fftLen=%d, ifftFlag=%d, bitReverseFlag=%d)\n\n', S.fftLen, S.ifftFlag, S.bitReverseFlag);
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: Initialize for 1024-point inverse FFT
fprintf('Test 2 - Initialize 1024-point inverse FFT:\n');
S = arm_cfft_radix2_init_f32_mex(1024, 1, 1);
if isstruct(S) && S.fftLen == 1024 && S.ifftFlag == 1
    fprintf('  PASS (fftLen=%d, ifftFlag=%d)\n\n', S.fftLen, S.ifftFlag);
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: Invalid FFT length
fprintf('Test 3 - Invalid FFT length:\n');
try
    S = arm_cfft_radix2_init_f32_mex(100, 0, 1);
    fprintf('  FAIL (should have errored)\n\n');
    errors = errors + 1;
catch
    fprintf('  PASS (correctly rejected invalid length)\n\n');
end

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end
end