function arm_q31_to_float()
% Test arm_q31_to_float MEX function
% pDst[n] = (float32_t) pSrc[n] / 2147483648

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_q31_to_float_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

pSrc = int32([0, 2147483647, -2147483648, 1, -1, 1073741824, -1073741824, 1000, -1000]');
pDst = arm_q31_to_float_mex(pSrc);

expected = single(pSrc) / single(2147483648);
tolerance = single(1e-7);
assert(all(abs(pDst - expected) < tolerance), 'Mismatch in q31_to_float');

disp('arm_q31_to_float: PASSED');
end