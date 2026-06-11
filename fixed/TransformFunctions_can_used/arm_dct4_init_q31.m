function arm_dct4_init_q31()
    S = arm_dct4_init_q31_mex(256, 759250125);
    assert(S.N == 256);
    assert(S.Nby2 == 128);
    fprintf('arm_dct4_init_q31 PASSED\n');
end