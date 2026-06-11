function arm_shift_q15()
clc;
fprintf('=== Testing arm_shift_q15 ===\n\n');
errors = 0;

% Test 1: left shift by 2
x = int16([0, 100, -100, 10000, -10000, 32767, -32768]);
y = arm_shift_q15_mex(x, int8(2));
fprintf('Test 1 - Left shift by 2:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
ref = int16([0, 400, -400, 32767, -32768, 32767, -32768]);
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL - expected: ['); fprintf('%d ', ref); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 2: right shift by 2
y = arm_shift_q15_mex(x, int8(-2));
fprintf('Test 2 - Right shift by 2:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
ref = int16([0, 25, -25, 2500, -2500, 8191, -8192]);
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: zero shift
y = arm_shift_q15_mex(x, int8(0));
fprintf('Test 3 - Zero shift:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
if isequal(y, x)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: right shift of all values (preserves sign bit)
x = int16([-32768, -32767, -1, 0, 1, 32766, 32767]);
y = arm_shift_q15_mex(x, int8(-1));
fprintf('Test 4 - Right shift by 1:\n  Input:  ['); fprintf('%d ', x); fprintf(']\n  Output: ['); fprintf('%d ', y); fprintf(']\n');
ref = bitshift(x, -1);
if isequal(y, ref), fprintf('  PASS\n\n'); else fprintf('  FAIL\n\n'); errors = errors + 1; end

% Test 5: random large vector
rng(42);
n = 10000;
x = int16(randi([-32768, 32767], n, 1, 'int16'));
tic; y = arm_shift_q15_mex(x, int8(3)); t = toc;
fprintf('Test 5 - Random %d-element, left shift 3: %.3f ms\n', n, t*1000);
s = double(x) * (2^3);
ref = int16(max(min(s, 32767), -32768));
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    mismatches = find(y ~= ref, 5, 'first');
    fprintf('  FAIL at indices: ['); fprintf('%d ', mismatches); fprintf(']\n');
    for k = mismatches(:)', fprintf('    x(%d)=%d, y=%d, expected=%d\n', k, x(k), y(k), ref(k)); end
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end