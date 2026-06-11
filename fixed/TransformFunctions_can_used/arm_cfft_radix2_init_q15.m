function arm_cfft_radix2_init_q15()
    fftLen = 64;
    S = arm_cfft_radix2_init_q15_mex(fftLen, 0, 1);
    assert(S.fftLen == fftLen);
    assert(S.ifftFlag == 0);
    assert(S.bitReverseFlag == 1);
    assert(S.twidCoefModifier == 64);
    assert(S.bitRevFactor == 64);
    fprintf('arm_cfft_radix2_init_q15 PASSED\n');
end