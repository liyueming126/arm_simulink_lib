function arm_fill_q15()
% Test arm_fill_q15 MEX function

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_fill_q15_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

blockSize = 50;
value = int16(12345);
pDst = arm_fill_q15_mex(value, blockSize);

expected = repmat(value, blockSize, 1);
assert(isequal(pDst, expected), 'Mismatch in fill_q15');

% Test negative
value = int16(-32768);
pDst = arm_fill_q15_mex(value, blockSize);
expected = repmat(value, blockSize, 1);
assert(isequal(pDst, expected), 'Mismatch in fill_q15 negative');

disp('arm_fill_q15: PASSED');
end