function arm_q7_to_float()
% Test arm_q7_to_float MEX function
% pDst[n] = (float32_t) pSrc[n] / 128

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_q7_to_float_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

pSrc = int8([0, 127, -128, 1, -1, 64, -64, 32, -32, 100, -100]');
pDst = arm_q7_to_float_mex(pSrc);

expected = single(pSrc) / single(128);
tolerance = single(1e-7);
assert(all(abs(pDst - expected) < tolerance), 'Mismatch in q7_to_float');

disp('arm_q7_to_float: PASSED');
end