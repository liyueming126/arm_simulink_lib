function arm_scale_f32()
clc;
fprintf('=== Testing arm_scale_f32 ===\n\n');
errors = 0;

% Test 1: basic vector
x = single([0, 1.5, -2.5, 100, -100, 0.1, -0.1]);
scale = single(2.0);
z = arm_scale_f32_mex(x, scale);
fprintf('Test 1 - Basic vector (scale=%g):\n  x: ['); fprintf('%g ', x); fprintf(']\n  z: ['); fprintf('%g ', z); fprintf(']\n');
ref = single([0, 3, -5, 200, -200, 0.2, -0.2]);
if all(abs(z - ref) < 1e-6)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: zeros and edge values
x = single([0, inf, -inf, nan, 1.5]);
scale = single(2.0);
z = arm_scale_f32_mex(x, scale);
fprintf('Test 2 - Edge values (scale=%g):\n  x: ['); fprintf('%g ', x); fprintf(']\n  z: ['); fprintf('%g ', z); fprintf(']\n');
ref = single([0, inf, -inf, nan, 3]);
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
scale = single(2.71828);
tic; z = arm_scale_f32_mex(x, scale); t = toc;
fprintf('Test 3 - Random %d-element vector: %.3f ms\n', n, t*1000);
ref = single(double(x) * double(scale));
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