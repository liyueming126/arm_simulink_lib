function arm_dot_prod_f32()
clc;
fprintf('=== Testing arm_dot_prod_f32 ===\n\n');
errors = 0;

% Test 1: basic vectors
x = single([1, 2, 3]);
y = single([4, 5, 6]);
z = arm_dot_prod_f32_mex(x, y);
fprintf('Test 1 - Basic:\n  x: ['); fprintf('%g ', x); fprintf(']\n  y: ['); fprintf('%g ', y); fprintf(']\n  z: %g\n', z);
ref = single(1*4 + 2*5 + 3*6);
if abs(z - ref) < 1e-6
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL (got %g, expected %g)\n\n', z, ref);
    errors = errors + 1;
end

% Test 2: zeros and edge values
x = single([0, inf, -inf, nan, 1]);
y = single([1, 1, 1, 1, 0]);
z = arm_dot_prod_f32_mex(x, y);
fprintf('Test 2 - Edge values:\n  x: ['); fprintf('%g ', x); fprintf(']\n  y: ['); fprintf('%g ', y); fprintf(']\n  z: %g\n', z);
ref = single(0 + inf - inf + nan + 0);
if isnan(z) && isnan(ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL (got %g, expected %g)\n\n', z, ref);
    errors = errors + 1;
end

% Test 3: random large vectors
rng(42);
n = 10000;
x = single(randn(n, 1) * 100);
y = single(randn(n, 1) * 100);
tic; z = arm_dot_prod_f32_mex(x, y); t = toc;
fprintf('Test 3 - Random %d-element vectors: %.3f ms\n', n, t*1000);
ref = single(sum(double(x) .* double(y)));
rel_diff = abs(double(z) - double(ref)) / max(abs(double(ref)), 1);
if rel_diff < 1e-3
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL (got %g, expected %g, diff %g)\n\n', z, ref, rel_diff);
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end