function arm_rms_q31()
clc;
fprintf('=== Testing arm_rms_q31 ===\n\n');
errors = 0;

% Test 1: Q31 constant value
% For Q31 RMS, input values should be large enough that (sum(x^2)/N) >> 31 > 0
% To get RMS output ~10000 in Q31: need x such that sqrt(x^2 * 2^31) = 10000
% x = sqrt(10000^2 / 2^31) = 10000 / sqrt(2^31) = 10000 / 46340 = 0.216 in Q31
% But with integer math, the minimum x is > sqrt(2^31) = 46340 for x^2 >> 31 > 0
x = int32(ones(1,100) * 50000);  % Q31 constant (50000/2^31 ~= 2.33e-5)
z = arm_rms_q31_mex(x);
fprintf('Test 1 - Constant 50000: z=%d (float=%.4f)\n', z, double(z)/2147483648.0);
if z > 0
    fprintf('  PASS (non-zero RMS)\n\n');
else
    fprintf('  FAIL (zero RMS)\n\n');
    errors = errors + 1;
end

% Test 2: zeros
x = int32(zeros(1,100));
z = arm_rms_q31_mex(x);
fprintf('Test 2 - All zeros: z=%d\n', z);
if z == int32(0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: Moderate Q31 values produce non-negative RMS
x = int32([100000, 200000, -100000, -200000]);
z = arm_rms_q31_mex(x);
fprintf('Test 3 - Moderate values: z=%d\n', z);
if z >= 0
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: random Q31 values
rng(42);
n = 1000;
x = int32(randi([-100000, 100000], n, 1, 'int32'));
tic; z = arm_rms_q31_mex(x); t = toc;
fprintf('Test 4 - Random %d-element: %.3f ms, z=%d\n', n, t*1000, z);
if z >= 0
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end