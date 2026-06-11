clear; clc;

%% 编译 MEX
mex cfft_q15_mex.c cfft_q15_simulink.c;
disp('MEX 编译成功');

%% ========== 测试 1：16点脉冲信号 ==========
disp(' ');
disp('========== 测试 1：16点脉冲信号 ==========');
N = 16;
in_pulse = zeros(1, N*2, 'int16');
in_pulse(1) = int16(32767);  % 第一个点=1.0

out_pulse_fft = cfft_q15_mex(N, 0, 1, in_pulse);
disp('脉冲FFT输出（实部）:'); disp(out_pulse_fft(1:2:end));
disp('脉冲FFT输出（虚部）:'); disp(out_pulse_fft(2:2:end));

% 脉冲的DFT应该是全1，即全32767（考虑缩放后）
% 16点FFT，2级Radix-4，每级除以4，总共除以16
% 所以输出应该是 32767/16 ≈ 2048？不对
% 实际上官方实现最后会左移补偿，所以应该是 32767
disp('理论脉冲FFT（实部）: 全 32767');

out_pulse_ifft = cfft_q15_mex(N, 1, 1, out_pulse_fft);
disp('脉冲IFFT还原（实部）:'); disp(out_pulse_ifft(1:2:end));
disp('脉冲IFFT误差:'); disp(int32(out_pulse_ifft) - int32(in_pulse));

%% ========== 测试 2：16点直流信号 ==========
disp(' ');
disp('========== 测试 2：16点直流信号 x=0.5 ==========');
N = 16;
in_dc = zeros(1, N*2, 'int16');
in_dc(1:2:end) = int16(16384);  % 实部=0.5

out_dc_fft = cfft_q15_mex(N, 0, 1, in_dc);
disp('直流FFT输出（实部）:'); disp(out_dc_fft(1:2:end));
disp('直流FFT输出（虚部）:'); disp(out_dc_fft(2:2:end));

% 直流0.5的16点DFT：只有直流分量=0.5*16/缩放
% 2级Radix-4，每级>>2，总共/16，所以直流=16384*16/16=16384
% 其他频率分量=0
disp('理论直流FFT: [16384, 0, 0, ..., 0]');

out_dc_ifft = cfft_q15_mex(N, 1, 1, out_dc_fft);
disp('直流IFFT还原（实部）:'); disp(out_dc_ifft(1:2:end));
disp('直流IFFT误差:'); disp(int32(out_dc_ifft) - int32(in_dc));

%% ========== 测试 3：正弦信号 ==========
disp(' ');
disp('========== 测试 3：单频正弦信号 ==========');
N = 16;
t = (0:N-1);
x = sin(2*pi*1*t/N);  % 1个周期正弦
in_sin = zeros(1, N*2, 'int16');
in_sin(1:2:end) = int16(x * 32767);

out_sin_fft = cfft_q15_mex(N, 0, 1, in_sin);
disp('正弦FFT幅度（实部）:'); disp(out_sin_fft(1:2:end));
disp('正弦FFT幅度（虚部）:'); disp(out_sin_fft(2:2:end));

out_sin_ifft = cfft_q15_mex(N, 1, 1, out_sin_fft);
disp('正弦IFFT还原误差:'); disp(int32(out_sin_ifft) - int32(in_sin));

%% ========== 测试 4：32点脉冲 ==========
disp(' ');
disp('========== 测试 4：32点脉冲信号 ==========');
N = 32;
in_32 = zeros(1, N*2, 'int16');
in_32(1) = int16(32767);

out_32_fft = cfft_q15_mex(N, 0, 1, in_32);
disp('32点脉冲FFT（实部）:'); disp(out_32_fft(1:2:end));

out_32_ifft = cfft_q15_mex(N, 1, 1, out_32_fft);
disp('32点脉冲IFFT误差:'); disp(int32(out_32_ifft) - int32(in_32));

%% ========== 测试 5：随机信号往返精度 ==========
disp(' ');
disp('========== 测试 5：随机信号往返精度 ==========');
N = 16;
rand_input = int16(randi([-32768 32767], 1, N*2));
out_rand_fft = cfft_q15_mex(N, 0, 1, rand_input);
out_rand_ifft = cfft_q15_mex(N, 1, 1, out_rand_fft);

max_err = max(abs(int32(out_rand_ifft) - int32(rand_input)));
disp(['随机信号最大往返误差: ', num2str(max_err)]);
if max_err < 10
    disp('✓ 测试通过！');
else
    disp('✗ 测试失败！');
end