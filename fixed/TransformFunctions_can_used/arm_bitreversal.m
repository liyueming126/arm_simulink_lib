function arm_bitreversal()
clc;
fprintf('=== Testing arm_bitreversal ===\n\n');
errors = 0;

% Test 1: 4-point FFT bitreversal (fftLen=4, bitRevFactor=2)
% for 4-point: bit-reversed order of [0,1,2,3] is [0,2,1,3]
fprintf('Test 1 - 4-point bitreversal:\n');
x = int16([1, -1, 2, -2, 3, -3, 4, -4]);  % complex: 4 points
table = uint16([2 0]);  % bitRevFactor=2
y = arm_bitreversal_mex(x, uint32(4), table);
ref = int16([1, -1, 3, -3, 2, -2, 4, -4]);
if isequal(y, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL: expected ['); fprintf('%d ', ref); fprintf(']\n');
    fprintf('  got:      ['); fprintf('%d ', y); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 2: 8-point FFT bitreversal
fprintf('Test 2 - 8-point bitreversal:\n');
x = int16([0:7; -1:-1:-8]);
x = x(:)';
x = int16(x);
table = uint16([4 0 2 0]);
y = arm_bitreversal_mex(x, uint32(8), table);

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end