function arm_cmplx_mult_real_f32
    fprintf('Testing arm_cmplx_mult_real_f32...\n');
    tolerance = single(1e-6);

    %% Test 1: basic values
    a1 = single([1+2i, 3+4i]);
    b1 = single([5, 7]);
    N1 = length(a1);
    a1_interleaved = single(zeros(1, 2*N1));
    a1_interleaved(1:2:end) = real(a1);
    a1_interleaved(2:2:end) = imag(a1);

    y_interleaved = arm_cmplx_mult_real_f32_mex(a1_interleaved, b1);
    y = y_interleaved(1:2:end) + 1i*y_interleaved(2:2:end);
    ref = a1 .* b1;

    if all(abs(y - ref) < tolerance)
        fprintf('  Test 1 PASSED\n');
    else
        fprintf('  Test 1 FAILED\n');
    end

    %% Test 2: edge values (zeros, negatives)
    a2 = single([0+0i, -1+0i, 0-2i, -3+0i]);
    b2 = single([0, -5, 3, -1]);
    N2 = length(a2);
    a2_interleaved = single(zeros(1, 2*N2));
    a2_interleaved(1:2:end) = real(a2);
    a2_interleaved(2:2:end) = imag(a2);

    y2_interleaved = arm_cmplx_mult_real_f32_mex(a2_interleaved, b2);
    y2 = y2_interleaved(1:2:end) + 1i*y2_interleaved(2:2:end);
    ref2 = a2 .* b2;

    if all(abs(y2 - ref2) < tolerance)
        fprintf('  Test 2 PASSED\n');
    else
        fprintf('  Test 2 FAILED\n');
    end

    %% Test 3: random large vector with timing
    n = 10000;
    a3 = single(randn(1, n) + 1i*randn(1, n));
    b3 = single(randn(1, n));
    a3_interleaved = single(zeros(1, 2*n));
    a3_interleaved(1:2:end) = real(a3);
    a3_interleaved(2:2:end) = imag(a3);

    tic;
    y3_interleaved = arm_cmplx_mult_real_f32_mex(a3_interleaved, b3);
    mex_time = toc;

    y3 = y3_interleaved(1:2:end) + 1i*y3_interleaved(2:2:end);
    ref3 = a3 .* b3;

    if all(abs(y3 - ref3) < tolerance)
        fprintf('  Test 3 PASSED (time: %.6f s)\n', mex_time);
    else
        fprintf('  Test 3 FAILED\n');
    end
end