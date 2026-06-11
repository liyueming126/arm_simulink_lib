function arm_q15_to_q31()
% Test arm_q15_to_q31 MEX function
% pDst[n] = (q31_t) pSrc[n] << 16

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_q15_to_q31_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

pSrc = int16([0, 1, -1, 32767, -32768, 1000, -1000, 42]');
pDst = arm_q15_to_q31_mex(pSrc);

expected = int32(pSrc) * 65536;  % << 16
assert(isequal(pDst, expected), 'Mismatch in q15_to_q31');

disp('arm_q15_to_q31: PASSED');
end