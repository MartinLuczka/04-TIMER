#include <stdbool.h>
#include <stm8s.h>
#include <stdio.h>
#include "main.h"
#include "milis.h"
//#include "delay.h"
//#include "uart1.h"
#include "daughterboard.h"

void init(void)
{

    init_milis();

    CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);   // taktování MCU na 16MHz

    GPIO_Init(PWM_R_PORT, PWM_R_PIN, GPIO_MODE_OUT_PP_LOW_SLOW); // inicializace RBG LED
    GPIO_Init(PWM_G_PORT, PWM_G_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(PWM_B_PORT, PWM_B_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(S1_PORT, S1_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(S2_PORT, S2_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(S3_PORT, S3_PIN, GPIO_MODE_IN_PU_NO_IT);

    GPIO_Init(LED7_PORT, LED7_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LED5_PORT, LED5_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_Init(LED8_PORT, LED8_PIN, GPIO_MODE_OUT_PP_LOW_SLOW);

    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 10000-1); // inicializace časovače

    TIM2_OC1Init( // inicializace výstupního kanálu
    TIM2_OCMODE_PWM1, // režim PWM1
    TIM2_OUTPUTSTATE_ENABLE, // vstup povolen (TIMer ovládá pin)
    5000,
    TIM2_OCPOLARITY_HIGH
    );

    TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 5000, TIM2_OCPOLARITY_HIGH); // nastavení činitele plnění
    TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 5000, TIM2_OCPOLARITY_HIGH);

    TIM2_OC1PreloadConfig(ENABLE);
    TIM2_OC2PreloadConfig(ENABLE);
    TIM2_OC3PreloadConfig(ENABLE);

    TIM2_Cmd(ENABLE);

}


int main(void)
{

    uint32_t time = 0;

    uint16_t r = 1, g = 1, b = 1;

    init();

    uint16_t soucasny_stav = 0;

    // red - 1
    // green - 2
    // blue - 3

    bool stav_S1 = 0;
    bool stav_S2 = 0;
    bool stav_S3 = 0;

    TIM2_SetCompare1(r-1);
    TIM2_SetCompare2(g-1);
    TIM2_SetCompare3(b-1);

    while (1) {


        if(milis() - time > 333) {
            if(PUSH(S1)) {
            stav_S1 = 1;
        }
        else
        {
            if (stav_S1 == 1) {
                REVERSE(LED7);
            }
            stav_S1 = 0;
        }
        }

        if(milis() - time > 333) {
            if(PUSH(S1)) {
                r += 1000;
                if (r > 1000) {
                    r = 0;
                }
                TIM2_SetCompare1(r-1); // nastavení trashholdu
            }
        }

        if(milis() - time > 333) {
            if(PUSH(S2)) {
                g += 1000;
                if (g > 1000) {
                    g = 0;
                }
                TIM2_SetCompare2(g-1);
            }
        }

        if(milis() - time > 333) {
            if(PUSH(S3)) {
                b += 1000;
                if (b > 1000) {
                    b = 0;
                }
                TIM2_SetCompare3(b-1);
            }
        }
    }

}

// tlačítko přepíná RGB, další dvě tlačítka jsou přidávání a oddělávání. Režimy RGB jsou signalizovány na desce. Změna barvy např. o 10 %,
// popřípadě třeba logaritické přidávání
// UART použít pro signalizaci 

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
