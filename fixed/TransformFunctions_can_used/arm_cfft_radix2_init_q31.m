function arm_cfft_radix2_init_q31()
    fftLen = 256;
    S = arm_cfft_radix2_init_q31_mex(fftLen, 1, 1);
    assert(S.fftLen == fftLen);
    assert(S.ifftFlag == 1);
    assert(S.bitReverseFlag == 1);
    assert(S.twidCoefModifier == 16);
    assert(S.bitRevFactor == 16);
    fprintf('arm_cfft_radix2_init_q31 PASSED\n');
end