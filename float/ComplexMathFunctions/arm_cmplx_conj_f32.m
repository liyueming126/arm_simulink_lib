function arm_cmplx_conj_f32
    fprintf('Testing arm_cmplx_conj_f32...\n');
    tolerance = single(1e-6);

    %% Test 1: basic values
    x = single([1+2i, 3-4i, -5+6i, -7-8i]);
    N = length(x);
    x_interleaved = single(zeros(1, 2*N));
    x_interleaved(1:2:end) = real(x);
    x_interleaved(2:2:end) = imag(x);

    y_interleaved = arm_cmplx_conj_f32_mex(x_interleaved);
    y = y_interleaved(1:2:end) + 1i*y_interleaved(2:2:end);
    ref = conj(x);

    if all(abs(y - ref) < tolerance)
        fprintf('  Test 1 PASSED\n');
    else
        fprintf('  Test 1 FAILED\n');
    end

    %% Test 2: edge values (zeros, negative zeros, large values)
    x2 = single([0+0i, -0-0i, 1e10+1e10i, -1e10-1e10i, 0+1i, 1+0i]);
    N2 = length(x2);
    x2_interleaved = single(zeros(1, 2*N2));
    x2_interleaved(1:2:end) = real(x2);
    x2_interleaved(2:2:end) = imag(x2);

    y2_interleaved = arm_cmplx_conj_f32_mex(x2_interleaved);
    y2 = y2_interleaved(1:2:end) + 1i*y2_interleaved(2:2:end);
    ref2 = conj(x2);

    if all(abs(y2 - ref2) < tolerance)
        fprintf('  Test 2 PASSED\n');
    else
        fprintf('  Test 2 FAILED\n');
    end

    %% Test 3: random large vector with timing
    n = 10000;
    x3 = single(randn(1, n) + 1i*randn(1, n));
    x3_interleaved = single(zeros(1, 2*n));
    x3_interleaved(1:2:end) = real(x3);
    x3_interleaved(2:2:end) = imag(x3);

    tic;
    y3_interleaved = arm_cmplx_conj_f32_mex(x3_interleaved);
    mex_time = toc;

    y3 = y3_interleaved(1:2:end) + 1i*y3_interleaved(2:2:end);
    ref3 = conj(x3);

    if all(abs(y3 - ref3) < tolerance)
        fprintf('  Test 3 PASSED (time: %.6f s)\n', mex_time);
    else
        fprintf('  Test 3 FAILED\n');
    end
end