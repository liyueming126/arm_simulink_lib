function arm_dct4_init_f32()
clc;
fprintf('=== Testing arm_dct4_init_f32 ===\n\n');
errors = 0;

% Test 1: Init 128-point DCT4
fprintf('Test 1 - Init 128-point DCT4:\n');
N = 128;
Nby2 = 64;
normalize = single(sqrt(2/N));
S = arm_dct4_init_f32_mex(uint16(N), uint16(Nby2), normalize);
if isstruct(S) && S.N == 128 && S.Nby2 == 64
    fprintf('  PASS (N=%d, Nby2=%d, normalize=%f)\n\n', S.N, S.Nby2, S.normalize);
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: Init 64-point
fprintf('Test 2 - Init 64-point DCT4:\n');
N = 64;
S = arm_dct4_init_f32_mex(uint16(N), uint16(N/2), single(sqrt(2/N)));
if isstruct(S) && S.N == 64
    fprintf('  PASS (N=%d)\n\n', S.N);
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end
end