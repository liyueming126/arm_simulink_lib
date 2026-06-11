function arm_cfft_radix4_init_q15()
    fftLen = 256;
    S = arm_cfft_radix4_init_q15_mex(fftLen, 0, 1);
    assert(S.fftLen == fftLen);
    assert(S.twidCoefModifier == 16);
    assert(S.bitRevFactor == 16);
    fprintf('arm_cfft_radix4_init_q15 PASSED\n');
end