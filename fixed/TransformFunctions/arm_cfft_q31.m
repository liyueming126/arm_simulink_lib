function arm_cfft_q31()
clc;
fprintf('=== Testing arm_cfft_q31 FFT (forward and inverse) ===\n\n');
errors = 0;

% Radix-4 DIF FFT scales both forward and inverse by ~1/N per stage.
% Verification uses spectral peak comparison and correlation.

% -----------------------------------------------------------------------
% Test 1: Forward FFT spectral peak check (16-point)
% -----------------------------------------------------------------------
fprintf('Test 1 - 16-point forward FFT spectral check:\n');

n = 16;
x = int32(zeros(1, 2*n));
for k = 0:n-1
    x(2*k+1) = int32(round(100000000 * cos(2*pi*k/n)));   % real
    x(2*k+2) = int32(round(80000000 * sin(2*pi*k/n)));    % imag
end

fprintf('  Input (real part):  ['); fprintf('%d ', x(1:2:end)); fprintf(']\n');
fprintf('  Input (imag part):  ['); fprintf('%d ', x(2:2:end)); fprintf(']\n');

y = arm_cfft_q31_mex(x, 0);
fprintf('  FFT output (real):  ['); fprintf('%d ', y(1:2:end)); fprintf(']\n');
fprintf('  FFT output (imag):  ['); fprintf('%d ', y(2:2:end)); fprintf(']\n');

% Reference: MATLAB FFT
x_cplx = double(x(1:2:end)) + 1j * double(x(2:2:end));
y_ref = fft(x_cplx);

% Our FFT output
y_cplx = double(y(1:2:end)) + 1j * double(y(2:2:end));

% Compare magnitudes to verify peak bins match
mag_mex = abs(y_cplx);
mag_ref = abs(y_ref);

[~, peak_mex] = max(mag_mex(2:end));
[~, peak_ref] = max(mag_ref(2:end));
peak_mex = peak_mex + 1;
peak_ref = peak_ref + 1;

if peak_mex == peak_ref
    fprintf('  Peak bin: %d (MATLAB: %d) PASS\n', peak_mex, peak_ref);
else
    fprintf('  Peak bin: %d (MATLAB: %d) FAIL\n', peak_mex, peak_ref);
    errors = errors + 1;
end

% -----------------------------------------------------------------------
% Test 2: 16-point forward+inverse FFT (correlation check)
% -----------------------------------------------------------------------
fprintf('\nTest 2 - 16-point forward+inverse FFT:\n');

z = arm_cfft_q31_mex(y, 1);
x_cplx_z = double(z(1:2:end)) + 1j * double(z(2:2:end));
corr = abs(sum(x_cplx .* conj(x_cplx_z))) / (sqrt(sum(abs(x_cplx).^2)) * sqrt(sum(abs(x_cplx_z).^2)));

fprintf('  Input-output correlation: %.6f\n', corr);
if corr > 0.95
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL (correlation too low)\n\n');
    errors = errors + 1;
end

% -----------------------------------------------------------------------
% Test 3: 32-point using radix4by2 path
% -----------------------------------------------------------------------
fprintf('Test 3 - 32-point forward+inverse FFT:\n');

n = 32;
x = int32(zeros(1, 2*n));
for k = 0:n-1
    x(2*k+1) = int32(round(50000000 * cos(4*pi*k/n)));
    x(2*k+2) = int32(round(30000000 * sin(4*pi*k/n)));
end

y = arm_cfft_q31_mex(x, 0);
z = arm_cfft_q31_mex(y, 1);

x_cplx = double(x(1:2:end)) + 1j * double(x(2:2:end));
x_cplx_z = double(z(1:2:end)) + 1j * double(z(2:2:end));
corr = abs(sum(x_cplx .* conj(x_cplx_z))) / (sqrt(sum(abs(x_cplx).^2)) * sqrt(sum(abs(x_cplx_z).^2)));

fprintf('  Input-output correlation: %.6f\n', corr);
if corr > 0.95
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% -----------------------------------------------------------------------
% Test 4: Impulse test for multiple sizes
% -----------------------------------------------------------------------
fprintf('Test 4 - Impulse response test (all sizes):\n');

sizes = [16, 32, 64, 128, 256];
pass_count = 0;

for idx = 1:length(sizes)
    n = sizes(idx);
    x = int32(zeros(1, 2*n));
    x(1) = 2147483647;

    y = arm_cfft_q31_mex(x, 0);
    z = arm_cfft_q31_mex(y, 1);

    x_cplx = double(x(1:2:end)) + 1j * double(x(2:2:end));
    x_cplx_z = double(z(1:2:end)) + 1j * double(z(2:2:end));
    corr = abs(sum(x_cplx .* conj(x_cplx_z))) / (sqrt(sum(abs(x_cplx).^2)) * sqrt(sum(abs(x_cplx_z).^2)));

    if corr > 0.90
        pass_count = pass_count + 1;
        fprintf('    N=%d: PASS (corr=%.4f)\n', n, corr);
    else
        fprintf('    N=%d: FAIL (corr=%.4f)\n', n, corr);
        errors = errors + 1;
    end
end
fprintf('  %d/%d sizes passed\n\n', pass_count, length(sizes));

% -----------------------------------------------------------------------
% Summary
% -----------------------------------------------------------------------
if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end
end