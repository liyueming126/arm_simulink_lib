function arm_copy_q31()
% Test arm_copy_q31 MEX function

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_copy_q31_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

blockSize = 100;
pSrc = int32(randi([-2147483648, 2147483647], blockSize, 1));
pDst = arm_copy_q31_mex(pSrc);

expected = pSrc;
assert(isequal(pDst, expected), 'Mismatch in copy_q31');

disp('arm_copy_q31: PASSED');
end