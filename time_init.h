
//----------------------------TIMER ENABLE/DISABLE----------------------------------------------------------------------------------------------
#define DISABLE_TIMER_1   TIM1_CR1.CEN = 0;
#define ENABLE_TIMER_1    TIM1_CR1.CEN = 1;    // Enable timer 1 interrupt

#define DISABLE_TIMER_2   TIM2_CR1.CEN = 0;
#define ENABLE_TIMER_2    TIM2_CR1.CEN = 1;   // Enable timer 2 interrupt

#define DISABLE_TIMER_3  TIM3_CR1.CEN = 0;
#define ENABLE_TIMER_3   TIM3_CR1.CEN = 1;  // Enable timer 3 interrupt

#define DISABLE_TIMER_4  TIM4_CR1.CEN = 0;
#define ENABLE_TIMER_4   TIM4_CR1.CEN = 1; // Enable timer 4 interrupt

#define DISABLE_TIMER_5  TIM5_CR1.CEN = 0;
#define ENABLE_TIMER_5   TIM5_CR1.CEN = 1; // Enable timer 5 interrupt

#define DISABLE_TIMER_6  TIM6_CR1.CEN = 0;
#define ENABLE_TIMER_6   TIM6_CR1.CEN = 1; // Enable timer 6 interrupt

#define DISABLE_TIMER_7  TIM7_CR1.CEN = 0;
#define ENABLE_TIMER_7   TIM7_CR1.CEN = 1;  // Enable timer 7 interrupt

#define DISABLE_TIMER_8   TIM8_CR1.CEN = 0;
#define ENABLE_TIMER_8    TIM8_CR1.CEN = 1;  // Enable timer 8 interrupt

#define DISABLE_TIMER_9  TIM9_CR1.CEN = 0;
#define ENABLE_TIMER_9   TIM9_CR1.CEN = 1;  // Enable timer 9 interrupt

#define DISABLE_TIMER_10  TIM10_CR1.CEN = 0;
#define ENABLE_TIMER_10   TIM10_CR1.CEN = 1;  // Enable timer 10 interrupt

#define DISABLE_TIMER_11  TIM11_CR1.CEN = 0;
#define ENABLE_TIMER_11   TIM11_CR1.CEN = 1;  // Enable timer 11 interrupt

#define DISABLE_TIMER_12  TIM12_CR1.CEN = 0;
#define ENABLE_TIMER_12   TIM12_CR1.CEN = 1; // Enable timer 12 interrupt

#define DISABLE_TIMER_13  TIM13_CR1.CEN = 0;
#define ENABLE_TIMER_13   TIM13_CR1.CEN = 1;   // Enable timer 13 interrupt

#define DISABLE_TIMER_14  TIM14_CR1.CEN = 0;
#define ENABLE_TIMER_14   TIM14_CR1.CEN = 1;  // Enable timer 14 interrupt

//------------------------------Timers---------------------------------------------------------------------------------------------------
 void timer_1_init(void);
 void timer_2_init(void);

 void timer_3_init(void);
 void timer_4_init(void);
 void timer_5_init(void);
 void timer_6_init(void);
 void timer_7_init(void);
 void timer_8_init(void);

 void timer_9_init(void);
 void timer_10_init(void);
 void timer_11_init(void);

 void timer_12_init(void);
 void timer_13_init(void);
 void timer_14_init(void);
 void all_timers_init(void);
 