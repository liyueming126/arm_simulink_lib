function arm_rfft_init_q31()
    S = arm_rfft_init_q31_mex(256, 1, 1);
    assert(S.fftLenReal == 256);
    assert(S.ifftFlagR == 1);
    assert(S.bitReverseFlagR == 1);
    fprintf('arm_rfft_init_q31 PASSED\n');
end