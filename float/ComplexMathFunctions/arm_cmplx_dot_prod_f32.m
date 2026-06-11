function arm_cmplx_dot_prod_f32
    fprintf('Testing arm_cmplx_dot_prod_f32...\n');
    tolerance = single(1e-6);

    %% Test 1: basic values
    a1 = single([1+2i, 3+4i]);
    b1 = single([5+6i, 7+8i]);
    N1 = length(a1);
    a1_interleaved = single(zeros(1, 2*N1));
    a1_interleaved(1:2:end) = real(a1);
    a1_interleaved(2:2:end) = imag(a1);
    b1_interleaved = single(zeros(1, 2*N1));
    b1_interleaved(1:2:end) = real(b1);
    b1_interleaved(2:2:end) = imag(b1);

    [realOut, imagOut] = arm_cmplx_dot_prod_f32_mex(a1_interleaved, b1_interleaved);
    ref = sum(a1 .* conj(b1));
    % dot = sum(a .* conj(b)) gives sum(real(a)*real(b) + imag(a)*imag(b) + j*(imag(a)*real(b) - real(a)*imag(b)))
    % But we want a1*real(b1) - a1*imag(b1) + j*(a1*imag(b1) + a1*real(b1))
    % Actually the complex dot product: sum(a[i] * b[i]) where both are complex
    % = sum( (ar+j*ai)*(br+j*bi) ) = sum( (ar*br - ai*bi) + j*(ar*bi + ai*br) )
    ref_real = sum(real(a1).*real(b1) - imag(a1).*imag(b1));
    ref_imag = sum(real(a1).*imag(b1) + imag(a1).*real(b1));

    if abs(realOut - ref_real) < tolerance && abs(imagOut - ref_imag) < tolerance
        fprintf('  Test 1 PASSED\n');
    else
        fprintf('  Test 1 FAILED (real: %f vs %f, imag: %f vs %f)\n', ...
            realOut, ref_real, imagOut, ref_imag);
    end

    %% Test 2: edge values (zeros, negatives)
    a2 = single([0+0i, -1+0i, 0-2i]);
    b2 = single([0+0i, 3+0i, 0+4i]);
    N2 = length(a2);
    a2_interleaved = single(zeros(1, 2*N2));
    a2_interleaved(1:2:end) = real(a2);
    a2_interleaved(2:2:end) = imag(a2);
    b2_interleaved = single(zeros(1, 2*N2));
    b2_interleaved(1:2:end) = real(b2);
    b2_interleaved(2:2:end) = imag(b2);

    [realOut2, imagOut2] = arm_cmplx_dot_prod_f32_mex(a2_interleaved, b2_interleaved);
    ref_real2 = sum(real(a2).*real(b2) - imag(a2).*imag(b2));
    ref_imag2 = sum(real(a2).*imag(b2) + imag(a2).*real(b2));

    if abs(realOut2 - ref_real2) < tolerance && abs(imagOut2 - ref_imag2) < tolerance
        fprintf('  Test 2 PASSED\n');
    else
        fprintf('  Test 2 FAILED\n');
    end

    %% Test 3: random large vector with timing
    n = 10000;
    a3 = single(randn(1, n) + 1i*randn(1, n));
    b3 = single(randn(1, n) + 1i*randn(1, n));
    a3_interleaved = single(zeros(1, 2*n));
    a3_interleaved(1:2:end) = real(a3);
    a3_interleaved(2:2:end) = imag(a3);
    b3_interleaved = single(zeros(1, 2*n));
    b3_interleaved(1:2:end) = real(b3);
    b3_interleaved(2:2:end) = imag(b3);

    tic;
    [realOut3, imagOut3] = arm_cmplx_dot_prod_f32_mex(a3_interleaved, b3_interleaved);
    mex_time = toc;

    ref_real3 = sum(real(a3).*real(b3) - imag(a3).*imag(b3));
    ref_imag3 = sum(real(a3).*imag(b3) + imag(a3).*real(b3));

    if abs(realOut3 - ref_real3) < tolerance && abs(imagOut3 - ref_imag3) < tolerance
        fprintf('  Test 3 PASSED (time: %.6f s)\n', mex_time);
    else
        fprintf('  Test 3 FAILED\n');
    end
end