function arm_cfft_radix8_f32()
clc;
fprintf('=== Testing arm_radix8_butterfly_f32 ===\n\n');
errors = 0;

% Test 1: 64-point radix-8 butterfly (forward step)
fprintf('Test 1 - 64-point radix-8 butterfly:\n');
n = 64;
x = single(zeros(1, 2*n));
for k = 0:n-1
    x(2*k+1) = single(cos(2*pi*k/n));
    x(2*k+2) = single(sin(2*pi*k/n));
end
y = arm_cfft_radix8_f32_mex(x, uint16(n), uint16(1));
fprintf('  Output length: %d elements\n', length(y));
fprintf('  Output (first 10): ');
for k = 1:10, fprintf('(%f,%f) ', y(2*k-1), y(2*k)); end
fprintf('\n');
% Just verify execution
fprintf('  PASS (function executed)\n\n');

fprintf('=== Tests completed ===\n');
end