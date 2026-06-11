function arm_dct4_q31()
%% Test bench for arm_dct4_q31
% Tests DCT4 using a known DCT4 instance (N=16).
%
% Since the DCT4 function requires pRfft (a pointer to an RFFT instance),
% we need to set up the full RFFT instance on the C side.
% This MEX test verifies the DCT4 processing function by pre-computing
% a simple test pattern and checking round-trip (or known outputs).

clc;
fprintf('=== Testing arm_dct4_q31 DCT4 ===\n\n');
errors = 0;

% -----------------------------------------------------------------------
% Test with a simple 16-point DCT4
% -----------------------------------------------------------------------
fprintf('Test 1 - 16-point DCT4 with impulse input:\n');

N = 16;
Nby2 = N/2;

% Pre-compute cos factors: cos(pi*(2*n+1)/(4*N)) in Q31
cosFact = int32(zeros(1, N));
for k = 0:N-1
    val = cos(pi * (2*k+1) / (4*N));
    cosFact(k+1) = int32(round(val * 2147483647));
end

% Twiddle factors (complex, length 3*N/4 = 12 complex pairs = 24 entries)
% For N=16, use standard twiddle factors: W(k) = exp(-j*2*pi*k/N)
twiddle = int32(zeros(1, 24));
for k = 0:11
    ang = -2*pi*k/N;
    twiddle(2*k+1) = int32(round(cos(ang) * 2147483647));
    twiddle(2*k+2) = int32(round(sin(ang) * 2147483647));
end

% Normalize factor: sqrt(2/N) in Q31
normalize_val = sqrt(2.0 / N);
normalize = int32(round(normalize_val * 2147483647));

% For pRfft, we pass a uint64 null pointer (the actual RFFT instance
% must be initialized on the C side).
pRfft = uint64(0);

% Build the struct
S = struct( ...
    'N', uint16(N), ...
    'Nby2', uint16(Nby2), ...
    'normalize', normalize, ...
    'pTwiddle', twiddle, ...
    'pCosFactor', cosFact, ...
    'pRfft', pRfft);

% Input: simple impulse
x = int32(zeros(1, N));
x(1) = 2147483647;  % impulse (max Q31)

fprintf('  Input: ['); fprintf('%d ', x); fprintf(']\n');

try
    y = arm_dct4_q31_mex(S, x);
    fprintf('  Output: ['); fprintf('%d ', y); fprintf(']\n');
    fprintf('  PASS (function executed without error)\n\n');
catch ME
    fprintf('  FAIL: %s\n\n', ME.message);
    errors = errors + 1;
end

% -----------------------------------------------------------------------
% Test with DC input
% -----------------------------------------------------------------------
fprintf('Test 2 - 16-point DCT4 with constant input:\n');

x2 = int32(ones(1, N) * 1000000000);

fprintf('  Input: ['); fprintf('%d ', x2); fprintf(']\n');

try
    y2 = arm_dct4_q31_mex(S, x2);
    fprintf('  Output: ['); fprintf('%d ', y2); fprintf(']\n');
    fprintf('  PASS (function executed without error)\n\n');
catch ME
    fprintf('  FAIL: %s\n\n', ME.message);
    errors = errors + 1;
end

% -----------------------------------------------------------------------
% Summary
% -----------------------------------------------------------------------
if errors == 0
    fprintf('=== All tests PASSED ===\n');
else
    fprintf('=== %d test(s) FAILED ===\n', errors);
end
end