function arm_max_q15()
clc;
fprintf('=== Testing arm_max_q15 ===\n\n');
errors = 0;

% Test 1: basic vector
x = int16([0, 100, -100, 500, -500, 1000]);
[val, idx] = arm_max_q15_mex(x);
fprintf('Test 1 - Basic vector: x=['); fprintf('%d ', x); fprintf(']\n  val=%d, idx=%.0f\n', val, idx);
ref_val = int16(1000); ref_idx = 6.0;
if val == ref_val && idx == ref_idx
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected val=%d, idx=%.0f\n\n', ref_val, ref_idx);
    errors = errors + 1;
end

% Test 2: negative values
x = int16([-32768, -100, -1, 0]);
[val, idx] = arm_max_q15_mex(x);
fprintf('Test 2 - All non-positive: val=%d, idx=%.0f\n', val, idx);
ref_val = int16(0); ref_idx = 4.0;
if val == ref_val && idx == ref_idx
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: saturation bounds
x = int16([32767, -32768, 0, 32767]);
[val, idx] = arm_max_q15_mex(x);
fprintf('Test 3 - Saturation bounds: val=%d, idx=%.0f\n', val, idx);
ref_val = int16(32767); ref_idx = 1.0;
if val == ref_val && idx == ref_idx
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: random large vector
rng(42);
n = 10000;
x = int16(randi([-32768, 32767], n, 1, 'int16'));
tic; [val, idx] = arm_max_q15_mex(x); t = toc;
fprintf('Test 4 - Random %d-element: %.3f ms\n', n, t*1000);
[dmax, didx] = max(x);
if val == dmax && idx == single(didx)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected val=%d, idx=%d\n\n', dmax, didx);
    errors = errors + 1;
end

% Test 5: single element
x = int16([42]);
[val, idx] = arm_max_q15_mex(x);
fprintf('Test 5 - Single element: val=%d, idx=%.0f\n', val, idx);
if val == int16(42) && idx == 1.0
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 6: zeros
x = int16(zeros(1,10));
[val, idx] = arm_max_q15_mex(x);
fprintf('Test 6 - All zeros: val=%d, idx=%.0f\n', val, idx);
if val == int16(0) && idx == 1.0
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end