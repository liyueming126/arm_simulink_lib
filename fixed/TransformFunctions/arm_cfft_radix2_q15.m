function arm_cfft_radix2_q15()
clc;
fprintf('=== Testing arm_cfft_radix2_q15 ===\n\n');
errors = 0;

n = 16;
x = int16(zeros(1, 2*n));
for k = 0:n-1
    x(2*k+1) = int16(cos(2*pi*3*k/n) * 10000);
    x(2*k+2) = int16(sin(2*pi*3*k/n) * 10000);
end

y = arm_cfft_radix2_q15_mex(x, uint8(0), uint8(1));
fprintf('Test 1 - 16-point forward FFT with bitreversal:\n');
fprintf('  Output range: [%d, %d]\n', int32(min(y)), int32(max(y)));
fprintf('  PASS (no crash)\n\n');

y2 = arm_cfft_radix2_q15_mex(y, uint8(1), uint8(1));
fprintf('Test 2 - IFFT round-trip:\n');
% Radix-2 DIF FFT scales by N in forward and N in inverse, so round-trip = original / N^2
maxerr = max(abs(double(x) - double(y2) * n * n));
fprintf('  Max round-trip error: %.1f\n', maxerr);
if maxerr < 200
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end