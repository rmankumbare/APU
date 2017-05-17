#include "headers.h"
#include "stdint.h"
#include "time_init.h"
//---------------------------GLOBAL VARIABLES---------------------------------------------------------------------------------------------------------------
uint8_t flag_dc_cont =0;
unsigned long preheat_counter=0,crank_counter=0,crank_rest_counter=0,warm_up_counter=0,run_up_counter=0;
unsigned long stop_counter=0,stop_rest_counter=0,excitation_pulse_counter=0;
//---------------------------------------------------------------------------------------------------------------------------------------------------

void all_timers_init()
{
  timer_3_init();
  timer_4_init();
  timer_7_init();
}

void timer_3_init()
{
  GPIOB_AFRL |=1<<17;            // capture pin  PB4
  RCC_APB1ENR.TIM3EN = 1;       // Enable clock gating for timer module 2
  TIM3_CR1.CEN = 0;             // Disable timer
  TIM3_PSC = 1153;              // Set timer prescaler.
  TIM3_ARR = 65535;
  NVIC_IntEnable(IVT_INT_TIM3); // Enable timer interrupt
  TIM3_CCMR1_input &= (~(0x3 << 0));
  TIM3_CCMR1_input |= (1 << 0);        //CC1_input_IC1_on_TI1
  TIM3_CCERbits.CC1E=1;
  TIM3_CCERbits.CC1P=0;
  TIM3_CCER.CC1NP=1;            //set_TIM2_CC1_state_and_polarity(enable, falling_edge);
  TIM3_DIERbits.CC1IE=1;
  TIM3_DIER.CC1IE = 1;          // Capture/Compare 1 interrupt enable
  TIM3_DIER.UIE = 1;            // Update interrupt enable
  TIM3_CR1.CEN = 1;             // Enable timer           */

}
void timer_4_init()
{
  GPIOD_AFRH |=1<<17;            //    capture pin                //PD12 AF@
  RCC_APB1ENR.TIM4EN = 1;       // Enable clock gating for timer module 2
  TIM4_CR1.CEN = 0;             // Disable timer
  TIM4_PSC = 1153;              // Set timer prescaler.
  TIM4_ARR = 65535;
  NVIC_IntEnable(IVT_INT_TIM4); // Enable timer interrupt
  TIM4_CCMR1_input &= (~(0x3 << 0));
  TIM4_CCMR1_input |= (1 << 0);        //CC1_input_IC1_on_TI1
  TIM4_CCERbits.CC1E=1;
  TIM4_CCERbits.CC1P=0;
  TIM4_CCER.CC1NP=1;            //set_TIM2_CC1_state_and_polarity(enable, falling_edge);
  TIM4_DIERbits.CC1IE=1;
  TIM4_DIER.CC1IE = 1;          // Capture/Compare 1 interrupt enable
  TIM4_DIER.UIE = 1;            // Update interrupt enable
  TIM4_CR1.CEN = 1;             // Enable timer           */
}

void timer_7_init()
{
  RCC_APB1ENR.TIM7EN = 1;       // Enable clock gating for timer module 2
  TIM7_CR1.CEN = 0;             // Disable timer
  TIM7_PSC = 1153;              // Set timer prescaler.
  TIM7_ARR = 65535;
  NVIC_IntEnable(IVT_INT_TIM7); // Enable timer interrupt
  TIM7_DIER.UIE = 1;            // Update interrupt enable
  TIM7_CR1.CEN = 1;             // Enable timer
}

//---------------------------TIMER INTERRUPTS------------------------------------------------------------------------------


void  Timer7_interrupt() iv IVT_INT_TIM7 ics ICS_AUTO
{
  if(TIM7_SR.UIF)
  {
      TIM7_SR.UIF = 0;
      if(preheat_counter > 0)
        preheat_counter--;
      if(excitation_pulse_counter > 0)
        excitation_pulse_counter--;
      if(crank_rest_counter > 0)
       crank_rest_counter--;
      if(stop_rest_counter > 0)
        stop_rest_counter--;
      if(warm_up_counter > 0)
        warm_up_counter--;
      if(crank_counter > 0)
        crank_counter--;
      if(run_up_counter > 0)
        run_up_counter--;
      if(stop_counter > 0)
        stop_counter--;
   }
}



