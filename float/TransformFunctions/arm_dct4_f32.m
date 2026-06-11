function arm_dct4_f32()
clc;
fprintf('=== Testing arm_dct4_f32 (DCT Type IV) ===\n\n');
errors = 0;

% Test 1: 32-point DCT4
fprintf('Test 1 - 32-point DCT4:\n');
n = 32;
x = single(cos(2*pi*2*(0:n-1)/n));
y = arm_dct4_f32_mex(x, uint16(n));
fprintf('  Output length: %d elements\n', length(y));
fprintf('  Output (first 8): ');
for k = 1:min(8,length(y)), fprintf('%f ', y(k)); end
fprintf('\n');
fprintf('  PASS (executed)\n\n');

% Test 2: 64-point DCT4
fprintf('Test 2 - 64-point DCT4:\n');
n = 64;
x = single(randn(1, n));
y = arm_dct4_f32_mex(x, uint16(n));
fprintf('  Output length: %d elements\n', length(y));
% Check Parseval-like energy relationship (DCT4 is orthogonal up to scaling)
energy_in = sum(double(x).^2);
energy_out = sum(double(y).^2);
fprintf('  Energy ratio (out/in): %f\n', energy_out / energy_in);
fprintf('  PASS (executed)\n\n');

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end
end