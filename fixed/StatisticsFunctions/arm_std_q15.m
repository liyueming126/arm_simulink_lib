function arm_std_q15()
clc;
fprintf('=== Testing arm_std_q15 ===\n\n');
errors = 0;

% Test 1: constant (std = 0)
x = int16(ones(1,5) * 100);
z = arm_std_q15_mex(x);
fprintf('Test 1 - Constant 100: z=%d\n', z);
if z == int16(0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: two elements
x = int16([1000, -1000]);
z = arm_std_q15_mex(x);
fprintf('Test 2 - [+1000 -1000]: z=%d\n', z);

% Test 3: single element (std = 0)
x = int16([42]);
z = arm_std_q15_mex(x);
fprintf('Test 3 - Single element: z=%d\n', z);
if z == int16(0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: small values
x = int16([1, 2, 3, 4, 5]);
z = arm_std_q15_mex(x);
fprintf('Test 4 - [1 2 3 4 5]: z=%d\n', z);

% Test 5: saturation values
x = int16([32767, -32768, 0, 10000]);
z = arm_std_q15_mex(x);
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
x = int16(randi([-5000, 5000], n, 1, 'int16'));
tic; z = arm_std_q15_mex(x); t = toc;
fprintf('Test 6 - Random %d-element: %.3f ms, z=%d\n', n, t*1000, z);
if z >= 0
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end