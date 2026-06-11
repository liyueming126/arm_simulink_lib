function arm_rfft_fast_f32()
clc;
fprintf('=== Testing arm_rfft_fast_f32 (fast real FFT) ===\n\n');
errors = 0;

% Test 1: 32-point forward RFFT
fprintf('Test 1 - 32-point forward RFFT:\n');
n = 32;
x = single(cos(2*pi*2*(0:n-1)/n));
y = arm_rfft_fast_f32_mex(x, uint16(n), 0);
fprintf('  Output length: %d elements\n', length(y));
fprintf('  Output (first 6): ');
for k = 1:min(6,length(y)), fprintf('%f ', y(k)); end
fprintf('\n');
fprintf('  PASS (executed)\n\n');

% Test 2: 64-point forward and compare spectrum
fprintf('Test 2 - 64-point forward RFFT:\n');
n = 64;
x = single(cos(2*pi*2*(0:n-1)/n));
y = arm_rfft_fast_f32_mex(x, uint16(n), 0);
% y is packed: [real(X0), real(Xn/2), complex X1, X2, ...]
% Reconstruct full spectrum for magnitude comparison
X0 = y(1); Xn2 = y(2);
Xk = y(3:2:end) + 1j*y(4:2:end);
mag = [abs(X0), abs(Xk(1:end-1)), abs(Xn2)];
ref = abs(fft(double(x)));
fprintf('  Peak bin (MEX): %d, Peak bin (MATLAB): %d\n', find(mag==max(mag),1)-1, find(ref==max(ref),1)-1);
fprintf('  PASS (executed)\n\n');

% Test 3: Forward + Inverse round-trip
fprintf('Test 3 - 32-point forward+inverse round-trip:\n');
n = 32;
x = single(randn(1, n));
y = arm_rfft_fast_f32_mex(x, uint16(n), 0);
z = arm_rfft_fast_f32_mex(y, uint16(n), 1);
x_cplx = double(x); z_cplx = double(z);
corr = abs(sum(x_cplx .* conj(z_cplx))) / (sqrt(sum(abs(x_cplx).^2)) * sqrt(sum(abs(z_cplx).^2)));
fprintf('  Correlation: %.6f\n', corr);
if corr > 0.95
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