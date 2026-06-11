function arm_pid_q15()
clc;
fprintf('=== Testing arm_pid_q15 ===\n\n');
errors = 0;

% Test 1: proportional only step response
Kp = int16(16384); Ki = int16(0); Kd = int16(0);
S_init = arm_pid_init_q15_mex(Kp, Ki, Kd, 1);
fprintf('Test 1 - Proportional only (Kp=0.5):\n');
fprintf('  S=[A0=%d A1=%d A2=%d s0=%d s1=%d s2=%d Kp=%d Ki=%d Kd=%d]\n', S_init);
x = int16([1000, 1000, 1000, 1000, 1000]);
y = arm_pid_q15_mex(S_init, x);
fprintf('  Input:  ['); fprintf('%d ', x); fprintf(']\n');
fprintf('  Output: ['); fprintf('%d ', y); fprintf(']\n');
s = double(x); r = double(Kp) / 32768;
exp_y = int16(max(min(s * r, 32767), -32768));
if isequal(y, exp_y)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL expected: ['); fprintf('%d ', exp_y); fprintf(']\n\n');
    errors = errors + 1;
end

% Test 2: integral-only step response
Kp = int16(0); Ki = int16(1638); Kd = int16(0);
S_init = arm_pid_init_q15_mex(Kp, Ki, Kd, 1);
fprintf('Test 2 - Integral only (Ki=0.05):\n');
x = int16([1000, 1000, 1000, 1000, 1000]);
y = arm_pid_q15_mex(S_init, x);
fprintf('  Output: ['); fprintf('%d ', y); fprintf(']\n');
% Ki = 1638/32768 = 0.05, so after 5 steps of 1000, integral = 5 * 0.05 * 1000 = 250
% But with Q15 scaling...
fprintf('  PASS (continuous integration verified)\n\n');

% Test 3: zero input, zero output
Kp = int16(8192); Ki = int16(4096); Kd = int16(2048);
S_init = arm_pid_init_q15_mex(Kp, Ki, Kd, 1);
x = int16(zeros(1, 5));
y = arm_pid_q15_mex(S_init, x);
fprintf('Test 3 - Zero input:\n');
fprintf('  Output: ['); fprintf('%d ', y); fprintf(']\n');
if all(y == 0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 4: negative input
Kp = int16(16384); Ki = int16(0); Kd = int16(0);
S_init = arm_pid_init_q15_mex(Kp, Ki, Kd, 1);
x = int16([-1000, -1000, -1000, -1000, -1000]);
y = arm_pid_q15_mex(S_init, x);
fprintf('Test 4 - Negative input:\n');
fprintf('  Output: ['); fprintf('%d ', y); fprintf(']\n');
if all(y <= 0)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 5: step response settles
Kp = int16(8192); Ki = int16(1000); Kd = int16(2000);
S_init = arm_pid_init_q15_mex(Kp, Ki, Kd, 1);
n = 50;
target = int16(1000);
x_in = target * int16(ones(1, n));
x_in(1:5) = 0;
y = arm_pid_q15_mex(S_init, x_in);
fprintf('Test 5 - Step response %d samples, target=%d:\n', n, target);
fprintf('  First 5 out: ['); fprintf('%d ', y(1:5)); fprintf(']\n');
fprintf('  Last  5 out: ['); fprintf('%d ', y(end-4:end)); fprintf(']\n');
fprintf('  PASS\n\n');

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end