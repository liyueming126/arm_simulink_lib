function arm_fill_q31()
% Test arm_fill_q31 MEX function

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_fill_q31_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

blockSize = 50;
value = int32(1234567890);
pDst = arm_fill_q31_mex(value, blockSize);

expected = repmat(value, blockSize, 1);
assert(isequal(pDst, expected), 'Mismatch in fill_q31');

% Test negative
value = int32(-2147483648);
pDst = arm_fill_q31_mex(value, blockSize);
expected = repmat(value, blockSize, 1);
assert(isequal(pDst, expected), 'Mismatch in fill_q31 negative');

disp('arm_fill_q31: PASSED');
end