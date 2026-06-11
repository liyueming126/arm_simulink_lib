function arm_std_q31()
clc;
fprintf('=== Testing arm_std_q31 ===\n\n');
errors = 0;

% Test 1: constant (std = 0)
x = int32(ones(1,5) * 1000);
z = arm_std_q31_mex(x);
fprintf('Test 1 - Constant 1000: z=%d\n', z);
if z == int32(0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: two elements
x = int32([100000, -100000]);
z = arm_std_q31_mex(x);
fprintf('Test 2 - [+100000 -100000]: z=%d\n', z);

% Test 3: single element (std = 0)
x = int32([42]);
z = arm_std_q31_mex(x);
fprintf('Test 3 - Single element: z=%d\n', z);
if z == int32(0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: small values
x = int32([1, 2, 3, 4, 5]);
z = arm_std_q31_mex(x);
fprintf('Test 4 - [1 2 3 4 5]: z=%d\n', z);

% Test 5: moderate random
rng(42);
n = 1000;
x = int32(randi([-10000, 10000], n, 1, 'int32'));
tic; z = arm_std_q31_mex(x); t = toc;
fprintf('Test 5 - Random %d-element: %.3f ms, z=%d\n', n, t*1000, z);
if z >= 0
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end