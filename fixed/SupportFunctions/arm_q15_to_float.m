function arm_q15_to_float()
% Test arm_q15_to_float MEX function
% pDst[n] = (float32_t) pSrc[n] / 32768

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_q15_to_float_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

pSrc = int16([0, 32767, -32768, 1, -1, 16384, -16384, 100, -100]');
pDst = arm_q15_to_float_mex(pSrc);

expected = single(pSrc) / single(32768);
tolerance = single(1e-7);
assert(all(abs(pDst - expected) < tolerance), 'Mismatch in q15_to_float');

disp('arm_q15_to_float: PASSED');
end