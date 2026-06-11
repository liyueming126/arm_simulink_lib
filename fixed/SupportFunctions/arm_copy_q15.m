function arm_copy_q15()
% Test arm_copy_q15 MEX function

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_copy_q15_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

blockSize = 100;
pSrc = int16(randi([-32768, 32767], blockSize, 1));
pDst = arm_copy_q15_mex(pSrc);

expected = pSrc;
assert(isequal(pDst, expected), 'Mismatch in copy_q15');

disp('arm_copy_q15: PASSED');
end