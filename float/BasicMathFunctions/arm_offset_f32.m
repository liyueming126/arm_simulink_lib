function arm_offset_f32()
clc;
fprintf('=== Testing arm_offset_f32 ===\n\n');
errors = 0;

% Test 1: basic vector
x = single([0, 1.5, -2.5, 100, -100, 0.1, -0.1]);
offset = single(3.0);
z = arm_offset_f32_mex(x, offset);
fprintf('Test 1 - Basic vector (offset=%g):\n  x: ['); fprintf('%g ', x); fprintf(']\n  z: ['); fprintf('%g ', z); fprintf(']\n');
ref = single([3, 4.5, 0.5, 103, -97, 3.1, 2.9]);
if all(abs(z - ref) < 1e-6)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: zeros and edge values
x = single([0, inf, -inf, nan, 1.5]);
offset = single(1.0);
z = arm_offset_f32_mex(x, offset);
fprintf('Test 2 - Edge values (offset=%g):\n  x: ['); fprintf('%g ', x); fprintf(']\n  z: ['); fprintf('%g ', z); fprintf(']\n');
ref = single([1, inf, -inf, nan, 2.5]);
if all(abs(z(~isnan(z)) - ref(~isnan(ref))) < 1e-6) && all(isnan(z(isnan(ref))))
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: random large vector
rng(42);
n = 10000;
x = single(randn(n, 1) * 100);
offset = single(3.14159);
tic; z = arm_offset_f32_mex(x, offset); t = toc;
fprintf('Test 3 - Random %d-element vector: %.3f ms\n', n, t*1000);
ref = single(double(x) + double(offset));
if all(abs(z - ref) < 1e-3)
    fprintf('  PASS\n\n');
else
    mismatches = find(abs(z - ref) >= 1e-3, 5, 'first');
    fprintf('  FAIL at indices: ['); fprintf('%d ', mismatches); fprintf(']\n');
    for k = mismatches(:)', fprintf('    x(%d)=%g, z=%g, expected=%g\n', k, x(k), z(k), ref(k)); end
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end