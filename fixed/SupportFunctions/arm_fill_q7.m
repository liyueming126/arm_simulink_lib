function arm_fill_q7()
% Test arm_fill_q7 MEX function

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_fill_q7_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

blockSize = 50;
value = int8(100);
pDst = arm_fill_q7_mex(value, blockSize);

expected = repmat(value, blockSize, 1);
assert(isequal(pDst, expected), 'Mismatch in fill_q7');

% Test negative
value = int8(-128);
pDst = arm_fill_q7_mex(value, blockSize);
expected = repmat(value, blockSize, 1);
assert(isequal(pDst, expected), 'Mismatch in fill_q7 negative');

% Test max positive
value = int8(127);
pDst = arm_fill_q7_mex(value, blockSize);
expected = repmat(value, blockSize, 1);
assert(isequal(pDst, expected), 'Mismatch in fill_q7 max');

disp('arm_fill_q7: PASSED');
end