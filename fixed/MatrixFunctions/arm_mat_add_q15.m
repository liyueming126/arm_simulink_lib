function arm_mat_add_q15()
clc;
fprintf('=== Testing arm_mat_add_q15 ===\n\n');
errors = 0;

% Test 1: basic 2x2
A = int16([1 2; 3 4]);
B = int16([10 20; 30 40]);
C = arm_mat_add_q15_mex(A, B);
fprintf('Test 1 - Basic 2x2:\n  A:\n'); disp(A);
fprintf('  B:\n'); disp(B);
fprintf('  C:\n'); disp(C);
ref = int16([11 22; 33 44]);
if isequal(C, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 2: saturation
A = int16([32767 32767; -32768 -32768]);
B = int16([1 32767; -1 -32768]);
C = arm_mat_add_q15_mex(A, B);
fprintf('Test 2 - Saturation:\n  A:\n'); disp(A);
fprintf('  B:\n'); disp(B);
fprintf('  C:\n'); disp(C);
ref = int16([32767 32767; -32768 -32768]);
if isequal(C, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 3: random matrices
rng(42);
A = int16(randi([-32768, 32767], 4, 6, 'int16'));
B = int16(randi([-32768, 32767], 4, 6, 'int16'));
tic; C = arm_mat_add_q15_mex(A, B); t = toc;
fprintf('Test 3 - Random 4x6: %.3f ms\n', t*1000);
s = double(A) + double(B);
ref = int16(max(min(s, 32767), -32768));
if isequal(C, ref)
    fprintf('  PASS\n\n');
else
    mismatches = find(C ~= ref, 5, 'first');
    fprintf('  FAIL at indices: ['); fprintf('%d ', mismatches); fprintf(']\n');
    errors = errors + 1;
end

% Test 4: vector matrices
A = int16([100; 200; 300; 400; 500]);
B = int16([50; -50; 1000; -1000; 0]);
C = arm_mat_add_q15_mex(A, B);
fprintf('Test 4 - Column vectors:\n  C:\n'); disp(C);
ref = int16([150; 150; 1300; -600; 500]);
if isequal(C, ref)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

% Test 5: single element
A = int16(32767);
B = int16(1);
C = arm_mat_add_q15_mex(A, B);
fprintf('Test 5 - Single element: C=%d (expected 32767)\n', C);
if C == int16(32767)
    fprintf('  PASS\n\n');
else
    fprintf('  FAIL\n\n');
    errors = errors + 1;
end

if errors == 0, fprintf('=== All tests PASSED ===\n'); else fprintf('=== %d test(s) FAILED ===\n', errors); end
end