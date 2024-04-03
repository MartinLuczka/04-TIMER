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
    GPIO_Init(S1_PORT, S1_PIN, GPIO_MODE_IN_PU_NO_IT); // inicializace tlačítek
    GPIO_Init(S2_PORT, S2_PIN, GPIO_MODE_IN_PU_NO_IT);
    GPIO_Init(S3_PORT, S3_PIN, GPIO_MODE_IN_PU_NO_IT);

    TIM2_TimeBaseInit( // inicializace časovače
    TIM2_PRESCALER_16, // nastavení předděličky
    10000-1); // časová perioda

    TIM2_OC1Init(
    TIM2_OCMODE_PWM1, // inicializace výstupního kanálu, jeho režim
    TIM2_OUTPUTSTATE_ENABLE, // kanál povolen, může generovat signál
    5000, // délka periody
    TIM2_OCPOLARITY_HIGH // polarita, log 1 při aktivním stavu
    );

    TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 5000, TIM2_OCPOLARITY_HIGH);
    TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 5000, TIM2_OCPOLARITY_HIGH);

    TIM2_OC1PreloadConfig(ENABLE); // povolení přednačítání hodnot pro výstupní komparátor
    TIM2_OC2PreloadConfig(ENABLE);
    TIM2_OC3PreloadConfig(ENABLE);

    TIM2_Cmd(ENABLE); // zapne časovač TIM2

}


int main(void)
{
  
    uint32_t time = 0;

    uint16_t r = 1, g = 1, b = 1;

    init();
    TIM2_SetCompare1(r-1); // nastavení hodnot pro porovnání
    TIM2_SetCompare2(g-1); // -1, aby byly hodnoty 0 a signál se zatím negeneroval
    TIM2_SetCompare3(b-1); // (vycházíme z deklarovaných promměných r, g, b)

    while (1) {

        if(milis() - time > 333) { // stisk kontrolujeme vždy po nějaké době
            if(PUSH(S1)) {
                r += 1000;
                if (r > 1000) { // pokud r == 1001, tak se nastaví zpátky do 0
                    r = 0;
                }
                TIM2_SetCompare1(r-1); // přenastavení porovnání
            }
        }

        if(milis() - time > 333) { // stisk kontrolujeme vždy po nějaké době
            if(PUSH(S2)) {
                g += 1000;
                if (g > 1000) { // pokud g == 1001, tak se nastaví zpátky do 0
                    g = 0;
                }
                TIM2_SetCompare2(g-1); // přenastavení porovnání
            }
        }

        if(milis() - time > 333) { // stisk kontrolujeme vždy po nějaké době
            if(PUSH(S3)) {
                b += 1000;
                if (b > 1000) { // pokud b == 1001, tak se nastaví zpátky do 0
                    b = 0;
                }
                TIM2_SetCompare3(b-1); // přenastavení porovnání
            }
        }
    }

}

// pokud je nějaká z hodnot r, g, b v setcompare větší, než 0, tak se bude generovat PWM signál (o takové frekvenci, že to lidské oko nepozná), střídou poté akorát měníme
// podíl a jednotlivou sílu příspěvků barev, různá kombinace RGB barev poté způsobuje zobrazování různých barev (barva svítí jen třeba pětinu periody - tuto dobu TH udává
// právě hodnota v capture compare registru, podle které se PWM signál dostává do 0 a do 1

/*-------------------------------  Assert -----------------------------------*/
#include "__assert__.h"
