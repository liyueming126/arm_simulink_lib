function arm_copy_q7()
% Test arm_copy_q7 MEX function

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_copy_q7_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

blockSize = 100;
pSrc = int8(randi([-128, 127], blockSize, 1));
pDst = arm_copy_q7_mex(pSrc);

expected = pSrc;
assert(isequal(pDst, expected), 'Mismatch in copy_q7');

disp('arm_copy_q7: PASSED');
end