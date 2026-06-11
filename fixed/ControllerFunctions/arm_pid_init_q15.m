function arm_pid_init_q15()
clc;
fprintf('=== Testing arm_pid_init_q15 ===\n\n');
errors = 0;

% Test 1: basic init with reset
Kp = int16(1000); Ki = int16(200); Kd = int16(300);
resetFlag = 1;
S = arm_pid_init_q15_mex(Kp, Ki, Kd, resetFlag);
fprintf('Test 1 - Basic init with reset:\n');
fprintf('  Kp=%d, Ki=%d, Kd=%d, resetFlag=%d\n', Kp, Ki, Kd, resetFlag);
fprintf('  S=[A0=%d, A1=%d, A2=%d, s0=%d, s1=%d, s2=%d, Kp=%d, Ki=%d, Kd=%d]\n', S);
if S(4)==0 && S(5)==0 && S(6)==0
    fprintf('  PASS (state reset to zero)\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: init without reset
Kp = int16(1000); Ki = int16(200); Kd = int16(500);
resetFlag = 0;
S = arm_pid_init_q15_mex(Kp, Ki, Kd, resetFlag);
fprintf('Test 2 - Init without reset:\n');
fprintf('  S=[A0=%d, A1=%d, A2=%d, s0=uninitialized, s1=uninitialized, s2=uninitialized]\n', S(1:3));
% Check A0, A1, A2 are computed correctly
expA0 = int16(max(min(double(Kp)+double(Ki)+double(Kd), 32767), -32768));
expA1 = int16(max(min(-(double(Kd)+double(Kd)+double(Kp)), 32767), -32768));
expA2 = Kd;
if S(1) == expA0 && S(2) == expA1 && S(3) == expA2
    fprintf('  PASS (A0=%d A1=%d A2=%d)\n\n', S(1), S(2), S(3));
else
    fprintf('  FAIL expected A0=%d A1=%d A2=%d\n\n', expA0, expA1, expA2);
    errors = errors + 1;
end

% Test 3: saturation
Kp = int16(30000); Ki = int16(5000); Kd = int16(5000);
resetFlag = 1;
S = arm_pid_init_q15_mex(Kp, Ki, Kd, resetFlag);
fprintf('Test 3 - Saturation:\n');
fprintf('  Kp=%d, Ki=%d, Kd=%d\n', Kp, Ki, Kd);
fprintf('  S=[A0=%d, A1=%d, A2=%d]\n', S(1:3));
if S(1) == 32767
    fprintf('  PASS (A0 saturated to 32767)\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: negative saturation
Kp = int16(-30000); Ki = int16(-5000); Kd = int16(-5000);
resetFlag = 1;
S = arm_pid_init_q15_mex(Kp, Ki, Kd, resetFlag);
fprintf('Test 4 - Negative saturation:\n');
if S(1) == -32768
    fprintf('  PASS (A0 saturated to -32768)\n\n');
else
    fprintf('  FAIL A0=%d\n\n', S(1));
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end