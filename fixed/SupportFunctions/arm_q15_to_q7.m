function arm_q15_to_q7()
% Test arm_q15_to_q7 MEX function
% pDst[n] = (q7_t) (pSrc[n] >> 8)

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_q15_to_q7_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

pSrc = int16([0, 256, -256, 32767, -32768, 128, -128, 1000, -1000, 2048, -2048]');
pDst = arm_q15_to_q7_mex(pSrc);

expected = int8(idivide(pSrc, int16(256), 'floor'));
assert(isequal(pDst, expected), 'Mismatch in q15_to_q7');

disp('arm_q15_to_q7: PASSED');
end