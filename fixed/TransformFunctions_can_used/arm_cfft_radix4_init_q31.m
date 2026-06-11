function arm_cfft_radix4_init_q31()
    fftLen = 1024;
    S = arm_cfft_radix4_init_q31_mex(fftLen, 0, 0);
    assert(S.fftLen == fftLen);
    assert(S.twidCoefModifier == 4);
    assert(S.bitRevFactor == 4);
    fprintf('arm_cfft_radix4_init_q31 PASSED\n');
end