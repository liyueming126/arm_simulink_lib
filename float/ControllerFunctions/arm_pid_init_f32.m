function arm_pid_init_f32()
clc;
fprintf('=== Testing arm_pid_init_f32 ===\n\n');
errors = 0;

% Test 1: basic init with reset
Kp = single(2.0); Ki = single(0.5); Kd = single(0.1);
resetFlag = 1;
S = arm_pid_init_f32_mex(Kp, Ki, Kd, resetFlag);
fprintf('Test 1 - Basic init with reset:\n');
fprintf('  Kp=%.2f, Ki=%.2f, Kd=%.2f, resetFlag=%d\n', Kp, Ki, Kd, resetFlag);
fprintf('  S=[A0=%.4f, A1=%.4f, A2=%.4f, s0=%.1f, s1=%.1f, s2=%.1f]\n', S);
if S(4)==0 && S(5)==0 && S(6)==0
    fprintf('  PASS (state reset to zero)\n\n');
else
    fprintf('  FAIL (state not reset)\n\n');
    errors = errors + 1;
end

% Test 2: verify coefficient computation
% A0 = Kp + Ki + Kd = 2.0 + 0.5 + 0.1 = 2.6
% A1 = -Kp - 2*Kd = -2.0 - 0.2 = -2.2
% A2 = Kd = 0.1
expected_A0 = single(2.6);
expected_A1 = single(-2.2);
expected_A2 = single(0.1);
fprintf('Test 2 - Coefficient computation:\n');
fprintf('  Expected A0=%.4f, A1=%.4f, A2=%.4f\n', expected_A0, expected_A1, expected_A2);
fprintf('  Got      A0=%.4f, A1=%.4f, A2=%.4f\n', S(1), S(2), S(3));
if abs(S(1) - expected_A0) < 1e-6 && abs(S(2) - expected_A1) < 1e-6 && abs(S(3) - expected_A2) < 1e-6
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: init without reset, state should preserve previous values
Kp = single(1.0); Ki = single(0.2); Kd = single(0.05);
resetFlag = 0;
S2 = arm_pid_init_f32_mex(Kp, Ki, Kd, resetFlag);
fprintf('Test 3 - Init without reset:\n');
fprintf('  S=[A0=%.4f, A1=%.4f, A2=%.4f]\n', S2(1), S2(2), S2(3));
if S2(3) == Kd
    fprintf('  PASS (A2 == Kd)\n\n');
else
    fprintf('  FAIL A2=%.4f vs Kd=%.4f\n\n', S2(3), Kd);
    errors = errors + 1;
end

% Test 4: negative gains
Kp = single(-1.5); Ki = single(-0.3); Kd = single(-0.2);
resetFlag = 1;
S3 = arm_pid_init_f32_mex(Kp, Ki, Kd, resetFlag);
expected_A0 = single(-2.0);  % -1.5 + (-0.3) + (-0.2) = -2.0
expected_A1 = single(1.9);   % -(-1.5) - 2*(-0.2) = 1.5 + 0.4 = 1.9
fprintf('Test 4 - Negative gains:\n');
fprintf('  A0=%.4f (expected %.4f), A1=%.4f (expected %.4f)\n', S3(1), expected_A0, S3(2), expected_A1);
if abs(S3(1) - expected_A0) < 1e-6 && abs(S3(2) - expected_A1) < 1e-6
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end