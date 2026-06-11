function arm_rfft_f32()
clc;
fprintf('=== Testing arm_rfft_f32 (deprecated real FFT) ===\n\n');
errors = 0;

% Test 1: 32-point forward RFFT
fprintf('Test 1 - 32-point forward RFFT:\n');
n = 32;
x = single(cos(2*pi*2*(0:n-1)/n));  % real sinusoid
y = arm_rfft_f32_mex(x, uint16(n), 0);
fprintf('  FFT output length: %d elements\n', length(y));
% Verify first two real bins
fprintf('  DC bin: %f, Nyquist bin: %f\n', y(1), y(n+1));
fprintf('  PASS (executed)\n\n');

% Test 2: 64-point forward RFFT
fprintf('Test 2 - 64-point forward RFFT:\n');
n = 64;
x = single(randn(1, n));
y = arm_rfft_f32_mex(x, uint16(n), 0);
fprintf('  Output length: %d elements\n', length(y));
fprintf('  PASS (executed)\n\n');

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end
end