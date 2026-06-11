function arm_float_to_q31()
% Test arm_float_to_q31 MEX function
% pDst[n] = sat_q31(pSrc[n] * 2147483648)

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_float_to_q31_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

pSrc = single([0.0, 0.5, -0.5, 0.999999, -1.0, 0.0000005, -0.0000005, 2.0, -2.0, 1.5, -1.5]');
pDst = arm_float_to_q31_mex(pSrc);

q31_min = int32(-2147483648);
q31_max = int32(2147483647);
expected = zeros(size(pSrc), 'int32');
for i = 1:numel(pSrc)
    v = double(pSrc(i)) * 2147483648.0;
    if v > 2147483647.0
        expected(i) = q31_max;
    elseif v < -2147483648.0
        expected(i) = q31_min;
    else
        expected(i) = int32(floor(v));
    end
end
assert(isequal(pDst, expected), 'Mismatch in float_to_q31');

disp('arm_float_to_q31: PASSED');
end