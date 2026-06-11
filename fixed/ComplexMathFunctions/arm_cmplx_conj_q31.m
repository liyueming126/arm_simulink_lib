function arm_cmplx_conj_q31()
    fprintf('=== arm_cmplx_conj_q31 ===\n');

    tests = {
        zeros(1, 4, 'int32'),                                  % [0,0,0,0]
        int32([1, 2, 3, 4]),                                   % small values
        int32([2147483647, -2147483648, 2147483647, 2147483647]), % saturation edge
        int32([-2147483648, -2147483648, 2147483647, -2147483647]), % -INT_MIN -> INT_MAX
        int32([-1000, 2000, 5000, -3000]),                      % mixed signs
        int32([1, -1, 2, -2]),                                  % simple inversion
    };

    for i = 1:length(tests)
        inp = tests{i};
        out = arm_cmplx_conj_q31_mex(inp);
        fprintf('  Test %d: ', i);
        fprintf('in=[%d,%d,%d,%d] ', inp(1), inp(2), inp(3), inp(4));
        fprintf('out=[%d,%d,%d,%d]\n', out(1), out(2), out(3), out(4));

        for j = 1:2:length(inp)
            assert(out(j) == inp(j), 'Real part must be unchanged');
            if inp(j+1) == int32(-2147483648)
                assert(out(j+1) == int32(2147483647), '-2147483648 imag must saturate to 2147483647');
            else
                assert(out(j+1) == -inp(j+1), 'Imag part must be negated');
            end
        end
    end

    fprintf('  All tests passed.\n');
end