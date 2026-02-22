/***********************************************************************
* @file           Main.c
* @author         ZhaoYun(ZFSZDZ)
* @date           2024.10.12
* @Version        V1.0.0
* @description    张飞实战电子"杨帆起航"课程单片机基础部分工程  
***********************************************************************/
#include "../Include/Main.h"

unsigned int TIM3_CCR1;
unsigned char Counter;

//Q15 * Q15
signed short qTestA;
signed short qTestB;
signed int qTestC;
signed short qTestD;

//Q16 * Q15
unsigned short uqTestE;
//Q16 * Q16
unsigned short uqTestF;
unsigned int uqTestG;
unsigned short uqTestH;

//θ:        0°~360°          -180°~180° 
//1.归一化
//          θ / 360° 0~1      θ / 180°  -1~1
//2.定点化  Q16               Q15
//          0~65535           -32768~32767

//Q14 * Q15 = Q29
//1.5 * 0.6
//1.5 * 2^14 = 
//0.6 * 32768 = 

//Q12 * Q15 = Q27
//6.8 * 0.5
//6.8 * 2^12 = 
//0.5 * 2^15 = 

//Q10 * Q14 = Q24
//20.8 * 1.4
//20.8 * 2^10 = 
//1.4 * 2^14 = 

/***********************************************************************
* 函数名称  ：MainLoop
* 功能描述  ：主函数
* 形参      ：无
* 返回值    ：无
* 要点备注  ：单片机上电复位向量执行后跳转到该函数
***********************************************************************/
void MainLoop(void)
{
    Initialize();
    
    //Q15 * Q15
    //0.1 * 0.6 = 0.06
    qTestA = 3277; // 0.1*2^15 + 0.5 = 3277
    qTestB = 19661; //0.6 * 2^15 + 0.5 = 19661
    qTestC = qTestA * qTestB; //Q15(0.1) * Q15(0.6)
    qTestD = qTestC >> 15; //Q30 >> 15 = Q15
    
    //-0.35 * 0.8 = -0.28
    qTestA = -11469; // -0.35*2^15 + 0.5 = -11469
    qTestB = 26214; //0.8 * 2^15 + 0.5 = 26214
    qTestC = qTestA * qTestB; //Q15(-0.35) * Q15(0.8)
    qTestD = qTestC >> 15; //Q30 >> 15 = Q15
    
    //Q16 * Q15
    //0.4 * -0.7 = -0.28
    uqTestE = 26214; //0.4 * 65536 + 0.5 
    qTestA = -22938; // -0.7 * 32768 + 0.5
    qTestC = uqTestE * qTestA; //Q16(0.4) * Q15(-0.7)
    qTestD = qTestC >> 16; //Q31 >> 16 = Q15
    
    //Q16 * Q16
    //0.08 * 0.29 = 0.0232
    uqTestE = 5243;//0.08 * 65536
    uqTestF = 19005;//0.29 * 65536
    uqTestG = uqTestE * uqTestF; //Q16 * Q16 = Q32
    uqTestH = uqTestG >> 16; //Q32 >> 16 = Q16
    
    //Q30 / Q15 = Q15
    //0.2 / 0.8 = 0.25
    //0.2 * 32768 = 6554  Q15
    //0.8 * 32768 = 26214 Q15
    
    //把分子扩大为Q30 
    qTestA = 6554;
    qTestB = 26214;
    qTestC = (signed int)(qTestA << 15); //Q15扩为Q30
    qTestD = qTestC / qTestB; //0.25 的Q15值
    
    do
    {
        Display();
    }while(1);
}
/***********************************************************************
* 函数名称  : DMA1_Channel1_IRQHandler
* 功能描述  ：DMA通道一中断服务函数
* 形参      ：无
* 返回值    ：无
* 要点备注  ：DMA搬运ADC数据，搬运完成后，进入中断
***********************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
    unsigned int Temp;
    
    Temp = DMA1->ISR;
    if(Temp & 0x00000002)
    {
        DMA1->IFCR |= 0x00000002; //清除中断标志位
        tAdc.AN0 = tDMA.ADDATA0;
        tAdc.AN3 = tDMA.ADDATA1;
        tAdc.AN4 = tDMA.ADDATA2;
        tAdc.AN5 = tDMA.ADDATA3;
        tAdc.AN9 = tDMA.ADDATA4;
    }
}
/***********************************************************************
* 函数名称  : TIM3_IRQHandler
* 功能描述  ：定时器3捕捉中断服务函数
* 形参      ：无
* 返回值    ：无
* 要点备注  ：有霍尔沿跳变，进入该中断
***********************************************************************/
void TIM3_IRQHandler(void)
{
    unsigned int Temp;
    
    Temp = TIM3->SR;
    if(Temp & 0x0002)
    {
        TIM3->SR &= 0xfffd; //CCIF = 0 清除捕捉中断标志位
        TIM3_CCR1 = TIM3->CCR1;
    }
}
/***********************************************************************
* 函数名称  : SysTick_Handler
* 功能描述  ：滴答定时器中断服务函数
* 形参      ：无
* 返回值    ：无
* 要点备注  ：1ms进入该中断一次
***********************************************************************/
void SysTick_Handler(void)
{
    unsigned int Temp;
    
    Temp = SysTick->CSR;
    
    if(Temp & 0x00010000) //判断COUNTFLAG = 1
    {
        Counter ++;
        if(Counter == 1)
        {
            GPIOB->BSRR |= 0x00100000; //GPIOB_PIN4  = 0
        }
        else if(Counter == 2)
        {
            GPIOB->BSRR |= 0x00000010; //GPIOB_PIN4  = 1
            Counter = 0;
        }
    }
}
/*******************Copyright (c) ZFSZDZ ******END OF FILE************/
