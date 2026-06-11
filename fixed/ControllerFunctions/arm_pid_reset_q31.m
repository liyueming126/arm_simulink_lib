function arm_pid_reset_q31()
clc;
fprintf('=== Testing arm_pid_reset_q31 ===\n\n');
errors = 0;

% Test 1: reset non-zero state
S_in = int32([100000 200000 300000 1 2 3 1000000 2000000 3000000]);
S_out = arm_pid_reset_q31_mex(S_in);
fprintf('Test 1 - Reset non-zero state:\n');
fprintf('  In:  [A0=%d A1=%d A2=%d s0=%d s1=%d s2=%d Kp=%d Ki=%d Kd=%d]\n', S_in);
fprintf('  Out: [A0=%d A1=%d A2=%d s0=%d s1=%d s2=%d Kp=%d Ki=%d Kd=%d]\n', S_out);
if S_out(4)==0 && S_out(5)==0 && S_out(6)==0 && all(S_out(1:3)==S_in(1:3)) && all(S_out(7:9)==S_in(7:9))
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: reset already zero state
S_in = int32([500000 600000 700000 0 0 0 3000000 4000000 5000000]);
S_out = arm_pid_reset_q31_mex(S_in);
fprintf('Test 2 - Reset already zero state:\n');
if all(S_out(4:6)==0) && all(S_out(1:3)==S_in(1:3)) && all(S_out(7:9)==S_in(7:9))
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: negative gain values
S_in = int32([1000000 2000000 3000000 5 -3 7 -500000 -1000000 -2000000]);
S_out = arm_pid_reset_q31_mex(S_in);
fprintf('Test 3 - Negative gain values:\n');
if S_out(4)==0 && S_out(5)==0 && S_out(6)==0 && all(S_out(7:9)==S_in(7:9))
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end