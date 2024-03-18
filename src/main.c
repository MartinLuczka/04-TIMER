#include <stdbool.h>
#include <stm8s.h>
//#include <stdio.h>
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
    GPIO_Init(S2_PORT, S2_PIN, GPIO_MODE_IN_PU_NO_IT);

    TIM2_TimeBaseInit(TIM2_PRESCALER_16, 10000-1); // inicializace časovače

    TIM2_OC1Init(
    TIM2_OCMODE_PWM1, // inicializace výstupního kanálu
    TIM2_OUTPUTSTATE_ENABLE,
    5000,
    TIM2_OCPOLARITY_HIGH
    );

    TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 5000, TIM2_OCPOLARITY_HIGH);
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
    TIM2_SetCompare1(r-1);
    TIM2_SetCompare2(g-1);
    TIM2_SetCompare3(b-1);

    while (1) {

        if(milis() - time > 333) {
            if(PUSH(S1)) {
                r += 1000;
                if (r > 1000) {
                    r = 0;
                }
                TIM2_SetCompare1(r-1);
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

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
