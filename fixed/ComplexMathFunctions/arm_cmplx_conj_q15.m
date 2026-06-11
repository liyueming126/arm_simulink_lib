function arm_cmplx_conj_q15()
    fprintf('=== arm_cmplx_conj_q15 ===\n');

    tests = {
        zeros(1, 4, 'int16'),                          % [0,0,0,0]
        int16([1, 2, 3, 4]),                           % small values
        int16([32767, -32768, 32767, 32767]),           % saturation edge
        int16([-32768, -32768, 32767, -32767]),         % -32768 -> 0x7fff sat
        int16([-100, 200, 500, -300]),                  % mixed signs
        int16([1, -1, 2, -2]),                          % simple inversion
    };

    for i = 1:length(tests)
        inp = tests{i};
        out = arm_cmplx_conj_q15_mex(inp);
        fprintf('  Test %d: ', i);
        fprintf('in=[%d,%d,%d,%d] ', inp(1), inp(2), inp(3), inp(4));
        fprintf('out=[%d,%d,%d,%d]\n', out(1), out(2), out(3), out(4));

        for j = 1:2:length(inp)
            assert(out(j) == inp(j), 'Real part must be unchanged');
            if inp(j+1) == int16(-32768)
                assert(out(j+1) == int16(32767), '-32768 imag must saturate to 32767');
            else
                assert(out(j+1) == -inp(j+1), 'Imag part must be negated');
            end
        end
    end

    fprintf('  All tests passed.\n');
end