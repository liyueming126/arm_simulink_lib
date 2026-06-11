# CMSIS-DSP TransformFunctions 分析报告 — 面向 Simulink 移植

> **来源**: CMSIS 4.5.0 / DSP_Lib / Source / TransformFunctions  
> **目的**: 详细分析所有变换函数（CFFT/RFFT/DCT4）的算法、数据流、依赖关系及 ARM 专有优化，为移植到 Simulink（MATLAB）平台提供参考

---

## 1. 模块概览

TransformFunctions 实现三类核心变换：

| 类别 | 功能 | 数据类型 | 核心文件 |
|------|------|----------|----------|
| **CFFT** | 复数 FFT/IFFT | f32 / q31 / q15 | `arm_cfft_*.c` |
| **RFFT** | 实数 FFT/IFFT | f32 / q31 / q15 | `arm_rfft_*.c` |
| **DCT4** | 离散余弦变换 Type-IV | f32 / q31 / q15 | `arm_dct4_*.c` |

**架构分层**（新 API，推荐使用）:

```
用户层 API:   arm_cfft_f32/q31/q15   arm_rfft_fast_f32   arm_dct4_f32/q31/q15
                    |                      |                    |
中间层:        arm_radix8_butterfly    stage_rfft/merge     arm_rfft (内部)
                    |                      |                    |
底层:         arm_bitreversal_32/16    arm_cfft_f32         arm_cfft_radix4_*
              (汇编/C 实现)             (混合基 CFFT)        (已废弃，向后兼容)
```

---

## 2. 文件清单与角色

### 2.1 CFFT — 复数 FFT（核心）

| 文件 | 角色 | 关键函数 | 说明 |
|------|------|----------|------|
| `arm_cfft_f32.c` | **CFFT 顶层 f32** | `arm_cfft_f32` | **推荐入口**。混合基算法（radix-8 + radix-2/4 回退），支持长度 16/32/64/128/256/512/1024/2048/4096 |
| `arm_cfft_q31.c` | **CFFT 顶层 q31** | `arm_cfft_q31` | 同上，Q31 定点 |
| `arm_cfft_q15.c` | **CFFT 顶层 q15** | `arm_cfft_q15` | 同上，Q15 定点 |
| `arm_cfft_radix8_f32.c` | Radix-8 蝶形 f32 | `arm_radix8_butterfly_f32` | 一次完成 8 路 DIF 蝶形。可直接处理 64/512/4096 点；32/256/2048 需 2 路 radix-8；16/128/1024 需 4 路 radix-8 |
| `arm_cfft_radix2_f32.c` | Radix-2 蝶形 f32 | `arm_radix2_butterfly_f32/inverse` | **已废弃**，作为 `arm_cfft_radix8by2_f32` 内部调用 |
| `arm_cfft_radix4_f32.c` | Radix-4 蝶形 f32 | `arm_radix4_butterfly_f32/inverse` | **已废弃**，作为 `arm_cfft_radix8by4_f32` 内部调用 |
| `arm_cfft_radix4_q31.c` | Radix-4 蝶形 q31 | `arm_radix4_butterfly_q31/inverse` | **已废弃**，arm_cfft_q31 内部调用 |
| `arm_cfft_radix4_q15.c` | Radix-4 蝶形 q15 | `arm_radix4_butterfly_q15/inverse` | **已废弃**，arm_cfft_q15 内部调用 |
| `arm_cfft_radix2_q15.c` | Radix-2 蝶形 q15 | `arm_radix2_butterfly_q15/inverse` | **已废弃** |
| `arm_cfft_radix2_q31.c` | Radix-2 蝶形 q31 | `arm_radix2_butterfly_q31/inverse` | **已废弃** |

### 2.2 位反序（Bit-Reversal）

| 文件 | 角色 | 关键函数 | 说明 |
|------|------|----------|------|
| `arm_bitreversal.c` | **C 实现** | `arm_bitreversal_f32/q31/q15` | 纯 C，便携但速度慢。新 API 不再调用 |
| `arm_bitreversal2.S` | **汇编实现** | `arm_bitreversal_32/16` | ARM Thumb-2 汇编优化。**新 API 调用的版本**，f32/q31 用 `_32`、q15 用 `_16` |

### 2.3 RFFT — 实数 FFT

| 文件 | 角色 | 关键函数 | 说明 |
|------|------|----------|------|
| `arm_rfft_fast_f32.c` | **推荐 RFFT f32** | `arm_rfft_fast_f32` | 新 API，内部使用 `arm_cfft_f32`（混合基），性能更优 |
| `arm_rfft_f32.c` | 旧版 RFFT f32 | `arm_rfft_f32` | **已废弃**。内部使用 radix-4 CFFT，需要 A/B 两组 twiddle 表 |
| `arm_rfft_q15.c` | RFFT q15 | `arm_rfft_q15` | 旧版 API，内部调用 `arm_cfft_q15` |
| `arm_rfft_q31.c` | RFFT q31 | `arm_rfft_q31` | 旧版 API，内部调用 `arm_cfft_q31` |

### 2.4 DCT4 — 离散余弦变换 Type-IV

| 文件 | 角色 | 关键函数 | 说明 |
|------|------|----------|------|
| `arm_dct4_f32.c` | DCT4 f32 | `arm_dct4_f32` | 通过 RFFT 实现 DCT4。4 步：预处理→重排→RFFT→后处理 |
| `arm_dct4_q15.c` | DCT4 q15 | `arm_dct4_q15` | Q15 定点，增加 format 转换移位操作 |
| `arm_dct4_q31.c` | DCT4 q31 | `arm_dct4_q31` | Q31 定点，使用 64 位中间累加器避免溢出 |

### 2.5 Init 文件（参考用，本次不深入）

| 文件组 | 用途 |
|--------|------|
| `arm_cfft_radix2_init_*.c` | 旧 API 的 radix-2 实例初始化（设置 twiddle 表指针、位反序表等） |
| `arm_cfft_radix4_init_*.c` | 旧 API 的 radix-4 实例初始化 |
| `arm_rfft_init_*.c` | 旧 API 的 RFFT 实例初始化 |
| `arm_rfft_fast_init_f32.c` | 新 API 的 `arm_rfft_fast_instance_f32` 初始化 |
| `arm_dct4_init_*.c` | DCT4 实例初始化（生成 cos factor 表、twiddle 权值表） |

---

## 3. 核心数据结构

### 3.1 `arm_cfft_instance_f32`（新 API）

```c
typedef struct {
    uint16_t fftLen;              // FFT 长度 (16/32/64/128/256/512/1024/2048/4096)
    const float32_t *pTwiddle;    // 旋转因子表指针（复系数交错存储: cos, sin, cos, sin, ...）
    const uint16_t *pBitRevTable; // 位反序查找表
    uint16_t bitRevLength;        // 位反序表长度
} arm_cfft_instance_f32;
```

Q31/Q15 版本结构相同，仅 `pTwiddle` 数据类型不同。

### 3.2 `arm_rfft_fast_instance_f32`（新 API）

```c
typedef struct {
    arm_cfft_instance_f32 Sint;   // 内部 CFFT 实例（半长复数 FFT）
    uint16_t fftLenRFFT;          // 实数序列长度
    float32_t *pTwiddleRFFT;      // RFFT 专用旋转因子
} arm_rfft_fast_instance_f32;
```

### 3.3 预定义的实例常量

由 `arm_const_structs.h` 声明，在 `arm_const_structs.c` 中定义：

```c
extern const arm_cfft_instance_f32 arm_cfft_sR_f32_len16;
extern const arm_cfft_instance_f32 arm_cfft_sR_f32_len32;
// ... 一直到 len4096，每种长度一个预定义实例
```

用户调用 `arm_cfft_f32` 时直接传入对应长度的实例常量，无需手动初始化。

---

## 4. 算法详解

### 4.1 CFFT — 混合基复数 FFT

#### 4.1.1 `arm_cfft_f32` — 执行流程

```
arm_cfft_f32(S, pData, ifftFlag, bitReverseFlag)
  │
  ├─ if (ifftFlag) 共轭输入（虚部取反）
  │
  ├─ 根据 fftLen 分发:
  │   ├─ 16/128/1024  → arm_cfft_radix8by2_f32()  // 2 路 radix-8 + 1 路 radix-2
  │   ├─ 32/256/2048  → arm_cfft_radix8by4_f32()  // 4 路 radix-8 + 1 路 radix-4
  │   └─ 64/512/4096  → arm_radix8_butterfly_f32() // 纯 radix-8 一次覆盖
  │
  ├─ if (bitReverseFlag) arm_bitreversal_32()       // 汇编位反序
  │
  └─ if (ifftFlag) 共轭输出并缩放 × 1/fftLen
```

#### 4.1.2 Radix-8 蝶形（`arm_radix8_butterfly_f32`）

- **DIF（频域抽取）结构**
- 每级将 N 点 FFT 分解为 8 个 N/8 子变换
- 8 个输入 → 8 个输出，每个输出乘以对应的旋转因子（co1/si1 到 co8/si8，其中 co1=1, si1=0）
- 涉及 45° 常数 `C81 = 0.70710678118f`（sqrt(2)/2）
- 级数循环：`do...while(n2 > 7)`，每轮 `n2 >>= 3`

#### 4.1.3 Radix-4 蝶形（旧 API）

4 输入 4 输出的 DIF 结构。核心计算：

```
输入: xa+jya, xb+jyb, xc+jyc, xd+jyd (索引间距 N/4)
                    │
     ┌──────────────┼──────────────┐
     │              │              │
  输出0: (xa+xc)+(xb+xd)    输出1: (xa-xc+jyb-yd)*W1
  输出2: (xa+xc-xb-xd)*W2   输出3: (xa-xc-jyb+yd)*W3
```

#### 4.1.4 Radix-2 蝶形（旧 API）

- 最简 DIF 结构，每级处理 2 输入
- 首级做 `>>1` 防溢出（定点版本），末级无旋转因子
- `log2(N)` 级

### 4.2 定点 CFFT 缩放策略

| 类型 | 每级缩放 | 原因 |
|------|----------|------|
| **f32** | 无缩放 | 浮点格式，动态范围足够 |
| **q31** | 每级 radix-4 后右移 2 bit（÷4） | 防溢出。16 点输入 1.31 → 输出 5.27 |
| **q15** | 每级 radix-4 后右移 2 bit（÷4） | 防溢出。使用 `__SHADD16` SIMD 指令 |

重要：Q31/Q15 的最终输出比真实值小了 `(fftLen)` 倍，用户需自行左移补偿（或使用 IFFT 时由函数自动处理）。

### 4.3 RFFT — 实数 FFT

#### 4.3.1 `arm_rfft_fast_f32`（推荐新 API）

核心技巧：**N 点实数序列 → 视为 N/2 点复数序列**（偶数为实部，奇数为虚部）→ CFFT → 后处理重构完整频谱

```
arm_rfft_fast_f32(S, p, pOut, ifftFlag)
  │
  ├─ 前向 (ifftFlag=0):
  │   1. p[0..N-1] 实数 → 视为 N/2 复数（p[2k]+j*p[2k+1]）
  │   2. arm_cfft_f32(S->Sint, p, 0, 0)  → 半长复数 FFT
  │   3. stage_rfft_f32() → 频谱重构（合并对称部分）
  │      XA(l) = 0.5*(XA(l)+XB(l) + TW(l)*(XB(l)-XA(l)))
  │   4. 输出 pOut[0]=DC, pOut[1]=Nyquist, pOut[2..N-1]=复频谱
  │
  └─ 反向 (ifftFlag=1):
      1. merge_rfft_f32() → 从 packed 频谱还原复数 CFFT 输入
      2. arm_cfft_f32(S->Sint, p, 1, 0)  → IFFT
      3. 输出 N 点实数序列
```

#### 4.3.2 `arm_rfft_f32`（旧 API）

- 内部调用 `arm_cfft_radix4_f32`（已废弃的 radix-4 CFFT）
- 需要独立的 A/B 两组 twiddle 表
- 不再推荐使用

### 4.4 DCT4 — 离散余弦变换 Type-IV

#### 4.4.1 算法原理

DCT4 通过 RFFT 间接计算，共 4 步：

```
x[n] (输入 N 点实数)
  │
  1. 预处理: r[n] = 2 × x[n] × cos(π(2n+1)/(4N))
  │
  2. 重排: r[even] → 前半, r[odd] 逆序 → 后半（构成 N 点实数序列给 RFFT）
  │
  3. DCT2 计算:
     a. RFFT(pInlineBuffer, pState)   // N 点实数 FFT
     b. 复数乘旋转因子                  // pState *= weights
  │
  4. 后处理: DCT4 递推
     Y4(0) = Y2(0)/2
     Y4(k) = Y2(k) - Y4(k-1)           // k = 1..N-1
  │
  5. 归一化: × sqrt(2/N)
  │
  Y[n] (输出 N 点 DCT4)
```

#### 4.4.2 定点注意事项

| 类型 | 特殊处理 |
|------|----------|
| **f32** | 直接浮点运算，无溢出问题 |
| **q15** | 复数乘后 3.13 → 左移 2 → 1.15；归一化使用 32 位中间量 (q31_t) |
| **q31** | **输入需先右移 1 bit** 防饱和；归一化使用 64 位中间量 (q63_t) |

---

## 5. ARM 专有优化总结（移植需替换部分）

### 5.1 汇编级优化

| 文件 | 优化 | 替换方案（Simulink/MATLAB） |
|------|------|-----------------------------|
| `arm_bitreversal2.S` | Thumb-2 汇编位反序 | MATLAB `bitrevorder()` 或手动索引交换 |
| `arm_bitreversal_32` | 循环展开 ×2，寄存器优化 | 纯 C/MATLAB 实现即可 |

### 5.2 SIMD DSP  intrinsics（Q15 定点）

Cortex-M3/M4 特有的 SIMD 指令，在 `arm_cfft_q15.c`、`arm_cfft_radix2_q15.c`、`arm_cfft_radix4_q15.c`、`arm_rfft_q15.c` 中使用：

| 内联函数 | ARM 指令 | 功能 | Simulink 替代 |
|----------|----------|------|---------------|
| `__SHADD16(a, b)` | SHADD16 | 半字并行 halving 加法 | `bitsra(a+b, 1)` 向量化 |
| `__QADD16(a, b)` | QADD16 | 半字并行饱和加法 | 循环+饱和判断 |
| `__QSUB16(a, b)` | QSUB16 | 半字并行饱和减法 | 循环+饱和判断 |
| `__SMUAD(a, b)` | SMUAD | 双 16 位乘加 → 32 位 | `a(1)*b(1) + a(2)*b(2)` |
| `__SMUSD(a, b)` | SMUSD | 双 16 位乘减 | `a(1)*b(1) - a(2)*b(2)` |
| `__SMUSDX(a, b)` | SMUSDX | 双 16 位交叉乘减 | `a(1)*b(2) - a(2)*b(1)` |
| `__SMUADX(a, b)` | SMUADX | 双 16 位交叉乘加 | `a(1)*b(2) + a(2)*b(1)` |
| `__PKHBT(a, b)` | PKHBT | 打包半字 | `[b[0], a[0]]` |
| `_SIMD32_OFFSET()` | LDRD | 32 位打包加载 2×q15 | 直接索引访问 |
| `__SIMD32(addr)` | LDRD | 同上 | 直接索引访问 |

### 5.3 饱和与乘累加宏（Q31 定点）

| 宏 | ARM 指令 | 功能 |
|----|----------|------|
| `mult_32x32_keep32_R(a,x,y)` | SMMULR | `a = (x*y + 0x80000000) >> 32` |
| `multAcc_32x32_keep32_R(a,x,y)` | SMMLAR | `a = ((a<<32) + x*y + 0x80000000) >> 32` |
| `multSub_32x32_keep32_R(a,x,y)` | SMMLSR | `a = ((a<<32) - x*y + 0x80000000) >> 32` |

Simulink 替代：MATLAB `int32` 运算 + `bitsra` + 四舍五入（`0x80000000` 等价于加 0.5 后右移）。

### 5.4 条件编译路径

```c
#ifndef ARM_MATH_CM0_FAMILY  →  Cortex-M3/M4/M7 优化路径（SIMD、循环展开）
#else                         →  Cortex-M0 标量路径（纯 C 回退）
```

移植时：**直接采用 Cortex-M0 路径语义**（它是标准 C，无平台依赖），或根据 Simulink 生成的代码目标选择等价实现。

### 5.5 循环展开

Cortex-M3/M4 路径大量使用 **4 倍循环展开**（`do { ... } while(i >>= 2)` + 尾部剩余处理），减少循环开销。Simulink MATLAB 代码无需手工展开（MATLAB JIT 自动处理）。

---

## 6. 函数调用依赖图

### 6.1 CFFT 调用链（新 API）

```
arm_cfft_f32
  ├── arm_cfft_radix8by2_f32    (内部静态函数，位于 arm_cfft_f32.c)
  │     ├── arm_radix8_butterfly_f32    (arm_cfft_radix8_f32.c)
  │     └── arm_radix2_butterfly_f32    (arm_cfft_radix2_f32.c)
  ├── arm_cfft_radix8by4_f32    (内部静态函数，位于 arm_cfft_f32.c)
  │     ├── arm_radix8_butterfly_f32    (arm_cfft_radix8_f32.c)
  │     └── arm_radix4_butterfly_f32    (arm_cfft_radix4_f32.c)
  ├── arm_radix8_butterfly_f32  (arm_cfft_radix8_f32.c)
  └── arm_bitreversal_32        (arm_bitreversal2.S) [汇编]
```

Q31 版本类似，仅调用的 radix-4 蝶形为 `arm_cfft_radix4_q31.c` 版本。

### 6.2 RFFT 调用链

```
arm_rfft_fast_f32
  ├── arm_cfft_f32               (arm_cfft_f32.c) [混合基 CFFT]
  ├── stage_rfft_f32             (arm_rfft_fast_f32.c, 内部静态)
  └── merge_rfft_f32             (arm_rfft_fast_f32.c, 内部静态)

arm_dct4_f32
  ├── arm_scale_f32              (其他模块: BasicMathFunctions)
  ├── arm_mult_f32               (其他模块: BasicMathFunctions)
  ├── arm_rfft_f32               (旧 RFFT)
  ├── arm_cmplx_mult_cmplx_f32   (其他模块: ComplexMathFunctions)
  └── 内部后处理递推
```

---

## 7. 旋转因子与查找表

### 7.1 Twiddle 因子表结构

CFFT 的 twiddle 因子采用 **交错存储**：

```
twiddleCoef_4096[8192] = {
  cos(0),  sin(0),     // 第 1 个复数因子
  cos(2π/4096), sin(2π/4096), // 第 2 个
  ...                      // 共 4096 个复数对
}
```

各长度 twiddle 表的关系：**短表是长表的前缀**。因此 `arm_cfft_sR_f32_len16` 的 `pTwiddle` 可以指向 `twiddleCoef_4096` 的起始位置。

### 7.2 位反序表

位反序表存储的是 **字节偏移量**（不是索引），因此可以直接用于指针运算。表内格式：

```c
armBitRevIndexTable4096[4032] = { 0, 8, 4, 12, 2, 10, 6, 14, ... };
```

每个条目 = `index * 8`（因复数含实部+虚部各 4 字节 f32），步长方向由 `bitRevFactor` 控制。

### 7.3 RFFT 专用表

RFFT 需要额外的 A/B 两组 twiddle 表（`pTwiddleAReal` / `pTwiddleBReal`），用于后处理的频谱重构。DCT4 还需要 `pCosFactor` 表（余弦预处理因子）和额外的复 twiddle 权重表。

---

## 8. 面向 Simulink 移植的关键建议

### 8.1 推荐策略

| 优先级 | 策略 | 适用场景 |
|--------|------|----------|
| **1** | 直接使用 MATLAB 内置 FFT/DCT 函数 | Simulink 模型中使用 `fft()`/`ifft()`/`dct()` 等，性能最优 |
| **2** | 按 Cortex-M0 路径移植 C 代码 | 需要逐 bit 匹配 CMSIS 定点行为时 |
| **3** | S-Function Builder 封装 | 保留原 C 代码直接集成到 Simulink |
| **4** | MATLAB Function 模块重写 | 对性能要求不高，需要完全可视化调试 |

### 8.2 定点处理注意事项

CMSIS Q15/Q31 使用 **DIF 结构 + 每级缩放** 避免溢出。MATLAB `fi()` 对象有溢出保护，但计算策略不同：

- CMSIS Q15：每级右移 2 bit（÷4），最终输出 × 1/N
- MATLAB fixed-point FFT：通常用 `fi(..., 'OverflowAction', 'Wrap', 'ProductMode', 'KeepMSB')`
- **建议**：若需 CMSIS 一致的定点行为，参考 `arm_cfft_radix4_q15.c` 的 `__SHADD16`/`__QADD16` 等操作，用 MATLAB `bitand`/`bitsra`/`int16` 模拟

### 8.3 位反序移植

MATLAB 原生支持：

```matlab
% 方案 A: 使用 bitrevorder
[y, i] = bitrevorder(x);

% 方案 B: 直接使用索引
N = length(x);
idx = 0:N-1;
rev_idx = bin2dec(fliplr(dec2bin(idx, log2(N)))) + 1;
y = x(rev_idx);
```

### 8.4 DCT4 移植（最复杂）

推荐直接用 MATLAB `dct(x, 'Type', 4)`（需 Signal Processing Toolbox）。

若需逐 bit 匹配 CMSIS 输出：

```
1. 预处理: r = 2 * x .* cos(pi*(2*(0:N-1)+1)/(4*N));
2. 重排: r_even = r(1:2:end);  r_odd = r(end:-2:2);
          y = [r_even; r_odd];
3. RFFT: y = fft(y, N);
4. 复乘: y = y .* weights;  % weights 从 init 表获取
5. DCT4 递推:
   y4 = zeros(N,1);
   y4(1) = real(y(1))/2;
   for k = 2:N, y4(k) = real(y(k)) - y4(k-1); end
6. 归一化: y4 = y4 * sqrt(2/N);
```

### 8.5 建议的移植文件结构

```
arm_simulink_lib/
├── TransformFunctions/
│   ├── cfft/
│   │   ├── arm_cfft_f32.m       (MATLAB 实现)
│   │   ├── arm_cfft_q31.m       (Q31 定点)
│   │   ├── arm_cfft_q15.m       (Q15 定点)
│   │   ├── arm_radix8_butterfly_f32.m
│   │   └── arm_bitreversal.m
│   ├── rfft/
│   │   ├── arm_rfft_fast_f32.m
│   │   └── stage_rfft_f32.m / merge_rfft_f32.m
│   └── dct4/
│       ├── arm_dct4_f32.m
│       ├── arm_dct4_q31.m
│       └── arm_dct4_q15.m
├── Tables/                       (twiddle 因子和位反序表)
│   ├── twiddleCoef.m
│   ├── armBitRevIndexTable.m
│   └── dct4_weights.m
```

---

## 9. 各文件关键特征速查表

| 文件 | 主要数据类型 | 有无 SIMD | 有无汇编 | 有无 CM0 分支 | 调用 init |
|------|-------------|-----------|---------|--------------|----------|
| `arm_cfft_f32.c` | float32_t | 无 | 调 `arm_bitreversal_32` | 无 | 否（预定义实例） |
| `arm_cfft_q31.c` | q31_t | 无 | 调 `arm_bitreversal_32` | 无 | 否 |
| `arm_cfft_q15.c` | q15_t | `__SMUAD` 等 | 调 `arm_bitreversal_16` | 有 | 否 |
| `arm_cfft_radix8_f32.c` | float32_t | 无 | 无 | 无 | 无 |
| `arm_cfft_radix2_f32.c` | float32_t | 无 | 无 | 有 | 无 |
| `arm_cfft_radix4_f32.c` | float32_t | 无 | 无 | 有 | 无 |
| `arm_cfft_radix4_q31.c` | q31_t | `__SIMD64` | 无 | 有（CM7 分支） | 无 |
| `arm_cfft_radix4_q15.c` | q15_t | `__SMUAD` 等 | 无 | 有 | 无 |
| `arm_cfft_radix2_q15.c` | q15_t | `__SMUAD` 等 | 无 | 有 | 无 |
| `arm_cfft_radix2_q31.c` | q31_t | 无 | 无 | 无 | 无 |
| `arm_rfft_fast_f32.c` | float32_t | 无 | 无 | 无 | 有 |
| `arm_rfft_f32.c` | float32_t | 无 | 无 | 无 | 有 |
| `arm_rfft_q15.c` | q15_t | `__SMUSD` 等 | 无 | 有 | 有 |
| `arm_rfft_q31.c` | q31_t | 无 | 无 | 无 | 有 |
| `arm_dct4_f32.c` | float32_t | 无 | 无 | 有 | 有 |
| `arm_dct4_q15.c` | q15_t | 无 | 无 | 有 | 有 |
| `arm_dct4_q31.c` | q31_t | 无 | 无 | 有 | 有 |
| `arm_bitreversal.c` | 通用 | 无 | 无 | 无 | 无 |
| `arm_bitreversal2.S` | 通用 | **汇编** | **汇编** | **有** | 无 |

---

## 10. 数据格式与精度总结

| 变换 | 数据类型 | 精度 | 缩放因子 | 内存布局 |
|------|----------|------|----------|----------|
| CFFT f32 | float32_t | ~7.2 位有效数字 | 无缩放 | 复交叠 `[r0,i0,r1,i1,...]` |
| CFFT q31 | q31_t (1.31) | 32 位 | 每级 ÷4，总 ÷N | 同上 |
| CFFT q15 | q15_t (1.15) | 16 位 | 每级 ÷4，总 ÷N | 同上 |
| RFFT f32 | float32_t | ~7.2 位 | 无缩放 | 输入实序列，输出复频谱 |
| RFFT q15/q31 | q15_t/q31_t | 16/32 位 | 依赖内部 CFFT | 同上 |
| DCT4 f32 | float32_t | ~7.2 位 | ×sqrt(2/N) | 实数 in/out |
| DCT4 q15/q31 | q15_t/q31_t | 16/32 位 | + 格式转换移位 | 同上 |

---

> **文档生成日期**: 2026-06-02  
> **分析范围**: CMSIS 4.5.0 DSP_Lib TransformFunctions（不含 init 文件）  
> **目的**: 辅助将 CMSIS-DSP 变换函数移植到 MATLAB Simulink 平台