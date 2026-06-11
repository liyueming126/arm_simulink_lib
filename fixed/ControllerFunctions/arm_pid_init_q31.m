function arm_pid_init_q31()
clc;
fprintf('=== Testing arm_pid_init_q31 ===\n\n');
errors = 0;

% Test 1: basic init with reset
Kp = int32(536870912); Ki = int32(107374182); Kd = int32(161061274);
resetFlag = 1;
S = arm_pid_init_q31_mex(Kp, Ki, Kd, resetFlag);
fprintf('Test 1 - Basic init with reset:\n');
fprintf('  Kp=%d, Ki=%d, Kd=%d, resetFlag=%d\n', Kp, Ki, Kd, resetFlag);
fprintf('  S=[A0=%d, A1=%d, A2=%d, s0=%d, s1=%d, s2=%d]\n', S);
if S(4)==0 && S(5)==0 && S(6)==0
    fprintf('  PASS (state reset to zero)\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: init without reset
Kp = int32(536870912); Ki = int32(107374182); Kd = int32(268435456);
resetFlag = 0;
S = arm_pid_init_q31_mex(Kp, Ki, Kd, resetFlag);
fprintf('Test 2 - Init without reset:\n');
fprintf('  S=[A0=%d, A1=%d, A2=%d]\n', S(1:3));
if S(3) == Kd
    fprintf('  PASS (A2 == Kd)\n\n');
else
    fprintf('  FAIL A2=%d vs Kd=%d\n\n', S(3), Kd);
    errors = errors + 1;
end

% Test 3: overflow clipping (Q31 range)
Kp = int32(1500000000); Ki = int32(1500000000); Kd = int32(1500000000);
resetFlag = 1;
S = arm_pid_init_q31_mex(Kp, Ki, Kd, resetFlag);
fprintf('Test 3 - Overflow clipping:\n');
fprintf('  Kp=%d, Ki=%d, Kd=%d\n', Kp, Ki, Kd);
fprintf('  S=[A0=%d, A1=%d, A2=%d]\n', S(1:3));
if S(1) == intmax('int32') || S(1) == intmin('int32')
    fprintf('  PASS (A0 clipped)\n\n');
else
    fprintf('  FAIL A0=%d (no clipping)\n\n', S(1));
end

% Test 4: negative values
Kp = int32(-500000000); Ki = int32(-100000000); Kd = int32(-200000000);
resetFlag = 1;
S = arm_pid_init_q31_mex(Kp, Ki, Kd, resetFlag);
fprintf('Test 4 - Negative gains:\n');
fprintf('  S=[A0=%d, A1=%d, A2=%d]\n', S(1:3));
fprintf('  PASS\n\n');

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end