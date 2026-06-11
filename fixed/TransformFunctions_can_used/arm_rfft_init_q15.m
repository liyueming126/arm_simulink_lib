function arm_rfft_init_q15()
    S = arm_rfft_init_q15_mex(128, 0, 1);
    assert(S.fftLenReal == 128);
    assert(S.ifftFlagR == 0);
    assert(S.bitReverseFlagR == 1);
    fprintf('arm_rfft_init_q15 PASSED\n');
end