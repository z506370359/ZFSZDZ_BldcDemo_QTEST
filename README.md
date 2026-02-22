# STM32 BLDC Open Loop Control Demo (ZFSZDZ)

## 📖 项目简介 | Introduction

本项目是一个基于 **STM32** 微控制器的无刷直流电机（BLDC）**开环方波控制**工程。
项目包含完整的电机控制软件架构，并增加了各个模块的源文件和头文件。系统实现了基于霍尔传感器的无刷电机开环转动控制。

> **适用课程**：张飞实战电子"扬帆起航"课程电机控制部分
> **当前版本**：V1.0.0
> **作者**：ZhaoYun (ZFSZDZ)

---

## 🛠️ 硬件平台 | Hardware Setup

### 1. 微控制器 (MCU)

* **型号**：STM32G030C8Tx（代码底层依赖 `stm32g030xx.h` 头文件）。

### 2. 电机参数与驱动

* **类型**：三相无刷直流电机 (3-Phase BLDC)。
* **传感器**：霍尔传感器 (Hall Sensors)。
* **驱动方式**：三相逆变桥（MOSFETs + Gate Drivers）。

---

## 🔌 引脚定义 | Pin Definitions

根据底层逻辑推断，霍尔传感器及 PWM 控制连接如下：

| 功能 (Function) | 引脚 (Pin) | 说明 |
| --- | --- | --- |
| **Hall Sensor A** | **PA6** | GPIOA Bit 6 |
| **Hall Sensor B** | **PA7** | GPIOA Bit 7 |
| **Hall Sensor C** | **PB0** | GPIOB Bit 0 |
| **PWM Output** | **TIM1 CH1/2/3** | 互补输出控制 U/V/W 相 |

---

## ⚙️ 软件功能 | Software Features

### 1. 核心控制策略与功能

* **六步换相法**：代码内置了 `BLDC_UV()`, `BLDC_WV()`, `BLDC_WU()`, `BLDC_VU()`, `BLDC_VW()`, `BLDC_UW()` 等具体的换相执行函数，以及全关断函数 `BLDC_PWMOFF()`。
* **双模式调速**：
* 系统支持使用电位器来调节占空比。
* 系统同时支持使用串口指令来调节占空比。


* **平滑启动控制**：在电机转速调整的逻辑中增加了斜坡功能，使加减速更加平稳。

### 2. 状态机架构 (FSM)

电机的运行逻辑由 `BLDC_StateMachine()` 统一管理，包含以下核心状态：

* `STOP_STATE` (0): 停机状态。
* `CHARGEREADY_STATE` (1): 自举电容预充电准备（通常打开下桥臂）。
* `CHARGEWAIT_STATE` (2): 等待预充电完成。
* `STARTINIT_STATE` (3): 启动初始化阶段（开环强拖/定位）。
* `MOTORRUN_STATE` (4): 稳定运行阶段（闭环换相，开环调速）。
* `TEST_STATE` (5): 专用的测试模式。

### 3. 可配置参数 (`UserParams.h`)

用户可以在 `Include/UserParams.h` 中修改系统参数，例如：

* 调速模式选择（如电位器模式 `DEFAULT_SPEEDMODE`）。
* 电机运行方向（如顺时针 `DEFAULT_DIR`）。
* 启动占空比（`START_MIN_DUTY` 和 `START_MAX_DUTY`）与最大运行占空比（`RUN_MAX_DUTY`）限制。
