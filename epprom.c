#include "headers.h"
#include "stdint.h"
#include "eeprom.h"
#include "my_uart.h"
//-----------------------------------Global variables------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8_t data_[10],temp_buf[5], rx_data[7],test_1_buf[5];
unsigned long temp_int;
volatile long i;
float temp_float;
//---------------------------------extern variables--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
extern unsigned long min_count_1,hour_count_1,maintainance_hour_count;
extern run_hours_buf[4],run_min_buf[4];
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

void Write_single_byte_to_eeprom(uint8_t  high_byte, uint8_t low_byte, uint8_t data_byte)
 {
  data_[0] = high_byte;
  data_[1] = low_byte ;
  data_[2] = data_byte;
  I2C1_Start();
  I2C1_Write(0x50,data_,3,END_MODE_STOP);
  Delay_ms(5);
 }
 uint8_t read_single_byte_from_eeprom(uint8_t  high_byte, uint8_t low_byte)
{
  data_[0] = high_byte;
  data_[1] = low_byte;
  I2C1_Start();
  I2C1_Write(0x50,data_,2,END_MODE_RESTART);
  I2C1_Read(0x50,data_,1,END_MODE_STOP);
  return data_[0];
}

 //---------------------------READ VALUES FROM EEPROM---------------------------------------------------------------------

 float get_mr_press_war_val_from_eeprom(void)
 {
  for(i=0;i<6;i++)
  rx_data[i] = read_single_byte_from_eeprom(MR_AIR_PRESS_WARNING_DEF_VAL_HIGH_ADD+i , MR_AIR_PRESS_WARNING_DEF_VAL_LOW_ADD+i);
   rx_data[6] = 0;
   temp_float = atof(rx_data);
  return  temp_float;
 }
 float get_mr_press_ala_val_from_eeprom(void)
 {
  for(i=0;i<6;i++)
  rx_data[i] = read_single_byte_from_eeprom(MR_AIR_PRESS_ALARM_DEF_VAL_HIGH_ADD+i , MR_AIR_PRESS_ALARM_DEF_VAL_LOW_ADD+i);
  rx_data[6] = 0;
  temp_float =  atof(rx_data);
  return  temp_float;
 }

 float get_bcp_press_war_val_from_eeprom(void)
 {
  for(i=0;i<6;i++)
   rx_data[i] = read_single_byte_from_eeprom(BCP_PRESS_WARNING_DEF_VAL_HIGH_ADD+i , BCP_PRESS_WARNING_DEF_VAL_LOW_ADD+i);
  rx_data[6] = 0;
  temp_float =  atof(rx_data);
  return  temp_float;
 }
 float get_bcp_press_ala_val_from_eeprom(void)
 {
  for(i=0;i<6;i++)
   rx_data[i] = read_single_byte_from_eeprom(BCP_PRESS_ALARM_DEF_VAL_HIGH_ADD+i , BCP_PRESS_ALARM_DEF_VAL_LOW_ADD+i);
  rx_data[6] = 0;
  temp_float =  atof(rx_data);
  return  temp_float;
 }

float get_loco_low_bat_war_val_from_eeprom(void)
 {
  for(i=0;i<6;i++)
    rx_data[i] = read_single_byte_from_eeprom(LOCO_BAT_LOW_VTG_WARNING_DEF_VAL_HIGH_ADD+i , LOCO_BAT_LOW_VTG_WARNING_DEF_VAL_LOW_ADD+i);
  rx_data[6] = 0;
  temp_float =  atof(rx_data);
  return  temp_float;
 }
float get_loco_low_bat_ala_val_from_eeprom(void)
 {
   for(i=0;i<6;i++)
    rx_data[i] = read_single_byte_from_eeprom(LOCO_BAT_LOW_VTG_ALARM_DEF_VAL_HIGH_ADD+i , LOCO_BAT_LOW_VTG_ALARM_DEF_VAL_LOW_ADD+i);
  rx_data[6] = 0;
  temp_float =  atof(rx_data);
  return  temp_float;
 }
 float get_loco_high_bat_war_val_from_eeprom(void)
 {
    for(i=0;i<6;i++)
    rx_data[i] = read_single_byte_from_eeprom(LOCO_BAT_HIGH_VTG_WARNING_DEF_VAL_HIGH_ADD+i , LOCO_BAT_HIGH_VTG_WARNING_DEF_VAL_LOW_ADD+i);
  rx_data[6] = 0;
  temp_float =  atof(rx_data);
  return  temp_float;
 }
 float get_loco_high_bat_ala_val_from_eeprom(void)
 {
   for(i=0;i<6;i++)
    rx_data[i] = read_single_byte_from_eeprom(LOCO_BAT_HIGH_VTG_ALARM_DEF_VAL_HIGH_ADD+i , LOCO_BAT_HIGH_VTG_ALARM_DEF_VAL_LOW_ADD+i);
  rx_data[6] = 0;
  temp_float =  atof(rx_data);
  return  temp_float;
 }

 unsigned long get_eng_cool_temp_war_val_from_eeprom(void)  // ==  main eng water temp
 {
  uint8_t rx_data[5];
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(ENG_COOL_TEMP_WARNING_DEF_VAL_HIGH_ADD+i , ENG_COOL_TEMP_WARNING_DEF_VAL_LOW_ADD+i);
  rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
 unsigned long get_eng_cool_temp_ala_val_from_eeprom(void)  // ==  main eng water temp
 {
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(ENG_COOL_TEMP_ALARM_DEF_VAL_HIGH_ADD+i , ENG_COOL_TEMP_ALARM_DEF_VAL_LOW_ADD+i);
    rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
 unsigned long get_eng_fuel_level_war_val_from_eeprom(void)
 {
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(ENG_FUEL_WARNING_DEF_VAL_HIGH_ADD+i , ENG_FUEL_WARNING_DEF_VAL_LOW_ADD+i);
  rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
 unsigned long get_eng_fuel_level_ala_val_from_eeprom(void)
 {
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(ENG_FUEL_ALARM_DEF_VAL_HIGH_ADD+i , ENG_FUEL_ALARM_DEF_VAL_LOW_ADD+i);
    rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
float eng_fuel_press_war_val_from_eeprom(void)    // == ENG OIL PRESSURE
 {
  for(i=0;i<6;i++)
   rx_data[i] = read_single_byte_from_eeprom(ENG_OIL_PRESS_WARNING_DEF_VAL_HIGH_ADD+i , ENG_OIL_PRESS_WARNING_DEF_VAL_LOW_ADD+i);
  rx_data[6] = 0;
  temp_float =  atof(rx_data);
  return  temp_float;
 }
float eng_fuel_press_ala_val_from_eeprom(void)  // == ENG OIL PRESSURE
 {
    for(i=0;i<6;i++)
    {
     rx_data[i] = read_single_byte_from_eeprom(ENG_OIL_PRESS_ALARM_DEF_VAL_HIGH_ADD+i , ENG_OIL_PRESS_ALARM_DEF_VAL_LOW_ADD+i);
    }
    rx_data[6] = 0;
    temp_float =  atof(rx_data);
    return  temp_float;
 }
 unsigned long get_preheat_counter_val_from_eeprom(void)
 {
  uint8_t rx_data[5];
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(PREHEAT_COUNTER_DEF_VAL_HIGH_ADD+i , PREHEAT_COUNTER_DEF_VAL_LOW_ADD+i);
  rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
 unsigned long get_crank_counter_val_from_eeprom(void)
 {
  uint8_t rx_data[5];
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(CRANK_TIME_DEF_VAL_HIGH_ADD+i , CRANK_TIME_DEF_VAL_LOW_ADD+i);
    rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
 unsigned long get_crank_rest_counter_val_from_eeprom(void)
 {
  uint8_t rx_data[5];
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(CRANK_REST_TIME_DEF_VAL_HIGH_ADD+i , CRANK_REST_TIME_DEF_VAL_LOW_ADD+i);
    rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
unsigned long get_warm_up_counter_val_from_eeprom(void)
 {
  uint8_t rx_data[5];
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(WARM_UP_TIME_DEF_VAL_HIGH_ADD+i , WARM_UP_TIME_DEF_VAL_LOW_ADD+i);
  rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
unsigned long get_run_up_counter_val_from_eeprom(void)
 {
  uint8_t rx_data[5];
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(RUN_UP_TIME_DEF_VAL_HIGH_ADD+i , RUN_UP_TIME_DEF_VAL_LOW_ADD+i);
    rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
unsigned long get_stop_counter_val_from_eeprom(void)
 {
  uint8_t rx_data[5];
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(STOP_TIME_DEF_VAL_HIGH_ADD+i , STOP_TIME_DEF_VAL_LOW_ADD+i);
    rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
 unsigned long get_stop_rest_counter_val_from_eeprom(void)
 {
  uint8_t rx_data[5];
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(STOP_REST_TIME_DEF_VAL_HIGH_ADD+i , STOP_REST_TIME_DEF_VAL_LOW_ADD+i);
    rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
 unsigned long get_crank_tries_val_from_eeprom(void)
 {
  uint8_t rx_data[5];
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(CRANK_TRIES_DEF_VAL_HIGH_ADD+i , CRANK_TRIES_DEF_VAL_LOW_ADD+i);
    rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
 unsigned long get_stop_tries_val_from_eeprom(void)
 {
  uint8_t rx_data[5];
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(STOP_TRIES_DEF_VAL_HIGH_ADD+i , STOP_TRIES_DEF_VAL_LOW_ADD+i);
    rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }

float get_charge_alt_frequency_val_from_eeprom(void)
{
 for(i=0;i<6;i++)
  rx_data[i] = read_single_byte_from_eeprom(CHARGE_ALT_FREQUENCY_DEF_VAL_HIGH_ADD+i , CHARGE_ALT_FREQUENCY_DEF_VAL_LOW_ADD+i);
 rx_data[6] = 0;
 temp_float =  atof(rx_data);
 return temp_float;
}
float get_apu_low_bat_war_val_from_eeprom(void)
{
 for(i=0;i<6;i++)
  rx_data[i] = read_single_byte_from_eeprom(APU_LOW_BAT_WARNING_DEF_VAL_HIGH_ADD+i , APU_LOW_BAT_WARNING_DEF_VAL_LOW_ADD+i);
 rx_data[6] = 0;
 temp_float =  atof(rx_data);
 return temp_float;
}
float get_apu_high_bat_war_val_from_eeprom(void)
{
 for(i=0;i<6;i++)
  rx_data[i] = read_single_byte_from_eeprom(APU_HIGH_BAT_WARNING_DEF_VAL_HIGH_ADD+i , APU_HIGH_BAT_WARNING_DEF_VAL_LOW_ADD+i);
 rx_data[6] = 0;
 temp_float =  atof(rx_data);
 return temp_float;
}
float get_apu_max_bat_war_val_from_eeprom(void)
{
 //uint8_t my_new_buf[10];
// float temp_float;
/*
 GPIOG_ODR.B14 = 1;
 UART6_PrintString("  EEPROM Bytes   ");
 memset(rx_data,'\0',7);  */
 for(i=0;i<6;i++)
 {
  rx_data[i] = read_single_byte_from_eeprom(APU_MAX_BAT_WARNING_DEF_VAL_HIGH_ADD+i , APU_MAX_BAT_WARNING_DEF_VAL_LOW_ADD+i);
 // UART6_Sendchar(rx_data[i]);
 }

// UART6_PrintString("     ");
 rx_data[6] = 0;
// UART6_PrintString("  atof data   ");

// temp_float = my_atof(rx_data);

 temp_float =  atof(rx_data);


// sprintf(my_new_buf,"%.2f",temp_float);
// UART6_PrintString(my_new_buf);

// GPIOG_ODR.B14 = 0;
 return temp_float;
}
//----------------------------------------------------------------------------------------------------------------------------
 unsigned long get_comp_air_temp_war_val_from_eeprom(void)
 {
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(COMP_AIR_TEMP_WARNING_DEF_VAL_HIGH_ADD+i , COMP_AIR_TEMP_WARNING_DEF_VAL_LOW_ADD+i);
    rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
 unsigned long get_comp_air_temp_ala_val_from_eeprom(void)
 {
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(COMP_AIR_TEMP_ALARM_DEF_VAL_HIGH_ADD+i , COMP_AIR_TEMP_ALARM_DEF_VAL_LOW_ADD+i);
    rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
 unsigned long get_comp_oil_temp_war_val_from_eeprom(void)
 {
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(COMP_AIR_TEMP_WARNING_DEF_VAL_HIGH_ADD+i , COMP_AIR_TEMP_WARNING_DEF_VAL_LOW_ADD+i);
    rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
 unsigned long get_comp_oil_temp_ala_val_from_eeprom(void)
 {
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(COMP_OIL_TEMP_ALARM_DEF_VAL_HIGH_ADD+i , COMP_OIL_TEMP_ALARM_DEF_VAL_LOW_ADD+i);
    rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
 /*       //get default values
float get_comp_oil_level_war_val_from_eeprom(void)
{
 for(i=0;i<5;i++)
  rx_data[i] = read_single_byte_from_eeprom(APU_HIGH_BAT_WARNING_DEF_VAL_HIGH_ADD+i , APU_HIGH_BAT_WARNING_DEF_VAL_LOW_ADD+i);
 temp_float =  atof(rx_data);
 return temp_float;
}
float get_comp_oil_level_ala_val_from_eeprom(void)
{
 for(i=0;i<5;i++)
  rx_data[i] = read_single_byte_from_eeprom(APU_HIGH_BAT_WARNING_DEF_VAL_HIGH_ADD+i , APU_HIGH_BAT_WARNING_DEF_VAL_LOW_ADD+i);
 temp_float =  atof(rx_data);
 return temp_float;
}
 */

float get_comp_air_press_war_val_from_eeprom(void)
{
 for(i=0;i<6;i++)
  rx_data[i] = read_single_byte_from_eeprom(COMP_AIR_PRESS_WARNING_DEF_VAL_HIGH_ADD+i , COMP_AIR_PRESS_WARNING_DEF_VAL_LOW_ADD+i);
 rx_data[6] = 0;
 temp_float =  atof(rx_data);
 return temp_float;
}
float get_comp_air_press_ala_val_from_eeprom(void)
{
 for(i=0;i<6;i++)
  rx_data[i] = read_single_byte_from_eeprom(COMP_AIR_PRESS_ALARM_DEF_VAL_HIGH_ADD+i , COMP_AIR_PRESS_ALARM_DEF_VAL_LOW_ADD+i);
 rx_data[6] = 0;
 temp_float =  atof(rx_data);
 return temp_float;
}

 void write_run_minutes_in_eeprom(void)
 {
   sprintf(temp_buf,"%4lu",min_count_1);
   for(i=0;i<4;i++)
    Write_single_byte_to_eeprom(MINUTES_COUNT_VAL_HIGH_ADD + i, MINUTES_COUNT_VAL_LOW_ADD+i ,temp_buf[i]);
 }
 void write_run_hours_in_eeprom(void)
 {
   sprintf(temp_buf,"%4lu",hour_count_1);
   for(i=0;i<4;i++)
    Write_single_byte_to_eeprom(HOURS_COUNT_VAL_HIGH_ADD + i, HOURS_COUNT_VAL_LOW_ADD+i ,temp_buf[i]);
 }
 void write_maintainance_hour_count_val_in_eeprom(void)
 {
   sprintf(temp_buf,"%4lu",maintainance_hour_count);
   for(i=0;i<4;i++)
    Write_single_byte_to_eeprom(MAINTAINANCE_HOUR_COUNT_VAL_HIGH_ADD + i, MAINTAINANCE_HOUR_COUNT_VAL_LOW_ADD+i ,temp_buf[i]);
 }

float get_excitation_vtg_val_from_eeprom(void)
{
 for(i=0;i<6;i++)
  rx_data[i] = read_single_byte_from_eeprom(APU_BAT_CHARGE_MIN_VTG_DEF_VAL_HIGH_ADD+i , APU_BAT_CHARGE_MIN_VTG_DEF_VAL_LOW_ADD+i);
 rx_data[6] = 0;
 temp_float =  atof(rx_data);
 return temp_float;
}
 unsigned long get_maintainance_hour_count_val_from_eeprom(void)
 {
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(MAINTAINANCE_HOUR_COUNT_VAL_HIGH_ADD+i , MAINTAINANCE_HOUR_COUNT_VAL_LOW_ADD+i);
    rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
//-------------------------------NEW additions with GUI------------------------------------------------------------------------------------------

 unsigned long get_loco_eng_oil_temp_war_val_from_eeprom(void)
 {
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(LOCO_ENG_OIL_TEMP_WARNING_DEF_VAL_HIGH_ADD+i , LOCO_ENG_OIL_TEMP_WARNING_DEF_VAL_LOW_ADD+i);
    rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
  unsigned long get_loco_eng_oil_temp_ala_val_from_eeprom(void)
 {
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(LOCO_ENG_OIL_TEMP_ALARM_DEF_VAL_HIGH_ADD+i , LOCO_ENG_OIL_TEMP_ALARM_DEF_VAL_LOW_ADD+i);
  rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
 unsigned long get_loco_eng_water_temp_war_val_from_eeprom(void)
 {
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(LOCO_ENG_WATER_TEMP_WARNING_DEF_VAL_HIGH_ADD+i , LOCO_ENG_WATER_TEMP_WARNING_DEF_VAL_LOW_ADD+i);
  rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
 unsigned long get_loco_eng_water_temp_ala_val_from_eeprom(void)
 {
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(LOCO_ENG_WATER_TEMP_ALARM_DEF_VAL_HIGH_ADD+i , LOCO_ENG_WATER_TEMP_ALARM_DEF_VAL_LOW_ADD+i);
  rx_data[4] = 0;
  temp_int =  atoi(rx_data);
  return  temp_int;
 }
 unsigned long get_loco_rpm_war_val_from_eeprom(void)
 {
   for(i=0;i<4;i++)
    rx_data[i] = read_single_byte_from_eeprom(LOCO_RPM_WARNING_DEF_VAL_HIGH_ADD+i , LOCO_RPM_WARNING_DEF_VAL_LOW_ADD+i);
   rx_data[4] = 0;
   temp_int =  atoi(rx_data);
   return  temp_int;
 }
 unsigned long get_loco_rpm_ala_val_from_eeprom(void)
 {
   for(i=0;i<4;i++)
    rx_data[i] = read_single_byte_from_eeprom(LOCO_RPM_ALARM_DEF_VAL_HIGH_ADD+i , LOCO_RPM_ALARM_DEF_VAL_LOW_ADD+i);
   rx_data[4] = 0;
   temp_int =  atoi(rx_data);
   return  temp_int;
 }
 unsigned long get_loco_speed_war_val_from_eeprom(void)
 {
   for(i=0;i<4;i++)
    rx_data[i] = read_single_byte_from_eeprom(LOCO_SPEED_WARNING_DEF_VAL_HIGH_ADD+i , LOCO_SPEED_WARNING_DEF_VAL_LOW_ADD+i);
   rx_data[4] = 0;
   temp_int =  atoi(rx_data);
   return  temp_int;
 }
 unsigned long get_loco_speed_ala_val_from_eeprom(void)
 {
   for(i=0;i<4;i++)
    rx_data[i] = read_single_byte_from_eeprom(LOCO_SPEED_ALARM_DEF_VAL_HIGH_ADD+i , LOCO_SPEED_ALARM_DEF_VAL_LOW_ADD+i);
   rx_data[4] = 0;
   temp_int =  atoi(rx_data);
   return  temp_int;
 }

 float get_apu_current_war_val_from_eeprom(void)
 {
  for(i=0;i<6;i++)
    rx_data[i] = read_single_byte_from_eeprom(OVERLOAD_WARNING_DEF_VAL_HIGH_ADD+i , OVERLOAD_WARNING_DEF_VAL_LOW_ADD+i);
  rx_data[6] = 0;
  temp_float =  atof(rx_data);
  return  temp_float;
 }
float get_apu_current_ala_val_from_eeprom(void)
 {
   for(i=0;i<6;i++)
    rx_data[i] = read_single_byte_from_eeprom(OVERLOAD_ALARM_DEF_VAL_HIGH_ADD+i , OVERLOAD_ALARM_DEF_VAL_LOW_ADD+i);
  rx_data[6] = 0;
  temp_float =  atof(rx_data);
  return  temp_float;
 }

float get_apu_74_low_bat_war_val_from_eeprom(void)
 {
  for(i=0;i<6;i++)
    rx_data[i] = read_single_byte_from_eeprom(APU_74_BAT_LOW_VTG_WARNING_DEF_VAL_HIGH_ADD+i , APU_74_BAT_LOW_VTG_WARNING_DEF_VAL_LOW_ADD+i);
 rx_data[6] = 0;
  temp_float =  atof(rx_data);
  return  temp_float;
 }
float get_apu_74_low_bat_ala_val_from_eeprom(void)
 {
   for(i=0;i<6;i++)
    rx_data[i] = read_single_byte_from_eeprom(APU_74_BAT_LOW_VTG_ALARM_DEF_VAL_HIGH_ADD+i , APU_74_BAT_LOW_VTG_ALARM_DEF_VAL_LOW_ADD+i);
 rx_data[6] = 0;
 temp_float =  atof(rx_data);
  return  temp_float;
 }
 float get_apu_74_high_bat_war_val_from_eeprom(void)
 {
    for(i=0;i<6;i++)
    rx_data[i] = read_single_byte_from_eeprom(APU_74_BAT_HIGH_VTG_WARNING_DEF_VAL_HIGH_ADD+i , APU_74_BAT_HIGH_VTG_WARNING_DEF_VAL_LOW_ADD+i);
  rx_data[6] = 0;
  temp_float =  atof(rx_data);
  return  temp_float;
 }
 float get_apu_74_high_bat_ala_val_from_eeprom(void)
 {
   for(i=0;i<6;i++)
    rx_data[i] = read_single_byte_from_eeprom(APU_74_BAT_HIGH_VTG_ALARM_DEF_VAL_HIGH_ADD+i , APU_74_BAT_HIGH_VTG_ALARM_DEF_VAL_LOW_ADD+i);
  rx_data[6] = 0;
  temp_float =  atof(rx_data);
  return  temp_float;
 }
float get_loco_bat_current_war_val_from_eeprom(void)
 {
  for(i=0;i<6;i++)
    rx_data[i] = read_single_byte_from_eeprom(LOCO_BAT_CURRENT_WARNING_DEF_VAL_HIGH_ADD+i , LOCO_BAT_CURRENT_WARNING_DEF_VAL_LOW_ADD+i);
  rx_data[6] = 0;
  temp_float =  atof(rx_data);
  return  temp_float;
 }
float get_loco_bat_current_ala_val_from_eeprom(void)
 {
   for(i=0;i<6;i++)
    rx_data[i] = read_single_byte_from_eeprom(LOCO_BAT_CURRENT_ALARM_DEF_VAL_HIGH_ADD+i , LOCO_BAT_CURRENT_ALARM_DEF_VAL_LOW_ADD+i);
  rx_data[6] = 0;
  temp_float =  atof(rx_data);
  return  temp_float;
 }
//-----------------------------------GUI calibration--------------------------------------------------------------------------------------------------------
float get_GUI_calibrated_val_from_eeprom(unsigned char  high_byte,unsigned char low_byte)
{
  for(i=0;i<5;i++)
   rx_data[i] = read_single_byte_from_eeprom(high_byte+i , low_byte+i);
  rx_data[5] = 0;
  temp_float =  atof(rx_data);
  return temp_float;
}

 //---------------------Additions during code optimization--------------------------------------------------------------------------------

 unsigned long get_run_minutes_val_from_eeprom(void)
 {
   for(i=0;i<4;i++)
    rx_data[i] = read_single_byte_from_eeprom(MINUTES_COUNT_VAL_HIGH_ADD+i , MINUTES_COUNT_VAL_LOW_ADD+i);
   rx_data[4] = 0;
   min_count_1 = atoi(rx_data);

   return min_count_1;
 }

unsigned long get_run_hours_val_from_eeprom(void)
 {
  for(i=0;i<4;i++)
   rx_data[i] = read_single_byte_from_eeprom(HOURS_COUNT_VAL_HIGH_ADD+i , HOURS_COUNT_VAL_LOW_ADD+i);
  rx_data[4]=0;
  hour_count_1= atoi(rx_data);
  return hour_count_1;

 }

 /*
//-----------------------------------------------------------------------------------------------------------------------------------
 float my_atof(uint8_t *rx_string)
 {
   uint8_t temp_b[10];
   float f_sum,f1;
   unsigned long  i_sum,d_sum;
   i_sum = (((rx_string[0]-48)*10000) + ((rx_string[1]- 48)*1000) + ((rx_string[2] - 48)*100) + ((rx_string[4]-48)*10) +  (rx_string[5] -48));
   d_sum =  (((rx_string[4]-48)*10) +  (rx_string[5] -48));

   UART6_PrintString("    i_sum is=   ");
   memset(temp_b,'\0',10);
   sprintf(temp_b,"%ld",i_sum);
   UART6_PrintString(temp_b);


   UART6_PrintString("    f_sum is=   ");
   f_sum = i_sum/100;
   f1 = (d_sum * 0.01);
   f_sum +=f1;
  // f_sum = (float)(i_sum * 0.01);
   memset(temp_b,'\0',10);
   sprintf(temp_b,"%.2f",f_sum);
   UART6_PrintString(temp_b);

 /*
   f_sum = f_sum * 0.01;
   UART6_PrintString("    final =   ");
   memset(temp_b,'\0',10);
   sprintf(temp_b,"%.2f",f_sum);
   UART6_PrintString(temp_b);
 */ // return  f_sum;
 //} */
 
 
 //------------------------------------------------------------------------------------------------------------------------
/*
 void write_eng_fuel_war_ala_spans_in_eeprom(unsigned long span_data , short int id)
{
   if(id==1)
   {
    sprintf(temp_buf,"%d",span_data);
    for(i=0;i<4;i++)
     Write_single_byte_to_eeprom(ENG_FUEL_WARNING_SPAN_LL_VAL_HIGH_ADD + i, ENG_FUEL_WARNING_SPAN_LL_VAL_LOW_ADD+i ,temp_buf[i]);
   }
   else if(id==2)
   {
    sprintf(temp_buf,"%d",span_data);
    for(i=0;i<4;i++)
     Write_single_byte_to_eeprom(ENG_FUEL_WARNING_SPAN_HL_VAL_HIGH_ADD + i, ENG_FUEL_WARNING_SPAN_HL_VAL_LOW_ADD+i ,temp_buf[i]);
   }
   else if(id==3)
   {
    sprintf(temp_buf,"%d",span_data);
    for(i=0;i<4;i++)
     Write_single_byte_to_eeprom(ENG_FUEL_ALARM_SPAN_LL_VAL_HIGH_ADD + i, ENG_FUEL_ALARM_SPAN_LL_VAL_LOW_ADD+i ,temp_buf[i]);
   }
   else if(id==4)
   {
    sprintf(temp_buf,"%d",span_data);
    for(i=0;i<4;i++)
     Write_single_byte_to_eeprom(ENG_FUEL_ALARM_SPAN_HL_VAL_HIGH_ADD + i, ENG_FUEL_ALARM_SPAN_HL_VAL_LOW_ADD+i ,temp_buf[i]);
   }
}
 void write_all_default_values_in_eeprom(void)
 {//------------------------------PARAMETERS RELATED TO ENGINE----------------------------------------------------------------

   Write_single_byte_to_eeprom(MINUTES_COUNT_VAL_HIGH_ADD , MINUTES_COUNT_VAL_LOW_ADD ,'0');    Delay_ms(1);
   Write_single_byte_to_eeprom(MINUTES_COUNT_VAL_HIGH_ADD +1, MINUTES_COUNT_VAL_LOW_ADD +1,'0');  Delay_ms(1);
   Write_single_byte_to_eeprom(MINUTES_COUNT_VAL_HIGH_ADD +2, MINUTES_COUNT_VAL_LOW_ADD +2,'0');  Delay_ms(1);
   Write_single_byte_to_eeprom(MINUTES_COUNT_VAL_HIGH_ADD +3, MINUTES_COUNT_VAL_LOW_ADD +3,'0');  Delay_ms(1);

   Write_single_byte_to_eeprom(HOURS_COUNT_VAL_HIGH_ADD , HOURS_COUNT_VAL_LOW_ADD+i ,'0');      Delay_ms(1);
   Write_single_byte_to_eeprom(HOURS_COUNT_VAL_HIGH_ADD + 1, HOURS_COUNT_VAL_LOW_ADD+1 ,'0');   Delay_ms(1);
   Write_single_byte_to_eeprom(HOURS_COUNT_VAL_HIGH_ADD + 2, HOURS_COUNT_VAL_LOW_ADD+2 ,'0');   Delay_ms(1);
   Write_single_byte_to_eeprom(HOURS_COUNT_VAL_HIGH_ADD + 3, HOURS_COUNT_VAL_LOW_ADD+3 ,'0');   Delay_ms(1);

    Write_single_byte_to_eeprom(ENG_FUEL_TANK_DEF_VAL_HIGH_ADD , ENG_FUEL_TANK_DEF_VAL_LOW_ADD ,'1');  Delay_ms(1);
    Write_single_byte_to_eeprom(ENG_FUEL_TANK_DEF_VAL_HIGH_ADD + 1, ENG_FUEL_TANK_DEF_VAL_LOW_ADD+1 ,'8'); Delay_ms(1);
    Write_single_byte_to_eeprom(ENG_FUEL_TANK_DEF_VAL_HIGH_ADD + 2, ENG_FUEL_TANK_DEF_VAL_LOW_ADD+2 ,'.');Delay_ms(1);
    Write_single_byte_to_eeprom(ENG_FUEL_TANK_DEF_VAL_HIGH_ADD + 3, ENG_FUEL_TANK_DEF_VAL_LOW_ADD+3 ,'7');Delay_ms(1);

   Write_single_byte_to_eeprom(ENG_FUEL_LEVEL_SEL_DEF_VAL_HIGH_ADD , ENG_FUEL_LEVEL_SEL_DEF_VAL_LOW_ADD , '1'); //sensor
   Delay_ms(1);

   Write_single_byte_to_eeprom(ENG_FUEL_LEVEL_CHECK_DEF_VAL_HIGH_ADD , ENG_FUEL_LEVEL_CHECK_DEF_VAL_LOW_ADD , '1'); //ENABLE
   Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_FUEL_WARNING_DEF_VAL_HIGH_ADD , ENG_FUEL_WARNING_DEF_VAL_LOW_ADD ,'0' );  Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_FUEL_WARNING_DEF_VAL_HIGH_ADD + 1, ENG_FUEL_WARNING_DEF_VAL_LOW_ADD+1 ,'0' );   Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_FUEL_WARNING_DEF_VAL_HIGH_ADD + 2, ENG_FUEL_WARNING_DEF_VAL_LOW_ADD+2 ,'0' );   Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_FUEL_WARNING_DEF_VAL_HIGH_ADD + 3, ENG_FUEL_WARNING_DEF_VAL_LOW_ADD+3 ,'0' );   Delay_ms(1);


   Write_single_byte_to_eeprom(ENG_FUEL_ALARM_DEF_VAL_HIGH_ADD , ENG_FUEL_ALARM_DEF_VAL_LOW_ADD ,'0' );
   Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_FUEL_ALARM_DEF_VAL_HIGH_ADD + 1, ENG_FUEL_ALARM_DEF_VAL_LOW_ADD +1 ,'0' );Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_FUEL_ALARM_DEF_VAL_HIGH_ADD + 2, ENG_FUEL_ALARM_DEF_VAL_LOW_ADD +2 ,'1' ); Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_FUEL_ALARM_DEF_VAL_HIGH_ADD + 3, ENG_FUEL_ALARM_DEF_VAL_LOW_ADD +3 ,'2' );  Delay_ms(1);

   Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_COOL_TEMP_SEL_DEF_VAL_HIGH_ADD , ENG_COOL_TEMP_SEL_DEF_VAL_LOW_ADD , '1'); //sensor
   Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_COOL_TEMP_CHECK_DEF_VAL_HIGH_ADD , ENG_COOL_TEMP_CHECK_DEF_VAL_LOW_ADD , '1'); //ENABLE
   Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_COOL_TEMP_WARNING_DEF_VAL_HIGH_ADD , ENG_COOL_TEMP_WARNING_DEF_VAL_LOW_ADD ,'0' );     Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_COOL_TEMP_WARNING_DEF_VAL_HIGH_ADD + 1, ENG_COOL_TEMP_WARNING_DEF_VAL_LOW_ADD+1 ,'0' );   Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_COOL_TEMP_WARNING_DEF_VAL_HIGH_ADD + 2, ENG_COOL_TEMP_WARNING_DEF_VAL_LOW_ADD+2 ,'9' );   Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_COOL_TEMP_WARNING_DEF_VAL_HIGH_ADD + 3, ENG_COOL_TEMP_WARNING_DEF_VAL_LOW_ADD+3 ,'0' );   Delay_ms(1);

   Write_single_byte_to_eeprom(ENG_COOL_TEMP_ALARM_DEF_VAL_HIGH_ADD , ENG_COOL_TEMP_ALARM_DEF_VAL_LOW_ADD ,'1' );   Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_COOL_TEMP_ALARM_DEF_VAL_HIGH_ADD + 1, ENG_COOL_TEMP_ALARM_DEF_VAL_LOW_ADD+1 ,'0' );  Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_COOL_TEMP_ALARM_DEF_VAL_HIGH_ADD + 2, ENG_COOL_TEMP_ALARM_DEF_VAL_LOW_ADD+2 ,'5' );  Delay_ms(1);

   Write_single_byte_to_eeprom(ENG_OIL_PRESS_SEL_DEF_VAL_HIGH_ADD , ENG_OIL_PRESS_SEL_DEF_VAL_LOW_ADD , '1'); //sensor
   Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_OIL_PRESS_ALARM_DEF_VAL_HIGH_ADD , ENG_OIL_PRESS_ALARM_DEF_VAL_LOW_ADD , '0'); //2 bar
   Write_single_byte_to_eeprom(ENG_OIL_PRESS_ALARM_DEF_VAL_HIGH_ADD , ENG_OIL_PRESS_ALARM_DEF_VAL_LOW_ADD , '0'); //2 bar
      Write_single_byte_to_eeprom(ENG_OIL_PRESS_ALARM_DEF_VAL_HIGH_ADD , ENG_OIL_PRESS_ALARM_DEF_VAL_LOW_ADD , '2'); //2 bar
         Write_single_byte_to_eeprom(ENG_OIL_PRESS_ALARM_DEF_VAL_HIGH_ADD , ENG_OIL_PRESS_ALARM_DEF_VAL_LOW_ADD , '.'); //2 bar
            Write_single_byte_to_eeprom(ENG_OIL_PRESS_ALARM_DEF_VAL_HIGH_ADD , ENG_OIL_PRESS_ALARM_DEF_VAL_LOW_ADD , '6'); //2 bar
               Write_single_byte_to_eeprom(ENG_OIL_PRESS_ALARM_DEF_VAL_HIGH_ADD , ENG_OIL_PRESS_ALARM_DEF_VAL_LOW_ADD , '8'); //2 bar

   Delay_ms(1);
   sprintf(temp_buf,"%.2f",2.1);
   for(i=0;i<3;i++)
   {
    Write_single_byte_to_eeprom(ENG_OIL_PRESS_WARNING_DEF_VAL_HIGH_ADD + i, ENG_OIL_PRESS_WARNING_DEF_VAL_LOW_ADD+i ,temp_buf[i]);
    Delay_ms(1);
  }
  //---------------------------PARAMETERS RELATED TO COMPRESSOR--------------------------------------------------------------

   Write_single_byte_to_eeprom(COMP_AIR_PRESS_SEL_DEF_VAL_HIGH_ADD , COMP_AIR_PRESS_SEL_DEF_VAL_LOW_ADD , '1'); //sensor
   Delay_ms(1);
   Write_single_byte_to_eeprom(COMP_AIR_PRESS_ALARM_DEF_VAL_HIGH_ADD , COMP_AIR_PRESS_ALARM_DEF_VAL_LOW_ADD ,'1' );
   Delay_ms(1);
   Write_single_byte_to_eeprom(COMP_AIR_PRESS_ALARM_DEF_VAL_HIGH_ADD + 1, COMP_AIR_PRESS_ALARM_DEF_VAL_LOW_ADD+1 ,'0' );
   Delay_ms(1);
   Write_single_byte_to_eeprom(COMP_AIR_PRESS_WARNING_DEF_VAL_HIGH_ADD , COMP_AIR_PRESS_WARNING_DEF_VAL_LOW_ADD , '8');
   Delay_ms(1);
   Write_single_byte_to_eeprom(COMP_AIR_TEMP_SEL_DEF_VAL_HIGH_ADD , COMP_AIR_TEMP_SEL_DEF_VAL_LOW_ADD , '1'); //sensor
   Delay_ms(1);
   Write_single_byte_to_eeprom(COMP_AIR_TEMP_CHECK_DEF_VAL_HIGH_ADD , COMP_AIR_TEMP_CHECK_DEF_VAL_LOW_ADD , '1'); //ENABLE
   Delay_ms(1);
   Write_single_byte_to_eeprom(COMP_AIR_TEMP_WARNING_DEF_VAL_HIGH_ADD , COMP_AIR_TEMP_WARNING_DEF_VAL_LOW_ADD ,'9' );   Delay_ms(1);
   Write_single_byte_to_eeprom(COMP_AIR_TEMP_WARNING_DEF_VAL_HIGH_ADD + 1, COMP_AIR_TEMP_WARNING_DEF_VAL_LOW_ADD+1 ,'0' );  Delay_ms(1);

   Write_single_byte_to_eeprom(ENG_COOL_TEMP_ALARM_DEF_VAL_HIGH_ADD , COMP_AIR_TEMP_ALARM_DEF_VAL_LOW_ADD ,'1' );   Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_COOL_TEMP_ALARM_DEF_VAL_HIGH_ADD + 1, COMP_AIR_TEMP_ALARM_DEF_VAL_LOW_ADD+1 ,'0' ); Delay_ms(1);
   Write_single_byte_to_eeprom(ENG_COOL_TEMP_ALARM_DEF_VAL_HIGH_ADD + 2, COMP_AIR_TEMP_ALARM_DEF_VAL_LOW_ADD+2 ,'5' );  Delay_ms(1);

   Write_single_byte_to_eeprom(COMP_OIL_TEMP_SEL_DEF_VAL_HIGH_ADD , COMP_OIL_TEMP_SEL_DEF_VAL_LOW_ADD , '1'); //sensor
  Delay_ms(1);
   Write_single_byte_to_eeprom(COMP_OIL_TEMP_CHECK_DEF_VAL_HIGH_ADD , COMP_OIL_TEMP_CHECK_DEF_VAL_LOW_ADD , '1'); //ENABLE
   Delay_ms(1);
   Write_single_byte_to_eeprom(COMP_OIL_TEMP_WARNING_DEF_VAL_HIGH_ADD , COMP_OIL_TEMP_WARNING_DEF_VAL_LOW_ADD ,'9' );  Delay_ms(1);
   Write_single_byte_to_eeprom(COMP_OIL_TEMP_WARNING_DEF_VAL_HIGH_ADD + 1, COMP_OIL_TEMP_WARNING_DEF_VAL_LOW_ADD+1 ,'0' );   Delay_ms(1);

   Write_single_byte_to_eeprom(COMP_OIL_TEMP_ALARM_DEF_VAL_HIGH_ADD , COMP_OIL_TEMP_ALARM_DEF_VAL_LOW_ADD ,'1' );   Delay_ms(1);
   Write_single_byte_to_eeprom(COMP_OIL_TEMP_ALARM_DEF_VAL_HIGH_ADD + 1, COMP_OIL_TEMP_ALARM_DEF_VAL_LOW_ADD+1 ,'0' ); Delay_ms(1);
   Write_single_byte_to_eeprom(COMP_OIL_TEMP_ALARM_DEF_VAL_HIGH_ADD + 2, COMP_OIL_TEMP_ALARM_DEF_VAL_LOW_ADD+2 ,'5' );  Delay_ms(1);

//------------------------PARAMETERS RELATED TO LOCO ENGINE----------------------------------------------------------------
   Write_single_byte_to_eeprom(MR_AIR_PRESS_CHECK_DEF_VAL_HIGH_ADD , MR_AIR_PRESS_CHECK_DEF_VAL_LOW_ADD ,'1'); //ENABLE
     Delay_ms(1);
   Write_single_byte_to_eeprom(MR_AIR_PRESS_WARNING_DEF_VAL_HIGH_ADD , MR_AIR_PRESS_WARNING_DEF_VAL_LOW_ADD , '8');
    Delay_ms(1);
   Write_single_byte_to_eeprom(MR_AIR_PRESS_ALARM_DEF_VAL_HIGH_ADD , MR_AIR_PRESS_ALARM_DEF_VAL_LOW_ADD ,'1' );   Delay_ms(1);
   Write_single_byte_to_eeprom(MR_AIR_PRESS_ALARM_DEF_VAL_HIGH_ADD + 1, MR_AIR_PRESS_ALARM_DEF_VAL_LOW_ADD+1 ,'0' );
    Delay_ms(1);
   Write_single_byte_to_eeprom(BCP_PRESS_CHECK_DEF_VAL_HIGH_ADD , BCP_PRESS_CHECK_DEF_VAL_LOW_ADD , '1');  //ENABLE
    Delay_ms(1);

   Write_single_byte_to_eeprom(BCP_PRESS_WARNING_DEF_VAL_HIGH_ADD , BCP_PRESS_WARNING_DEF_VAL_LOW_ADD ,'2' );  // Delay_ms(1);
   Write_single_byte_to_eeprom(BCP_PRESS_WARNING_DEF_VAL_HIGH_ADD+1 , BCP_PRESS_WARNING_DEF_VAL_LOW_ADD+1 ,'.' );  // Delay_ms(1);
   Write_single_byte_to_eeprom(BCP_PRESS_WARNING_DEF_VAL_HIGH_ADD+2 , BCP_PRESS_WARNING_DEF_VAL_LOW_ADD+2 ,'0' );  // Delay_ms(1);

   Write_single_byte_to_eeprom(BCP_PRESS_ALARM_DEF_VAL_HIGH_ADD , BCP_PRESS_ALARM_DEF_VAL_LOW_ADD ,'1' );   //  Delay_ms(1);
   Write_single_byte_to_eeprom(BCP_PRESS_ALARM_DEF_VAL_HIGH_ADD + 1, BCP_PRESS_ALARM_DEF_VAL_LOW_ADD+1 ,'.' ); // Delay_ms(1);
   Write_single_byte_to_eeprom(BCP_PRESS_ALARM_DEF_VAL_HIGH_ADD + 2, BCP_PRESS_ALARM_DEF_VAL_LOW_ADD+2 ,'8' );//  Delay_ms(1);

   Write_single_byte_to_eeprom(IDLE_LOCO_SPEED_CHECK_DEF_VAL_HIGH_ADD , IDLE_LOCO_SPEED_CHECK_DEF_VAL_LOW_ADD , '1'); //ENABLE
    Delay_ms(1);
   Write_single_byte_to_eeprom(RESERVE_HANDLE_POS_CHECK_DEF_VAL_HIGH_ADD , RESERVE_HANDLE_POS_CHECK_DEF_VAL_LOW_ADD , '1'); //ENABLE
    Delay_ms(1);
   Write_single_byte_to_eeprom(LOCO_BAT_VTG_CHECK_DEF_VAL_HIGH_ADD , LOCO_BAT_VTG_CHECK_DEF_VAL_LOW_ADD , '1'); //ENABLE
     Delay_ms(1);
   Write_single_byte_to_eeprom(LOCO_BAT_HIGH_VTG_WARNING_DEF_VAL_HIGH_ADD , LOCO_BAT_HIGH_VTG_WARNING_DEF_VAL_LOW_ADD ,'7' ); Delay_ms(1);
   Write_single_byte_to_eeprom(LOCO_BAT_HIGH_VTG_WARNING_DEF_VAL_HIGH_ADD + 1, LOCO_BAT_HIGH_VTG_WARNING_DEF_VAL_LOW_ADD+1 ,'4' );
     Delay_ms(1);
   Write_single_byte_to_eeprom(LOCO_BAT_HIGH_VTG_ALARM_DEF_VAL_HIGH_ADD , LOCO_BAT_HIGH_VTG_ALARM_DEF_VAL_LOW_ADD ,'8' );  Delay_ms(1);
   Write_single_byte_to_eeprom(LOCO_BAT_HIGH_VTG_ALARM_DEF_VAL_HIGH_ADD + 1, LOCO_BAT_HIGH_VTG_ALARM_DEF_VAL_LOW_ADD+1 ,'4' );
    Delay_ms(1);
   Write_single_byte_to_eeprom(LOCO_BAT_HIGH_VTG_DELAY_DEF_VAL_HIGH_ADD , LOCO_BAT_HIGH_VTG_DELAY_DEF_VAL_LOW_ADD , 1);
    Delay_ms(1);
   Write_single_byte_to_eeprom(LOCO_BAT_LOW_VTG_WARNING_DEF_VAL_HIGH_ADD , LOCO_BAT_LOW_VTG_WARNING_DEF_VAL_LOW_ADD ,'7' );    Delay_ms(1);
   Write_single_byte_to_eeprom(LOCO_BAT_LOW_VTG_WARNING_DEF_VAL_HIGH_ADD + 1, LOCO_BAT_LOW_VTG_WARNING_DEF_VAL_LOW_ADD+1 ,'0' );
    Delay_ms(1);
   Write_single_byte_to_eeprom(LOCO_BAT_LOW_VTG_ALARM_DEF_VAL_HIGH_ADD , LOCO_BAT_LOW_VTG_ALARM_DEF_VAL_LOW_ADD ,'6' );   Delay_ms(1);
   Write_single_byte_to_eeprom(LOCO_BAT_LOW_VTG_ALARM_DEF_VAL_HIGH_ADD + 1, LOCO_BAT_LOW_VTG_ALARM_DEF_VAL_LOW_ADD+1,'4');   Delay_ms(1);

   Write_single_byte_to_eeprom(LOCO_BAT_LOW_VTG_DELAY_DEF_VAL_HIGH_ADD , LOCO_BAT_LOW_VTG_DELAY_DEF_VAL_LOW_ADD , '1');
    Delay_ms(1);
   Write_single_byte_to_eeprom(LOCO_SPEED_WARNING_DEF_VAL_HIGH_ADD , LOCO_SPEED_WARNING_DEF_VAL_LOW_ADD ,'4' );   Delay_ms(1);
   Write_single_byte_to_eeprom(LOCO_SPEED_WARNING_DEF_VAL_HIGH_ADD + 1, LOCO_SPEED_WARNING_DEF_VAL_LOW_ADD+1 ,'0' ); Delay_ms(1);
   Write_single_byte_to_eeprom(LOCO_SPEED_WARNING_DEF_VAL_HIGH_ADD + 2, LOCO_SPEED_WARNING_DEF_VAL_LOW_ADD+2 ,'0' );  Delay_ms(1);

   Write_single_byte_to_eeprom(LOCO_SPEED_ALARM_DEF_VAL_HIGH_ADD , LOCO_SPEED_ALARM_DEF_VAL_LOW_ADD ,'5' );     Delay_ms(1);
   Write_single_byte_to_eeprom(LOCO_SPEED_ALARM_DEF_VAL_HIGH_ADD + 1, LOCO_SPEED_ALARM_DEF_VAL_LOW_ADD+1 ,'0' );  Delay_ms(1);
   Write_single_byte_to_eeprom(LOCO_SPEED_ALARM_DEF_VAL_HIGH_ADD + 2, LOCO_SPEED_ALARM_DEF_VAL_LOW_ADD+2 ,'0' );  Delay_ms(1);

   Write_single_byte_to_eeprom(OVERLOAD_CHECK_DEF_VAL_HIGH_ADD , OVERLOAD_CHECK_DEF_VAL_LOW_ADD , '1');   //ENABLE
    Delay_ms(1);
   Write_single_byte_to_eeprom(OVERLOAD_WARNING_DEF_VAL_HIGH_ADD , OVERLOAD_WARNING_DEF_VAL_LOW_ADD ,'8');    Delay_ms(1);
   Write_single_byte_to_eeprom(OVERLOAD_WARNING_DEF_VAL_HIGH_ADD + 1, OVERLOAD_WARNING_DEF_VAL_LOW_ADD+1 ,'0');    Delay_ms(1);

   Write_single_byte_to_eeprom(OVERLOAD_ALARM_DEF_VAL_HIGH_ADD , OVERLOAD_ALARM_DEF_VAL_LOW_ADD ,'9' );      Delay_ms(1);
   Write_single_byte_to_eeprom(OVERLOAD_ALARM_DEF_VAL_HIGH_ADD + 1, OVERLOAD_ALARM_DEF_VAL_LOW_ADD+1 ,'0' );   Delay_ms(1);

   Write_single_byte_to_eeprom(OVERLOAD_DELAY_DEF_VAL_HIGH_ADD , OVERLOAD_DELAY_DEF_VAL_LOW_ADD ,'1' );      Delay_ms(1);
   Write_single_byte_to_eeprom(OVERLOAD_DELAY_DEF_VAL_HIGH_ADD + 1, OVERLOAD_DELAY_DEF_VAL_LOW_ADD+1 ,'0' );   Delay_ms(1);


//------------------------OTHER PARAMETERS---------------------------------------------------------------------------------

   Write_single_byte_to_eeprom(FUEL_SELECT_DEF_VAL_HIGH_ADD , FUEL_SELECT_DEF_VAL_LOW_ADD , '1');  //  1==ETR MODE  0==ETS
    Delay_ms(1);
   Write_single_byte_to_eeprom(APU_BAT_VTG_DEF_VAL_HIGH_ADD , APU_BAT_VTG_DEF_VAL_LOW_ADD , '1');      // ENABLE
     Delay_ms(1);
   sprintf(temp_buf,"%.2f",9.5);
   for(i=0;i<3;i++)
   {
    Write_single_byte_to_eeprom(APU_LOW_BAT_WARNING_DEF_VAL_HIGH_ADD + i, APU_LOW_BAT_WARNING_DEF_VAL_LOW_ADD+i ,temp_buf[i]);
     Delay_ms(1);
    }
   sprintf(temp_buf,"%.2f",15.5);
   for(i=0;i<4;i++)
   {
    Write_single_byte_to_eeprom(APU_HIGH_BAT_WARNING_DEF_VAL_HIGH_ADD + i, APU_HIGH_BAT_WARNING_DEF_VAL_LOW_ADD+i ,temp_buf[i]);
     Delay_ms(1);
    }
   sprintf(temp_buf,"%.2f",15.5);
    for(i=0;i<4;i++)
    {
      Write_single_byte_to_eeprom(APU_MAX_BAT_WARNING_DEF_VAL_HIGH_ADD + i, APU_MAX_BAT_WARNING_DEF_VAL_LOW_ADD+i ,temp_buf[i]);
      Delay_ms(1);
    }
   Write_single_byte_to_eeprom(EXCITE_CHECK_DEF_VAL_HIGH_ADD , EXCITE_CHECK_DEF_VAL_LOW_ADD , 1);      // ENABLE
    Delay_ms(1);
   Write_single_byte_to_eeprom(APU_BAT_CHARGE_MIN_VTG_DEF_VAL_HIGH_ADD , APU_BAT_CHARGE_MIN_VTG_DEF_VAL_LOW_ADD ,'1' );   Delay_ms(1);      //excitation vtg limit
   Write_single_byte_to_eeprom(APU_BAT_CHARGE_MIN_VTG_DEF_VAL_HIGH_ADD + 1, APU_BAT_CHARGE_MIN_VTG_DEF_VAL_LOW_ADD+1 ,'0' );  Delay_ms(1);
   Write_single_byte_to_eeprom(APU_BAT_CHARGE_MIN_VTG_DEF_VAL_HIGH_ADD + 2, APU_BAT_CHARGE_MIN_VTG_DEF_VAL_LOW_ADD+2 ,'.' );  Delay_ms(1);
   Write_single_byte_to_eeprom(APU_BAT_CHARGE_MIN_VTG_DEF_VAL_HIGH_ADD + 3, APU_BAT_CHARGE_MIN_VTG_DEF_VAL_LOW_ADD+3 ,'0' );  Delay_ms(1);
   Write_single_byte_to_eeprom(APU_BAT_CHARGE_MIN_VTG_DEF_VAL_HIGH_ADD + 4, APU_BAT_CHARGE_MIN_VTG_DEF_VAL_LOW_ADD+4 ,'0' );  Delay_ms(1);

   Write_single_byte_to_eeprom(COOLING_TIME_DEF_VAL_HIGH_ADD , COOLING_TIME_DEF_VAL_LOW_ADD ,'1' );   Delay_ms(1);
   Write_single_byte_to_eeprom(COOLING_TIME_DEF_VAL_HIGH_ADD +1, COOLING_TIME_DEF_VAL_LOW_ADD+1 ,'0');   Delay_ms(1);

//   Write_single_byte_to_eeprom(MAINTAINANCE_TIME_DEF_VAL_HIGH_ADD , MAINTAINANCE_TIME_DEF_VAL_LOW_ADD ,'9' );   Delay_ms(1);
//   Write_single_byte_to_eeprom(MAINTAINANCE_TIME_DEF_VAL_HIGH_ADD + 1, MAINTAINANCE_TIME_DEF_VAL_LOW_ADD+1 ,'9' );   Delay_ms(1);
//   Write_single_byte_to_eeprom(MAINTAINANCE_TIME_DEF_VAL_HIGH_ADD + 2, MAINTAINANCE_TIME_DEF_VAL_LOW_ADD+2 ,'9' );   Delay_ms(1);

   Write_single_byte_to_eeprom(HOOTER_ON_TIME_DEF_VAL_HIGH_ADD , HOOTER_ON_TIME_DEF_VAL_LOW_ADD , '5');    Delay_ms(1);

   Write_single_byte_to_eeprom(RPM_CHECK_DEF_VAL_HIGH_ADD , RPM_CHECK_DEF_VAL_LOW_ADD , '1');   //ENABLE
    Delay_ms(1);
   Write_single_byte_to_eeprom(HIGH_RPM_DELAY_DEF_VAL_HIGH_ADD , HIGH_RPM_DELAY_DEF_VAL_LOW_ADD ,'5');
    Delay_ms(1);
   Write_single_byte_to_eeprom(LOW_RPM_DELAY_DEF_VAL_HIGH_ADD , LOW_RPM_DELAY_DEF_VAL_LOW_ADD , '5');
    Delay_ms(1);
   Write_single_byte_to_eeprom(CRANK_TIME_DEF_VAL_HIGH_ADD , CRANK_TIME_DEF_VAL_LOW_ADD , '0');   Delay_ms(1);
   Write_single_byte_to_eeprom(CRANK_TIME_DEF_VAL_HIGH_ADD +1 , CRANK_TIME_DEF_VAL_LOW_ADD+1 , '4'); Delay_ms(1);

   Write_single_byte_to_eeprom(CRANK_REST_TIME_DEF_VAL_HIGH_ADD , CRANK_REST_TIME_DEF_VAL_LOW_ADD , '1');   Delay_ms(1);
   Write_single_byte_to_eeprom(CRANK_REST_TIME_DEF_VAL_HIGH_ADD +1 , CRANK_REST_TIME_DEF_VAL_LOW_ADD +1, '0');   Delay_ms(1);

   Write_single_byte_to_eeprom(CRANK_TRIES_DEF_VAL_HIGH_ADD  , CRANK_TRIES_DEF_VAL_LOW_ADD , '0');   Delay_ms(1);
   Write_single_byte_to_eeprom(CRANK_TRIES_DEF_VAL_HIGH_ADD + 1, CRANK_TRIES_DEF_VAL_LOW_ADD +1 , '3');   Delay_ms(1);

   Write_single_byte_to_eeprom(RUN_UP_TIME_DEF_VAL_HIGH_ADD , RUN_UP_TIME_DEF_VAL_LOW_ADD , '1'); Delay_ms(1);
   Write_single_byte_to_eeprom(RUN_UP_TIME_DEF_VAL_HIGH_ADD +1, RUN_UP_TIME_DEF_VAL_LOW_ADD +1, '0'); Delay_ms(1);

   Write_single_byte_to_eeprom(WARM_UP_TIME_DEF_VAL_HIGH_ADD , WARM_UP_TIME_DEF_VAL_LOW_ADD , '1');     Delay_ms(1);
   Write_single_byte_to_eeprom(WARM_UP_TIME_DEF_VAL_HIGH_ADD +1 , WARM_UP_TIME_DEF_VAL_LOW_ADD +1, '0');     Delay_ms(1);

   Write_single_byte_to_eeprom(STOP_TIME_DEF_VAL_HIGH_ADD , STOP_TIME_DEF_VAL_LOW_ADD ,'2' );  Delay_ms(1);
   Write_single_byte_to_eeprom(STOP_TIME_DEF_VAL_HIGH_ADD +1, STOP_TIME_DEF_VAL_LOW_ADD+1 ,'0');  Delay_ms(1);

   Write_single_byte_to_eeprom(STOP_REST_TIME_DEF_VAL_HIGH_ADD , STOP_REST_TIME_DEF_VAL_LOW_ADD , '0');      Delay_ms(1);
   Write_single_byte_to_eeprom(STOP_REST_TIME_DEF_VAL_HIGH_ADD +1, STOP_REST_TIME_DEF_VAL_LOW_ADD +1, '3');  Delay_ms(1);

   Write_single_byte_to_eeprom(SITE_ID_DEF_VAL_HIGH_ADD , SITE_ID_DEF_VAL_LOW_ADD , '1');
     Delay_ms(1);
   Write_single_byte_to_eeprom(STOP_TRIES_DEF_VAL_HIGH_ADD , STOP_TRIES_DEF_VAL_LOW_ADD ,'0');    Delay_ms(1);
   Write_single_byte_to_eeprom(STOP_TRIES_DEF_VAL_HIGH_ADD +1, STOP_TRIES_DEF_VAL_LOW_ADD +1,'3');   Delay_ms(1);
 //-------------------EXTRA ADDITIONS---------------------------------------------------------------------------------------------------------------------------------------------------
   Write_single_byte_to_eeprom(GEAR_TEETH_DEF_VAL_HIGH_ADD , GEAR_TEETH_DEF_VAL_LOW_ADD ,'1' );     Delay_ms(1);
   Write_single_byte_to_eeprom(GEAR_TEETH_DEF_VAL_HIGH_ADD + 1, GEAR_TEETH_DEF_VAL_LOW_ADD+1 ,'1' );    Delay_ms(1);
   Write_single_byte_to_eeprom(GEAR_TEETH_DEF_VAL_HIGH_ADD + 2, GEAR_TEETH_DEF_VAL_LOW_ADD+2 ,'6' );    Delay_ms(1);

   Write_single_byte_to_eeprom(PREHEAT_COUNTER_DEF_VAL_HIGH_ADD , PREHEAT_COUNTER_DEF_VAL_LOW_ADD ,'0');   Delay_ms(1);
   Write_single_byte_to_eeprom(PREHEAT_COUNTER_DEF_VAL_HIGH_ADD +1, PREHEAT_COUNTER_DEF_VAL_LOW_ADD +1,'3');   Delay_ms(1);

   Write_single_byte_to_eeprom(CHARGE_ALT_FREQUENCY_DEF_VAL_HIGH_ADD , CHARGE_ALT_FREQUENCY_DEF_VAL_LOW_ADD ,'0');    Delay_ms(1);
   Write_single_byte_to_eeprom(CHARGE_ALT_FREQUENCY_DEF_VAL_HIGH_ADD+1 , CHARGE_ALT_FREQUENCY_DEF_VAL_LOW_ADD+1 ,'7');  Delay_ms(1);
   Write_single_byte_to_eeprom(CHARGE_ALT_FREQUENCY_DEF_VAL_HIGH_ADD+2 , CHARGE_ALT_FREQUENCY_DEF_VAL_LOW_ADD+2 ,'4'); Delay_ms(1);
 }
 */