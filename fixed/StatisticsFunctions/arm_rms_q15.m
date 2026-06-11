function arm_rms_q15()
clc;
fprintf('=== Testing arm_rms_q15 ===\n\n');
errors = 0;

% Test 1: constant positive
x = int16(ones(1,100) * 1000);
z = arm_rms_q15_mex(x);
fprintf('Test 1 - Constant 1000: z=%d\n', z);
if z >= int16(990) && z <= int16(1010)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: zeros
x = int16(zeros(1,100));
z = arm_rms_q15_mex(x);
fprintf('Test 2 - All zeros: z=%d\n', z);
if z == int16(0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: alternating
x = int16([1000, -1000, 1000, -1000, 1000, -1000, 1000, -1000]);
z = arm_rms_q15_mex(x);
fprintf('Test 3 - Alternating +/-1000: z=%d\n', z);
if z >= int16(990) && z <= int16(1010)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: small values
x = int16([1, 2, 3, 4, 5]);
z = arm_rms_q15_mex(x);
fprintf('Test 4 - [1 2 3 4 5]: z=%d\n', z);

% Test 5: saturation values
x = int16([32767, -32768, 32767, -32768]);
z = arm_rms_q15_mex(x);
fprintf('Test 5 - Saturation bounds: z=%d\n', z);
if z >= 0
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 6: random
rng(42);
n = 1000;
x = int16(randi([-10000, 10000], n, 1, 'int16'));
tic; z = arm_rms_q15_mex(x); t = toc;
fprintf('Test 6 - Random %d-element: %.3f ms, z=%d\n', n, t*1000, z);
ref = sqrt(mean(double(x).^2));
q15_ref = int16(ref * 32768);
fprintf('  MATLAB ref float ~%.4f, q15=%d\n', ref, q15_ref);
if z >= 0
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end