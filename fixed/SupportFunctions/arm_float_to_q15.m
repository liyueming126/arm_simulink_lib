function arm_float_to_q15()
% Test arm_float_to_q15 MEX function
% pDst[n] = sat_q15(pSrc[n] * 32768)

mex_src = fullfile(fileparts(mfilename('fullpath')), 'arm_float_to_q15_mex.c');
if ~exist([mex_src(1:end-2) '.' mexext], 'file')
    mex(mex_src);
end

pSrc = single([0.0, 0.5, -0.5, 0.9999, -1.0, 0.00003, -0.00003, 2.0, -2.0, 1.5, -1.5]');
pDst = arm_float_to_q15_mex(pSrc);

q15_min = int16(-32768);
q15_max = int16(32767);
expected = zeros(size(pSrc), 'int16');
for i = 1:numel(pSrc)
    v = double(pSrc(i)) * 32768.0;
    if v > 32767.0
        expected(i) = q15_max;
    elseif v < -32768.0
        expected(i) = q15_min;
    else
        expected(i) = int16(floor(v));
    end
end
assert(isequal(pDst, expected), 'Mismatch in float_to_q15');

disp('arm_float_to_q15: PASSED');
end