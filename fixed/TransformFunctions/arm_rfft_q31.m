function arm_rfft_q31()
clc;
fprintf('=== Testing arm_rfft_q31 RFFT/RIFFT (forward and inverse) ===\n\n');
errors = 0;

% -----------------------------------------------------------------------
% RFFT/RIFFT round-trip test for multiple sizes
% The Real FFT transforms N real samples into N complex spectrum values,
% and the inverse recovers the original N real samples (scaled by N).
% -----------------------------------------------------------------------
fprintf('Test 1 - RFFT/RIFFT round-trip for various sizes:\n');

sizes = [32, 64, 128, 256];
pass_count = 0;

for idx = 1:length(sizes)
    n = sizes(idx);

    % Create a real input signal (sine wave)
    x = int32(zeros(1, n));
    for k = 0:n-1
        x(k+1) = int32(round(100000000 * sin(2*pi*3*k/n)));  % bin 3 sine
    end

    % Forward RFFT: real -> complex
    y = arm_rfft_q31_mex(x, 0);

    % Output should have 2*n elements (n complex interleaved)
    if length(y) ~= 2*n
        fprintf('    N=%d: FAIL (output length %d, expected %d)\n', n, length(y), 2*n);
        errors = errors + 1;
        continue;
    end

    % Inverse RIFFT: complex -> real
    z = arm_rfft_q31_mex(y, 1);

    % Output should have n elements
    if length(z) ~= n
        fprintf('    N=%d: FAIL (RIFFT output length %d, expected %d)\n', n, length(z), n);
        errors = errors + 1;
        continue;
    end

    % Compare with original input
    diff = max(abs(double(x) - double(z)));
    if diff <= 10
        pass_count = pass_count + 1;
        fprintf('    N=%d: PASS (max diff=%d)\n', n, diff);
    else
        fprintf('    N=%d: FAIL (max diff=%d > 10)\n', n, diff);
        errors = errors + 1;
    end
end
fprintf('  %d/%d sizes passed\n\n', pass_count, length(sizes));

% -----------------------------------------------------------------------
% Test 2: Verify output is conjugate symmetric (property of real FFT)
% -----------------------------------------------------------------------
fprintf('Test 2 - Conjugate symmetry check:\n');

n = 64;
x = int32(zeros(1, n));
for k = 0:n-1
    x(k+1) = int32(round(80000000 * cos(2*pi*2*k/n) + 50000000 * sin(2*pi*5*k/n)));
end

y = arm_rfft_q31_mex(x, 0);

% For a real input, X[k] = conj(X[N-k])
y_real = double(y(1:2:end));
y_imag = double(y(2:2:end));

% Check symmetry: X[0] and X[N/2] should be purely real
sym_ok = 1;
if abs(y_imag(1)) > 500
    fprintf('  FAIL: DC bin has non-zero imaginary part: %d\n', y_imag(1));
    sym_ok = 0;
end
if abs(y_imag(n/2+1)) > 500
    fprintf('  FAIL: Nyquist bin has non-zero imaginary part: %d\n', y_imag(n/2+1));
    sym_ok = 0;
end

% Check X[k] == conj(X[N-k]) for k=1..N/2-1
max_sym_err = 0;
for k = 2:n/2
    re_diff = abs(y_real(k) - y_real(n - k + 2));
    im_diff = abs(y_imag(k) + y_imag(n - k + 2));
    max_sym_err = max(max_sym_err, max(re_diff, im_diff));
end

if max_sym_err <= 500
    fprintf('  Maximum conjugate symmetry error: %d\n', max_sym_err);
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL: Conjugate symmetry violated (max err=%d)\n', max_sym_err);
    errors = errors + 1;
end

% -----------------------------------------------------------------------
% Test 3: DC input (all constant value) - verify only DC bin output
% -----------------------------------------------------------------------
fprintf('Test 3 - DC input test:\n');

n = 64;
x = int32(ones(1, n) * 100000000);

y = arm_rfft_q31_mex(x, 0);

% For a constant input, only DC (bin 0) should be nonzero
y_mag = sqrt(double(y(1:2:end)).^2 + double(y(2:2:end)).^2);
max_non_dc = max(y_mag(2:end));

if max_non_dc < 1000
    fprintf('  DC value: %.1f, max non-DC magnitude: %.1f\n', y_mag(1), max_non_dc);
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL: Non-DC bins too large (max=%.1f)\n', max_non_dc);
    errors = errors + 1;
end

% -----------------------------------------------------------------------
% Summary
% -----------------------------------------------------------------------
if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end
end