function arm_float_to_q7()
% Test arm_float_to_q7 MEX function
% pDst[n] = sat_q7(pSrc[n] * 128)

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_float_to_q7_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

pSrc = single([0.0, 0.5, -0.5, 0.99, -1.0, 0.0078, -0.0079, 2.0, -2.0, 0.0078125, -0.0078125]');
pDst = arm_float_to_q7_mex(pSrc);

q7_min = int8(-128);
q7_max = int8(127);
expected = zeros(size(pSrc), 'int8');
for i = 1:numel(pSrc)
    v = double(pSrc(i)) * 128.0;
    if v > 127.0
        expected(i) = q7_max;
    elseif v < -128.0
        expected(i) = q7_min;
    else
        expected(i) = int8(floor(v));
    end
end
assert(isequal(pDst, expected), 'Mismatch in float_to_q7');

disp('arm_float_to_q7: PASSED');
end