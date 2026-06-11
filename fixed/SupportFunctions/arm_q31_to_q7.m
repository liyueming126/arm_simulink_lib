function arm_q31_to_q7()
% Test arm_q31_to_q7 MEX function
% pDst[n] = (q7_t) (pSrc[n] >> 24)

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_q31_to_q7_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

pSrc = int32([0, 16777216, -16777216, 2147483647, -2147483648, 100000000, -100000000, 1000, -1000]');
pDst = arm_q31_to_q7_mex(pSrc);

scale = int32(2^24);
expected = int8(idivide(pSrc, scale, 'floor'));
assert(isequal(pDst, expected), 'Mismatch in q31_to_q7');

disp('arm_q31_to_q7: PASSED');
end