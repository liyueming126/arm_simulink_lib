function arm_cfft_radix2_f32()
clc;
fprintf('=== Testing arm_cfft_radix2_f32 ===\n\n');
errors = 0;

% Test 1: 16-point forward FFT, verify with MATLAB
fprintf('Test 1 - 16-point forward FFT:\n');
n = 16;
x = single(zeros(1, 2*n));
for k = 0:n-1
    x(2*k+1) = single(cos(2*pi*k/n));   % real
    x(2*k+2) = single(sin(2*pi*k/n));   % imag
end
y = arm_cfft_radix2_f32_mex(x, 0);
% Compare magnitude spectrum
y_cplx = double(y(1:2:end)) + 1j*double(y(2:2:end));
ref_cplx = fft(double(x(1:2:end)) + 1j*double(x(2:2:end)));
% Compare (normalize by n since CMSIS FFT grows by n)
err = max(abs(abs(y_cplx) - abs(ref_cplx)));
if err < 1e-5 * max(abs(ref_cplx))
    fprintf('  PASS (mag error = %e)\n\n', err);
else
    fprintf('  FAIL (mag error = %e)\n\n', err);
    errors = errors + 1;
end

% Test 2: Forward+Inverse round-trip
fprintf('Test 2 - Forward+Inverse round-trip:\n');
n = 16;
x = single(randn(1, 2*n));
y = arm_cfft_radix2_f32_mex(x, 0);
z = arm_cfft_radix2_f32_mex(y, 1);
% Correlation
x_cplx = double(x(1:2:end)) + 1j*double(x(2:2:end));
z_cplx = double(z(1:2:end)) + 1j*double(z(2:2:end));
corr = abs(sum(x_cplx .* conj(z_cplx))) / (sqrt(sum(abs(x_cplx).^2)) * sqrt(sum(abs(z_cplx).^2)));
fprintf('  Correlation: %.6f\n', corr);
if corr > 0.99
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: 32-point forward+inverse
fprintf('Test 3 - 32-point round-trip:\n');
n = 32;
x = single(randn(1, 2*n));
y = arm_cfft_radix2_f32_mex(x, 0);
z = arm_cfft_radix2_f32_mex(y, 1);
x_cplx = double(x(1:2:end)) + 1j*double(x(2:2:end));
z_cplx = double(z(1:2:end)) + 1j*double(z(2:2:end));
corr = abs(sum(x_cplx .* conj(z_cplx))) / (sqrt(sum(abs(x_cplx).^2)) * sqrt(sum(abs(z_cplx).^2)));
fprintf('  Correlation: %.6f\n', corr);
if corr > 0.99
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