function arm_q31_to_q15()
% Test arm_q31_to_q15 MEX function
% pDst[n] = (q15_t) (pSrc[n] >> 16)

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_q31_to_q15_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

pSrc = int32([0, 65536, -65536, 2147418112, -2147418112, 1000*65536, -1000*65536]');
pDst = arm_q31_to_q15_mex(pSrc);

expected = int16(idivide(pSrc, int32(65536), 'floor'));
assert(isequal(pDst, expected), 'Mismatch in q31_to_q15');

disp('arm_q31_to_q15: PASSED');
end