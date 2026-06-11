function arm_cfft_radix4_q31()
clc;
fprintf('=== Testing arm_cfft_radix4_q31 ===\n\n');
errors = 0;

n = 16;
x = int32(zeros(1, 2*n));
for k = 0:n-1
    x(2*k+1) = int32(cos(2*pi*3*k/n) * 10000);
    x(2*k+2) = int32(sin(2*pi*3*k/n) * 10000);
end

y = arm_cfft_radix4_q31_mex(x, uint8(0), uint8(1));
fprintf('Test 1 - 16-point forward FFT with bitreversal:\n');
fprintf('  Output range: [%d, %d]\n', int64(min(y)), int64(max(y)));
fprintf('  PASS (no crash)\n\n');

y2 = arm_cfft_radix4_q31_mex(y, uint8(1), uint8(1));
fprintf('Test 2 - IFFT round-trip:\n');
maxerr = max(abs(int64(x) - int64(y2)));
fprintf('  Max round-trip error: %d\n', maxerr);
if maxerr < 100
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end