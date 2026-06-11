function arm_dct4_init_q15()
    S = arm_dct4_init_q15_mex(128, 11585);
    assert(S.N == 128);
    assert(S.Nby2 == 64);
    assert(S.normalize == 11585);
    fprintf('arm_dct4_init_q15 PASSED\n');
end