function arm_q7_to_q31()
% Test arm_q7_to_q31 MEX function
% pDst[n] = (q31_t) pSrc[n] << 24

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_q7_to_q31_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

pSrc = int8([0, 1, -1, 127, -128, 64, -64, 10, -10, 42, -42]');
pDst = arm_q7_to_q31_mex(pSrc);

expected = int32(pSrc) * int32(16777216);  % << 24
assert(isequal(pDst, expected), 'Mismatch in q7_to_q31');

disp('arm_q7_to_q31: PASSED');
end