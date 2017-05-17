
//------------------------DIGITAL INPUTS ON PORT E -----------------------------------------------------------------------------------------------
#define  ECU_1_ENG_OVER_SPEED       (GPIOE_IDR & (1<<0))
#define  ECU_2_SERVICE_CHECK_LAMP   (GPIOE_IDR & (1<<1))
#define  ECU_3_LOW_OIL_PRESS_LAMP   (GPIOE_IDR & (1<<2))
#define  ECU_4_HIGH_COOL_TEMP_LAMP  (GPIOE_IDR & (1<<3))
#define  ECU_5_WATER_IN_FUEL_LAMP   (GPIOE_IDR & (1<<4))

#define  AIR_FILTER_SWITCH (GPIOE_IDR & (1<<5))
#define  EMERGENCY_STOP    (GPIOE_IDR & (1<<7))
#define  AESS_READY        (GPIOE_IDR & (1<<9))
#define  OPERATION_MODE    (GPIOE_IDR & (1<<10))
#define  APU_ENABLE        (GPIOE_IDR & (1<<11))
#define  APU_DISABLE       (GPIOE_IDR & (1<<12))
#define  EXCITATION_RUN    (GPIOE_IDR & (1<<14))

#define  MCCB_FEEDBACK         (GPIOE_IDR & (1<<6))
#define  DC_CONTACTOR_FEEDBACK (GPIOE_IDR & (1<<8))

//#define  SPARE             (GPIOE_IDR & (1<<6))
//#define  APU_DISABLE       (GPIOE_IDR & (1<<8))



 void port_configurations(void);
void initial_port_config(void);
void my_delay_ms(unsigned long value);