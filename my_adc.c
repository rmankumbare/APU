#include "headers.h"
#include "stdint.h"
#include "eeprom.h"
#include "my_adc.h"
#include "my_uart.h"
#include "war_ala_check.h"
#include "freq_capture.h"
//--------------------------GLOBAL VARIABLES---------------------------------------------------------------------------------------------------------------
uint8_t gui_temp_char ='0';
volatile short int d;
unsigned long adc_value,temp_int_value;
unsigned long eng_fuel_ala_span_ll,eng_fuel_ala_span_hl,eng_fuel_war_span_ll,eng_fuel_war_span_hl;
float temp_value,GUI_value,diff,eng_fuel_tank_def_value;

//---------------------------EXTERN  VARIABLES-------------------------------------------------------------------------------------------------------------------------
extern  volatile long i;
extern unsigned long amb_temp;
extern uint8_t flag_llop_open,flag_eng_temp_open,flag_apu_current_open,calibration_done_flag,flag_rectifier_temp_open;

extern float gui_apu_74V_bat_vtg, gui_apu_current;
extern float gui_apu_bat_vtg,gui_loco_bat_vtg,gui_loco_bat_current, gui_comp_air_press;

 //---------------------------------------------------------------------------------------------------------------------------------------------------


void my_adc_init(void)
{
  ADC1_Init();  // Initialize ADC module with default settings
  ADC2_Init();
  ADC3_Init();
  ADC1_SMPR1 = 0xFFFFFFFF;
  ADC1_SMPR2 = 0xFFFFFFFF;

  ADC2_SMPR1 = 0xFFFFFFFF;
  ADC2_SMPR2 = 0xFFFFFFFF;

  ADC3_SMPR1 = 0xFFFFFFFF;
  ADC3_SMPR2 = 0xFFFFFFFF;

  ADC_CCR |= 1<<16|1<<17;

  ADC_Set_Input_Channel(_ADC_CHANNEL_0 | _ADC_CHANNEL_1|_ADC_CHANNEL_2 | _ADC_CHANNEL_3|_ADC_CHANNEL_4 | _ADC_CHANNEL_5);
  ADC_Set_Input_Channel(_ADC_CHANNEL_6 | _ADC_CHANNEL_7|_ADC_CHANNEL_8 | _ADC_CHANNEL_9|_ADC_CHANNEL_10 | _ADC_CHANNEL_11);
  ADC_Set_Input_Channel(_ADC_CHANNEL_12 | _ADC_CHANNEL_13|_ADC_CHANNEL_14 | _ADC_CHANNEL_15|_ADC_CHANNEL_16 | _ADC_CHANNEL_17);
}
 //-------------------------------------------------ENGINE FUEL LEVEL----------------------------------------------------------------------------
float scan_engine_fuel_level()
{
  uint8_t rx_data[5];
  temp_value=0;

    temp_int_value=0;
    for(i=0;i<AVG_COUNT;i++)
    {
     adc_value = ADC_ENG_FUEL_LEVEL;
     temp_int_value += adc_value;
    }
    adc_value=temp_int_value/AVG_COUNT;
    for(i=0;i<5;i++)
    {
      rx_data[i] = read_single_byte_from_eeprom(ENG_FUEL_TANK_DEF_VAL_HIGH_ADD+i , ENG_FUEL_TANK_DEF_VAL_LOW_ADD+i);
    }
  
    eng_fuel_tank_def_value =  atof(rx_data);

    if(adc_value > 3102)
      temp_value = eng_fuel_tank_def_value;
    else if(adc_value < 20)
      temp_value=0.0;
    else
      temp_value= (((float)eng_fuel_tank_def_value*(float)adc_value)/3102.0);     //FUEL LEVEL 0 TO 35 LITERS ADC INP VTG IS 0 TO 2.50V

   calibration_done_flag = read_single_byte_from_eeprom (0xFF,0xFF);
   diff=0.0;

  //-----------------------------------GUI calibration--------------------------------------------------------------------------------
   if(calibration_done_flag == '1')
   {
      gui_temp_char = read_single_byte_from_eeprom(ENG_FUEL_LEVEL_DIFFERENCE_SIGN_VAL_HIGH_ADD ,ENG_FUEL_LEVEL_DIFFERENCE_SIGN_VAL_LOW_ADD );

      diff = get_GUI_calibrated_val_from_eeprom(ENG_FUEL_LEVEL_DIFFERENCE_VAL_HIGH_ADD , ENG_FUEL_LEVEL_DIFFERENCE_VAL_LOW_ADD); // this is diff

      if(gui_temp_char == '1')
       temp_value = temp_value - diff;
      else
       temp_value = temp_value +  diff;
   }
  if(adc_value < 20)
      temp_value=0.0;
   
  return temp_value;
}


//------------------------------------------------ENGINE OIL PRESSURE---== ENG LLOP-------------------------------------------------------------------------
float scan_eng_fuel_pressure()
{
  float x[5]={161.3,775.6,1166.5,1470.5,1613.2}, y[5]={0.0,2.0,4.0,6.0,8.0};     // look up table for llop
  float m1,m2,m3,m4,c1,c2,c3,c4;

  m1 =  ((y[1]- y[0])/(x[1]- x[0]));
  c1=  (y[0]-(m1 *x[0]));

  m2 =  ((y[2]- y[1])/(x[2]- x[1]));
  c2=  (y[1]-(m2 *x[1]));

  m3 =  ((y[3]- y[2])/(x[3]- x[2]));
  c3=  (y[2]-(m3 *x[2]));

  m4 =  ((y[4]- y[3])/(x[4]- x[3]));
  c4=  (y[3]-(m4 *x[3]));

  temp_int_value=0;
  for(i=0;i<AVG_COUNT;i++)
  {
   adc_value = ADC_ENGINE_FUEL_PRESSURE;
   temp_int_value += adc_value;
  }
   adc_value=temp_int_value/AVG_COUNT;
   if(adc_value < x[0])
   {
     temp_value=0.0;
     flag_llop_open='0';
   }
   else if((adc_value > x[0]) && (adc_value < x[1]))
   {
    temp_value = ( ((float)adc_value *(m1)) + (c1));
    flag_llop_open='0';
   }
   else if((adc_value > x[1]) && (adc_value < x[2]))
   {
    temp_value = ( ((float)adc_value *(m2)) + (c2));
    flag_llop_open='0';
   }
   else if((adc_value > x[2]) && (adc_value < x[3]))
   {
    temp_value = ( ((float)adc_value *(m3)) + (c3));
    flag_llop_open='0';
   }
   else if((adc_value > x[3]) && (adc_value < x[4]))
   {
    temp_value = ( ((float)adc_value *(m4)) + (c4));
    flag_llop_open='0';
   }
   else if((adc_value > x[4]) && (adc_value < 2000 ))
   {
    flag_llop_open='0';
    temp_value=8.0;
   }
   else
   {
    temp_value=8.0;
    flag_llop_open='1';
   }
  return temp_value;
}

//------------------------------COMP OIL LEVEL---CHANGED TO +12V BATTERY SENSE- OR APU BAT CHARGE MIN -----------------------------------------------------------------------
float apu_battery_sense()
{
  temp_int_value=0;
  for(i=0;i<AVG_COUNT;i++)
  {
   adc_value = ADC_APU_BATTERY_SENSE;
   temp_int_value += adc_value;
  }
  adc_value=temp_int_value/AVG_COUNT;
  temp_value= ((float)adc_value * 16.0)/3248.0; //BAT VTG 0 TO 16V ADC INPUT VTG IS 0 TO 2.62 V       Written by Madhur on 12112016

   calibration_done_flag = read_single_byte_from_eeprom (0xFF,0xFF);
   diff=0.0;

  //-----------------------------------GUI calibration--------------------------------------------------------------------------------
 if(calibration_done_flag == '1')
 {
    gui_temp_char = read_single_byte_from_eeprom(APU_BAT_VTG_DIFFERENCE_SIGN_VAL_HIGH_ADD ,APU_BAT_VTG_DIFFERENCE_SIGN_VAL_LOW_ADD );

    diff = get_GUI_calibrated_val_from_eeprom(APU_BAT_VTG_DIFFERENCE_VAL_HIGH_ADD , APU_BAT_VTG_DIFFERENCE_VAL_LOW_ADD); // this is diff

    if(gui_temp_char == '1')
     temp_value = temp_value - diff;
    else
     temp_value = temp_value +  diff;
 }

  return temp_value;
}
//-----------------------------------------COMP AIR PRESSURE--------------------------------------------------------------------------------
float scan_comp_air_pressure()
{
  temp_int_value=0;
  for(i=0;i<AVG_COUNT;i++)
  {
   adc_value = ADC_COMP_AIR_PRESSURE;
   temp_int_value += adc_value;
  }
   adc_value=temp_int_value/AVG_COUNT;
   if(adc_value < 595)
    temp_value=0.0;
   else if((adc_value < 2978) && (adc_value >595))
    temp_value= ((0.006717 * (float)adc_value) - 4.003 ); //adc input 0.48 to 2.4 V compa air press range is 0 to 16 bar
   else if(adc_value > 2978)
     temp_value=16.0;

   calibration_done_flag = read_single_byte_from_eeprom (0xFF,0xFF);
   diff=0.0;

  //-----------------------------------GUI calibration--------------------------------------------------------------------------------
 if(calibration_done_flag == '1')
 {
    gui_temp_char = read_single_byte_from_eeprom(COMP_AIR_PRESS_DIFFERENCE_SIGN_VAL_HIGH_ADD ,COMP_AIR_PRESS_DIFFERENCE_SIGN_VAL_LOW_ADD );

    diff = get_GUI_calibrated_val_from_eeprom(COMP_AIR_PRESS_DIFFERENCE_VAL_HIGH_ADD , COMP_AIR_PRESS_DIFFERENCE_VAL_LOW_ADD); // this is diff

    if(gui_temp_char == '1')
     temp_value = temp_value - diff;
    else
     temp_value = temp_value +  diff;
 }
 if(adc_value < 595)
    temp_value=0.0;
  return temp_value;
}

//----------------------------------===----ENG COOLENT TEMP --------------------------------------------------------------------------------
unsigned long scan_coolent_temp()
{
  float x[5]={228.3,444.1,686.1,1142.9,2196.9}, y[5]={120.0,100.0,80.0,60.0,40.0};     // look up table for llop
  float m1,m2,m3,m4,c1,c2,c3,c4;

  m1 =  ((y[1]- y[0])/(x[1]- x[0]));
  c1=  (y[0]-(m1 *x[0]));

  m2 =  ((y[2]- y[1])/(x[2]- x[1]));
  c2=  (y[1]-(m2 *x[1]));

  m3 =  ((y[3]- y[2])/(x[3]- x[2]));
  c3=  (y[2]-(m3 *x[2]));

  m4 =  ((y[4]- y[3])/(x[4]- x[3]));
  c4=  (y[3]-(m4 *x[3]));


  temp_int_value=0;
  for(i=0;i<COOLENT_TEMP_AVG_COUNT;i++)
  {
   adc_value = ADC_COOLENT_TEMP;
   temp_int_value += adc_value;
  }
  adc_value=temp_int_value/COOLENT_TEMP_AVG_COUNT;
   if(adc_value < 228)
   {
    temp_value=120.0;
    flag_eng_temp_open='0';
   }
   else if((adc_value > 228) && (adc_value < 444))
   {
    temp_value = ( ((float)adc_value *(m1)) + (c1));
    flag_eng_temp_open='0';
   }
   else if((adc_value > 444) && (adc_value < 686))
   {
    temp_value = ( ((float)adc_value *(m2)) + (c2));
    flag_eng_temp_open='0';
   }
   else if((adc_value > 686) && (adc_value < 1142))
   {
    temp_value = ( ((float)adc_value *(m3)) + (c3));
    flag_eng_temp_open='0';
   }
   else if((adc_value > 1410) && (adc_value < 2842))
   {
    temp_value = ( ((float)adc_value *(m4)) + (c4));
    flag_eng_temp_open='0';
   }
   else if((adc_value > 1142) && (adc_value < 2196))
   {
     flag_eng_temp_open='0';
     temp_value=40.0;
   }
   else
   {
     flag_eng_temp_open='1';
   }

  return temp_value;
}

//---------------------------DC CURRENT SENSING---== OVERLOAD-----------------------------------------------------------------------
float scan_dc_current_sensing()
{
  temp_int_value=0;
  for(i=0;i<CURRENTS_AVG_COUNT;i++)
  {
   adc_value = ADC_AMBIENT_TEMP;
   temp_int_value += adc_value;
  }
  adc_value=temp_int_value/CURRENTS_AVG_COUNT;

//   temp_value = ( ((float)adc_value *120.0 )/ 2457.0);   // 1.98 V i.e. 2457 raw count is == 120 amp
   temp_value = ( ((float)adc_value *100.0 )/ 2047.5);   // 1.65 V i.e. 2047 raw count is == 100 amp
   calibration_done_flag = read_single_byte_from_eeprom (0xFF,0xFF);
   diff=0.0;

 //-----------------------------------GUI calibration--------------------------------------------------------------------------------
   if(calibration_done_flag == '1')
   {
      gui_temp_char = read_single_byte_from_eeprom(OVERLOAD_DIFFERENCE_SIGN_VAL_HIGH_ADD ,OVERLOAD_DIFFERENCE_SIGN_VAL_LOW_ADD );

      diff = get_GUI_calibrated_val_from_eeprom(OVERLOAD_DIFFERENCE_VAL_HIGH_ADD , OVERLOAD_DIFFERENCE_VAL_LOW_ADD); // this is diff

      if(gui_temp_char == '1')
       temp_value = temp_value - diff;
      else
       temp_value = temp_value +  diff;
   }
   if(adc_value < 20)
   {
       temp_value= 0.0;
   }
   temp_value = fabs(temp_value);
   return temp_value;
}
//-----------------------------------------COMP OIL TEMP-- changed to LOCO BAT CURRENT-----------------------------------------------------------------------------
float scan_loco_bat_current()
{
  temp_int_value=0;
  for(i=0;i<CURRENTS_AVG_COUNT;i++)
  {
   adc_value = ADC_COMP_OIL_TEMP;
   temp_int_value += adc_value;
  }
  adc_value=temp_int_value/CURRENTS_AVG_COUNT;

//  temp_value = ( ((float)adc_value *120.0 )/ 2457.0);   // 1.98 V i.e. 2457 raw count is == 120 amp
 temp_value = ( ((float)adc_value *100.0 )/ 2047.5);   // 1.65 V i.e. 2047 raw count is == 100 amp
   calibration_done_flag = read_single_byte_from_eeprom (0xFF,0xFF);
   diff=0.0;

 //-----------------------------------GUI calibration--------------------------------------------------------------------------------
   if(calibration_done_flag == '1')
   {
      gui_temp_char = read_single_byte_from_eeprom(LOCO_BAT_CURRENT_DIFFERENCE_SIGN_VAL_HIGH_ADD ,LOCO_BAT_CURRENT_DIFFERENCE_SIGN_VAL_LOW_ADD );

      diff = get_GUI_calibrated_val_from_eeprom(LOCO_BAT_CURRENT_DIFFERENCE_VAL_HIGH_ADD , LOCO_BAT_CURRENT_DIFFERENCE_VAL_LOW_ADD); // this is diff

      if(gui_temp_char == '1')
       temp_value = temp_value - diff;
      else
       temp_value = temp_value +  diff;
   }
    if(adc_value < 20)  // prev 100 which corresponds to 5 A
    {
       temp_value= 0.0;
    }
   temp_value = fabs(temp_value);
   return temp_value;
}
//------------------------LOCO_BATT/DC VTG SENSE-------------------------------------------------------------------------------
float scan_loco_bat_dc_vtg_sense()
{
  temp_int_value=0;
  for(i=0;i<AVG_COUNT;i++)
  {
   adc_value = ADC_LOCO_BAT_DC_VTG_SENSE;
   temp_int_value += adc_value;
  }
  adc_value=temp_int_value/AVG_COUNT;

   temp_value= ((100.0*(float)adc_value)/3909);      // formula is same as LOCO bat vtg      //100.0   3.1==3847  3.0 ==3722  3.15==3909

   if(temp_value >= 85.0)
     temp_value=temp_value +2.0;
   calibration_done_flag = read_single_byte_from_eeprom (0xFF,0xFF);
   diff=0.0;
//-----------------------------------GUI calibration--------------------------------------------------------------------------------
   if(calibration_done_flag == '1')
   {
      gui_temp_char = read_single_byte_from_eeprom(LOCO_BAT_HIGH_VTG_DIFFERENCE_SIGN_VAL_HIGH_ADD ,LOCO_BAT_HIGH_VTG_DIFFERENCE_SIGN_VAL_LOW_ADD );

      diff = get_GUI_calibrated_val_from_eeprom(LOCO_BAT_HIGH_VTG_DIFFERENCE_VAL_HIGH_ADD , LOCO_BAT_HIGH_VTG_DIFFERENCE_VAL_LOW_ADD); // this is diff

      if(gui_temp_char == '1')
       temp_value = temp_value - diff;
      else
       temp_value = temp_value +  diff;
   }
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  if(adc_value < 100)
    temp_value=0.0;
    
  return temp_value;
}
//---------------------------DC BATTERY---=== 64 to 84 V battery sense ----------------------------------------------------------------------------
float scan_dc_battery()
{

  temp_int_value=0;
  diff=0;
  for(i=0;i<APU_74_V_AVG_COUNT;i++)
  {
   adc_value = ADC_DC_BATTERY;
   temp_int_value += adc_value;
  }
  adc_value=temp_int_value/APU_74_V_AVG_COUNT;

   temp_value= ((100.0*(float)adc_value)/3811);      // formula is same as LOCO bat vtg     3711 == 2.991V  3611 ==2091 V    //100.0   3.1==3847  3.0 ==3722  3.15==3909

   if(temp_value < 77.0)
     temp_value = temp_value - 1.0;
   else if(temp_value >= 85.0)
     temp_value=temp_value +2.0;

   calibration_done_flag = read_single_byte_from_eeprom (0xFF,0xFF);
   diff=0.0;
  //-----------------------------------GUI calibration--------------------------------------------------------------------------------
   if(calibration_done_flag == '1')
   {
      gui_temp_char = read_single_byte_from_eeprom(APU_74_BAT_DIFFERENCE_SIGN_VAL_HIGH_ADD ,APU_74_BAT_DIFFERENCE_SIGN_VAL_LOW_ADD );

      diff = get_GUI_calibrated_val_from_eeprom(APU_74_BAT_DIFFERENCE_VAL_HIGH_ADD , APU_74_BAT_DIFFERENCE_VAL_LOW_ADD); // this is diff

      if(gui_temp_char == '1')
       temp_value = temp_value - diff;
      else
       temp_value = temp_value +  diff;
   }
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  if(adc_value < 100)
    temp_value=0.0;
  return temp_value;
}


//----------------------------------===----APU RECTIFIER TEMP --------------------------------------------------------------------------------
unsigned long scan_rectifier_temp()
{
  float x[5]={228.3,444.1,686.1,1142.9,2196.9}, y[5]={120.0,100.0,80.0,60.0,40.0};     // look up table for rectifier temp
  float m1,m2,m3,m4,c1,c2,c3,c4;

  m1 =  ((y[1]- y[0])/(x[1]- x[0]));
  c1=  (y[0]-(m1 *x[0]));

  m2 =  ((y[2]- y[1])/(x[2]- x[1]));
  c2=  (y[1]-(m2 *x[1]));

  m3 =  ((y[3]- y[2])/(x[3]- x[2]));
  c3=  (y[2]-(m3 *x[2]));

  m4 =  ((y[4]- y[3])/(x[4]- x[3]));
  c4=  (y[3]-(m4 *x[3]));


  temp_int_value=0;
  for(i=0;i<RECTIFIER_TEMP_AVG_COUNT;i++)
  {
   adc_value = ADC_RECTIFIER_TEMP;
   temp_int_value += adc_value;
  }
  adc_value=temp_int_value/RECTIFIER_TEMP_AVG_COUNT;
   if(adc_value < 228)
   {
    temp_value=120.0;
    flag_rectifier_temp_open ='0';
   }
   else if((adc_value > 228) && (adc_value < 444))
   {
    temp_value = ( ((float)adc_value *(m1)) + (c1));
    flag_rectifier_temp_open='0';
   }
   else if((adc_value > 444) && (adc_value < 686))
   {
    temp_value = ( ((float)adc_value *(m2)) + (c2));
    flag_rectifier_temp_open='0';
   }
   else if((adc_value > 686) && (adc_value < 1142))
   {
    temp_value = ( ((float)adc_value *(m3)) + (c3));
    flag_rectifier_temp_open='0';
   }
   else if((adc_value > 1410) && (adc_value < 2842))
   {
    temp_value = ( ((float)adc_value *(m4)) + (c4));
    flag_rectifier_temp_open='0';
   }
   else if((adc_value > 1142) && (adc_value < 2196))
   {
     flag_rectifier_temp_open='0';
     temp_value=40.0;
   }
   else
   {
     flag_rectifier_temp_open='1';
   }

  return temp_value;
}