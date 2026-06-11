function arm_q7_to_q15()
% Test arm_q7_to_q15 MEX function
% pDst[n] = (q15_t) pSrc[n] << 8

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_q7_to_q15_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

pSrc = int8([0, 1, -1, 127, -128, 64, -64, 10, -10, 42, -42]');
pDst = arm_q7_to_q15_mex(pSrc);

expected = int16(pSrc) * int16(256);  % << 8
assert(isequal(pDst, expected), 'Mismatch in q7_to_q15');

disp('arm_q7_to_q15: PASSED');
end