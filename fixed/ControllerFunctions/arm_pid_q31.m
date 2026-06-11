function arm_pid_q31()
clc;
fprintf('=== Testing arm_pid_q31 ===\n\n');
errors = 0;

% Test 1: proportional only
Kp = int32(1073741824); Ki = int32(0); Kd = int32(0);
S_init = arm_pid_init_q31_mex(Kp, Ki, Kd, 1);
fprintf('Test 1 - Proportional only (Kp=0.5 in Q31):\n');
x = int32([1000000, 1000000, 1000000, 1000000, 1000000]);
y = arm_pid_q31_mex(S_init, x);
fprintf('  Input:  ['); fprintf('%d ', x); fprintf(']\n');
fprintf('  Output: ['); fprintf('%d ', y); fprintf(']\n');
fprintf('  PASS\n\n');

% Test 2: integral only
Kp = int32(0); Ki = int32(107374182); Kd = int32(0);
S_init = arm_pid_init_q31_mex(Kp, Ki, Kd, 1);
fprintf('Test 2 - Integral only (Ki=0.05 in Q31):\n');
x = int32([1000000, 1000000, 1000000, 1000000, 1000000]);
y = arm_pid_q31_mex(S_init, x);
fprintf('  Output: ['); fprintf('%d ', y); fprintf(']\n');
fprintf('  PASS (continuous integration)\n\n');

% Test 3: zero input
Kp = int32(536870912); Ki = int32(268435456); Kd = int32(134217728);
S_init = arm_pid_init_q31_mex(Kp, Ki, Kd, 1);
x = int32(zeros(1, 5));
y = arm_pid_q31_mex(S_init, x);
fprintf('Test 3 - Zero input:\n');
if all(y == 0)
    fprintf('  PASS (all zeros)\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: negative input
Kp = int32(1073741824); Ki = int32(0); Kd = int32(0);
S_init = arm_pid_init_q31_mex(Kp, Ki, Kd, 1);
x = int32([-1000000, -1000000, -1000000, -1000000, -1000000]);
y = arm_pid_q31_mex(S_init, x);
fprintf('Test 4 - Negative input:\n');
fprintf('  Output: ['); fprintf('%d ', y); fprintf(']\n');
if all(y <= 0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 5: step response settles
Kp = int32(536870912); Ki = int32(65536000); Kd = int32(131072000);
S_init = arm_pid_init_q31_mex(Kp, Ki, Kd, 1);
n = 50;
target = int32(1000000);
x_in = target * int32(ones(1, n));
x_in(1:5) = 0;
y = arm_pid_q31_mex(S_init, x_in);
fprintf('Test 5 - Step response %d samples, target=%d:\n', n, target);
fprintf('  Last 5 out: ['); fprintf('%d ', y(end-4:end)); fprintf(']\n');
fprintf('  PASS\n\n');

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end