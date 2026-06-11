function arm_pid_reset_f32()
clc;
fprintf('=== Testing arm_pid_reset_f32 ===\n\n');
errors = 0;

% Test 1: reset non-zero state
S_in = single([1.0, 2.0, 3.0, 100.0, 200.0, 300.0, 0.5, 0.1, 0.05]);
S_out = arm_pid_reset_f32_mex(S_in);
fprintf('Test 1 - Reset non-zero state:\n');
fprintf('  In:  [A0=%.2f A1=%.2f A2=%.2f s0=%.2f s1=%.2f s2=%.2f Kp=%.3f Ki=%.3f Kd=%.3f]\n', S_in);
fprintf('  Out: [A0=%.2f A1=%.2f A2=%.2f s0=%.2f s1=%.2f s2=%.2f Kp=%.3f Ki=%.3f Kd=%.3f]\n', S_out);
if S_out(4)==0 && S_out(5)==0 && S_out(6)==0 && all(S_out(1:3)==S_in(1:3)) && all(S_out(7:9)==S_in(7:9))
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: reset already zero state
S_in = single([5.0, 6.0, 7.0, 0, 0, 0, 3.0, 4.0, 5.0]);
S_out = arm_pid_reset_f32_mex(S_in);
fprintf('Test 2 - Reset already zero state:\n');
if all(S_out(4:6)==0) && all(S_out(1:3)==S_in(1:3)) && all(S_out(7:9)==S_in(7:9))
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: negative gain values
S_in = single([1.0, 2.0, 3.0, 5.0, -3.0, 7.0, -0.5, -1.0, -2.0]);
S_out = arm_pid_reset_f32_mex(S_in);
fprintf('Test 3 - Negative gain values:\n');
if S_out(4)==0 && S_out(5)==0 && S_out(6)==0 && all(S_out(7:9)==S_in(7:9))
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end