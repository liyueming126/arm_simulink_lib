function arm_q15_to_float()
clc;
fprintf('=== Testing arm_q15_to_float ===\n\n');
errors = 0;

% Test 1: Basic conversion
fprintf('Test 1 - Basic conversion:\n');
x = int16([0, 16384, -16384, 32767, -32768]);
y = arm_q15_to_float_mex(x);
expected = single(double(x) / 32768.0);
d = max(abs(y - expected));
if d < 1e-7
    fprintf('  PASS (max diff = %e)\n\n', d);
else
    fprintf('  FAIL (max diff = %e)\n\n', d);
    errors = errors + 1;
end

% Test 2: Round-trip with float_to_q15
fprintf('Test 2 - Round-trip with float_to_q15:\n');
x = single(randn(1, 50) * 0.8);
q = arm_float_to_q15_mex(x);
x2 = arm_q15_to_float_mex(q);
d = max(abs(double(x) - double(x2)));
fprintf('  Round-trip error: %e\n', d);
if d < 2e-4
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: All values
fprintf('Test 3 - Full range:\n');
x = int16([-32768, -16384, -1, 0, 1, 16384, 32767]);
y = arm_q15_to_float_mex(x);
expected = single(double(x) / 32768.0);
d = max(abs(y - expected));
if d < 1e-7
    fprintf('  PASS (max diff = %e)\n\n', d);
else
    fprintf('  FAIL (max diff = %e)\n\n', d);
    errors = errors + 1;
end

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end
end