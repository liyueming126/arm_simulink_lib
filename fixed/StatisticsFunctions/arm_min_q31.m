function arm_min_q31()
clc;
fprintf('=== Testing arm_min_q31 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int32([0, 1000, -1000, 500000, -500000, -1000000]);
[val, idx] = arm_min_q31_mex(x);
fprintf('Test 1 - Basic vector: val=%d, idx=%.0f\n', val, idx);
ref_val = int32(-1000000); ref_idx = 6.0;
if val == ref_val && idx == ref_idx
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected val=%d, idx=%.0f\n\n', ref_val, ref_idx);
    errors = errors + 1;
end

% Test 2: saturation bounds
x = int32([2147483647, -2147483648, 0, -2147483648]);
[val, idx] = arm_min_q31_mex(x);
fprintf('Test 2 - Saturation bounds: val=%d, idx=%.0f\n', val, idx);
if val == int32(-2147483648) && (idx == 2.0 || idx == 4.0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: random large vector
rng(42);
n = 10000;
x = int32(randi([-2147483648, 2147483647], n, 1, 'int32'));
tic; [val, idx] = arm_min_q31_mex(x); t = toc;
fprintf('Test 3 - Random %d-element: %.3f ms\n', n, t*1000);
[dmin, didx] = min(x);
if val == dmin && idx == single(didx)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected val=%d, idx=%d\n\n', dmin, didx);
    errors = errors + 1;
end

% Test 4: single element
x = int32([42]);
[val, idx] = arm_min_q31_mex(x);
fprintf('Test 4 - Single element: val=%d, idx=%.0f\n', val, idx);
if val == int32(42) && idx == 1.0
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 5: zeros
x = int32(zeros(1,10));
[val, idx] = arm_min_q31_mex(x);
fprintf('Test 5 - All zeros: val=%d, idx=%.0f\n', val, idx);
if val == int32(0) && idx == 1.0
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end