#include "headers.h"
#include "stdint.h"
#include "eeprom.h"
#include "my_adc.h"
#include "my_uart.h"
#include "war_ala_check.h"
#include "freq_capture.h"
#include "port_mapping.h"

//------------------GLOBAL VARIABLES---------------------------------------------------------------------------------------------------------------
static unsigned long lbvtg_count_war,lbvtg_count_ala,rectifier_temp_count_war,rectifier_temp_count_ala;
static unsigned long lbvtgh_count_war,lbvtgh_count_ala ,lov_count_war,lov_count_ala,apu_low_bat_count,apu_high_bat_count,apu_max_bat_count;
static unsigned long eng_oil_press_count_war,eng_oil_press_count_ala,eng_fuel_level_war,eng_fuel_level_ala,eng_cool_temp_count_war,eng_cool_temp_count_ala;
static unsigned long charge_alt_freq_count_ala,charge_alt_freq_stop_count_ala,eng_oil_press_stop_count_ala,comp_air_temp_count_war,comp_air_temp_count_ala;
static unsigned long comp_oil_temp_count_war,comp_oil_temp_count_ala,comp_air_press_count_war,comp_air_press_count_ala,apu_dcvtg_count_war,apu_dcvtg_count_ala;
extern  unsigned int done_flag_1;
extern unsigned long flag_key_start_eng,flag_key_stop_eng;
//---------------------------EXTERN  VARIABLES-------------------------------------------------------------------------------------------------------------------------
extern uint8_t  flag_dc_cont_on_off,flag_MCCB_on_off;
extern uint8_t  flag_eng_cool_temp_war,flag_eng_cool_temp_ala,flag_excitation_fail, flag_rect_over_temp;
extern uint8_t  flag_eng_fuel_war,flag_eng_fuel_ala,flag_eng_fuel_press_war,flag_eng_fuel_press_ala;
extern uint8_t  flag_charge_alt_freq,flag_apu_low_bat_war,flag_apu_high_bat_war,flag_apu_max_bat_war;
extern uint8_t  flag_charge_alt_freq_in_stop,flag_eng_fuel_press_ala_in_stop;
extern uint8_t  flag_loco_bat_low_vtg_war,flag_loco_bat_low_vtg_ala,flag_rectifier_temp_war,flag_rectifier_temp_ala;
extern uint8_t  flag_loco_bat_high_vtg_war,flag_loco_bat_high_vtg_ala, flag_loco_bat_current_war,flag_loco_bat_current_ala;
extern uint8_t  flag_comp_air_press_war,flag_comp_air_press_ala,flag_maintanance_hooter,flag_emegrgency_stop;
extern uint8_t  flag_apu_74V_bat_vtg_war,flag_apu_74V_bat_vtg_ala,flag_apu_current_war,flag_apu_current_ala;
//---------------------------------------flags related to ECU signals-------------------------------------------------------------------------------
extern uint8_t  flag_eng_over_speed,flag_ser_checck_lamp,flag_low_oil_press_lamp,flag_high_cool_temp_lamp,flag_water_in_fuel_lamp;

extern float apu_74V_bat_vtg,apu_current;
extern float apu_rpm,temp_def_flaot,eng_fuel_level, eng_fuel_press,apu_bat_vtg,charge_alt_freq,loco_bat_vtg,loco_bat_current, comp_air_press;
extern unsigned long temp_def_int,cool_temp,rectifier_temp;
 //---------------------------------------------------------------------------------------------------------------------------------------------------

void check_loco_bat_vtg(void)
{//-------------------------- check loco bat low vtg is less than warning limit-------------------------------------------
   loco_bat_vtg = scan_loco_bat_dc_vtg_sense();
   if(loco_bat_vtg <= 72)
    {
         flag_loco_bat_high_vtg_ala='0';
         flag_loco_bat_high_vtg_war='0';
         temp_def_flaot = get_loco_low_bat_war_val_from_eeprom();
         if(loco_bat_vtg < temp_def_flaot )
         {
            lbvtg_count_war++;
            if(lbvtg_count_war > WAR_ALARM_CHECK_SET_COUNT)
            {
               flag_loco_bat_low_vtg_war='1';
               lbvtg_count_war=0;
            }
         }
         else
         {
            flag_loco_bat_low_vtg_war='0';
            lbvtg_count_war=0;
         }
      //-------------------------- check loco bat low vtg is less than alarm limit-------------------------------------------
         temp_def_flaot=get_loco_low_bat_ala_val_from_eeprom();
         if(loco_bat_vtg < temp_def_flaot )
         {
             lbvtg_count_ala++;
             if(lbvtg_count_ala > WAR_ALARM_CHECK_SET_COUNT)
             {
                lbvtg_count_ala=0;
                flag_loco_bat_low_vtg_ala='1';
                flag_loco_bat_low_vtg_war='0';
             }
         }
         else
         {
            flag_loco_bat_low_vtg_ala='0';
            lbvtg_count_ala=0;
         }
    }
    else
    {
     //-------------------------- check loco bat high vtg is less less than warning limit-------------------------------------------
      flag_loco_bat_low_vtg_ala='0';
      flag_loco_bat_low_vtg_war='0';

       temp_def_flaot=get_loco_high_bat_war_val_from_eeprom();
      if(loco_bat_vtg > temp_def_flaot )
       {
          lbvtgh_count_war++;
          if(lbvtgh_count_war > WAR_ALARM_CHECK_SET_COUNT)
          {
             lbvtgh_count_war=0;
             flag_loco_bat_high_vtg_war='1';
          }
       }
       else
       {
          flag_loco_bat_high_vtg_war='0';
          lbvtgh_count_war =0;
       }
    //-------------------------- check loco bat high vtg is less is less than alarm limit-------------------------------------------
       temp_def_flaot=get_loco_high_bat_ala_val_from_eeprom();
      if(loco_bat_vtg > temp_def_flaot )
       {
           lbvtgh_count_ala++;
             if(lbvtgh_count_ala> WAR_ALARM_CHECK_SET_COUNT)
             {
                lbvtgh_count_ala = 0;
                flag_loco_bat_high_vtg_ala='1';
                flag_loco_bat_high_vtg_war='0';
             }
       }
       else
       {
          flag_loco_bat_high_vtg_ala='0';
          lbvtgh_count_ala=0;
       }
    }

}

//-------------------------- PARAMETERS RELATED TO APU ENGINE-----------------------------------------------------------------------------------------------------
void check_apu_low_bat_vtg()
{//-------------------------- check apu low bat vtg -warning------------------------------------------------------------------------------------------------------
     temp_def_flaot=get_apu_low_bat_war_val_from_eeprom();
     apu_bat_vtg = apu_battery_sense();       //APU  12 v battery

     if(apu_bat_vtg < temp_def_flaot )   //apu bat vtg less than warning limit
     {
        apu_low_bat_count++;
        if(apu_low_bat_count > WAR_ALARM_CHECK_SET_COUNT)
        {
           apu_low_bat_count = 0;
           flag_apu_low_bat_war='1';
        }
     }
     else
     {
        flag_apu_low_bat_war='0';
        apu_low_bat_count=0;
     }
}
void check_apu_high_bat_vtg(void)
{//-------------------------- check apu high bat vtg -warning------------------------------------------------------------------------------------------------------
   temp_def_flaot = get_apu_high_bat_war_val_from_eeprom();
   if(apu_bat_vtg > temp_def_flaot )   //apu bat vtg less than warning limit
    {
       apu_high_bat_count++;
       if(apu_high_bat_count > WAR_ALARM_CHECK_SET_COUNT)
       {
          apu_high_bat_count =0;
          flag_apu_high_bat_war='1';
       }
    }
   else
    {
       apu_high_bat_count =0;
       flag_apu_high_bat_war='0';
    }
}

void compare_eng_oil_pressure(void)
{
//-------------------------- check scanned eng oil pressure is higher than alarm limit----------------------------------------
   temp_def_flaot =eng_fuel_press_ala_val_from_eeprom();
   if(eng_fuel_press > temp_def_flaot)       // check scanned eng oil press is greater than alarm limit
   {
      flag_eng_fuel_press_ala='0';
      eng_oil_press_count_ala=0;
   }
   else
   {
      eng_oil_press_count_ala++;
      if(eng_oil_press_count_ala > WAR_ALARM_CHECK_SET_COUNT)
      {
       eng_oil_press_count_ala=0;
       flag_eng_fuel_press_ala='1';
       flag_eng_fuel_press_war='0';
      }
   }
 //-------------------------- check scanned eng oil pressure is higher than warning limit----------------------------------------
   eng_fuel_press=scan_eng_fuel_pressure();
   temp_def_flaot =eng_fuel_press_war_val_from_eeprom();
   if(eng_fuel_press > temp_def_flaot)       // check scanned eng oil press is greater than warning limit
    {
        eng_oil_press_count_war=0;
        flag_eng_fuel_press_war='0';
    }
   else
    { 
       eng_oil_press_count_war++;
       if(eng_oil_press_count_war > WAR_ALARM_CHECK_SET_COUNT)
       {
          flag_eng_fuel_press_war='1';
          eng_oil_press_count_war=0;
       }
    }
}
void check_eng_fuel_level(void)
{//-------------------------- check scanned eng fuel level is less than warning limit-------------------------------------------
     eng_fuel_level=scan_engine_fuel_level();
     temp_def_int = get_eng_fuel_level_war_val_from_eeprom();
     if(eng_fuel_level > temp_def_int )
     {
        flag_eng_fuel_war='0';
        eng_fuel_level_war =0;
     }
     else
     {
        eng_fuel_level_war++;
        if(eng_fuel_level_war > WAR_ALARM_CHECK_SET_COUNT)
        {
           eng_fuel_level_war=0;
           flag_eng_fuel_war='1';
        }
     }
    //-------------------------- check scanned eng fuel level is less than alarm limit--------------------------------------------
     temp_def_int=get_eng_fuel_level_ala_val_from_eeprom();
     if(eng_fuel_level > temp_def_int )
     {
        flag_eng_fuel_ala='0';
        eng_fuel_level_ala=0;
     }
     else
     {
        eng_fuel_level_ala++;
        if(eng_fuel_level_ala > WAR_ALARM_CHECK_SET_COUNT)
        {
           eng_fuel_level_ala =0;
           flag_eng_fuel_ala='1';
           flag_eng_fuel_war='0';
        }
     }
}
void check_eng_cool_temp(void)
{
 //-------------- check scanned eng cool temp is less than warning limit--// LOCO ENGINE WATER TEMP ==  ENG COOL TEMP--------------------------------------------------------------------------------------------
   cool_temp=scan_coolent_temp();
   temp_def_int = get_eng_cool_temp_war_val_from_eeprom();
   if(cool_temp < temp_def_int )
   {
      flag_eng_cool_temp_war='0';
      eng_cool_temp_count_war=0;
   }
   else
   {
     eng_cool_temp_count_war++;
     if(eng_cool_temp_count_war > WAR_ALARM_CHECK_SET_COUNT)
     {
        eng_cool_temp_count_war=0;
        flag_eng_cool_temp_war='1';
     }
   }
//-------------- check scanned eng cool temp is less than alarm limit--// LOCO ENGINE WATER TEMP ==  ENG COOL TEMP------
    temp_def_int = get_eng_cool_temp_ala_val_from_eeprom();
    if(cool_temp < temp_def_int )
     {
        eng_cool_temp_count_ala=0;
        flag_eng_cool_temp_ala='0';
     }
     else
     {
         eng_cool_temp_count_ala++;
         if(eng_cool_temp_count_ala > WAR_ALARM_CHECK_SET_COUNT)
         {
            eng_cool_temp_count_ala=0;
            flag_eng_cool_temp_ala='1';
            flag_eng_cool_temp_war='0';
         }
     }
}
void check_charge_alt_freq(void)
{
   temp_def_flaot = get_charge_alt_frequency_val_from_eeprom();
   charge_alt_freq = get_alt_freq_using_loco_timer_channel();
   if(charge_alt_freq <= temp_def_flaot)      // check alt frq present  within limits
   {
        charge_alt_freq_count_ala++;
        if(charge_alt_freq_count_ala > WAR_ALARM_CHECK_SET_COUNT)
        {
            flag_charge_alt_freq='1';
            charge_alt_freq_count_ala=0;
        }
   }
   else
   {
     flag_charge_alt_freq='0';
     charge_alt_freq_count_ala=0;
   }
   if(charge_alt_freq >= 600)
      charge_alt_freq = 600;

   apu_rpm = ( (1500.0 * (float)charge_alt_freq)/532.0);    //530 replaced by 490
   
   if((charge_alt_freq  >=475.0) && (charge_alt_freq  <=545.0))
    apu_rpm=1501;
    
   if(charge_alt_freq  > 545.0)
      apu_rpm=1503;
}
void check_charge_alt_freq_in_stop_sequence(void)
{
   charge_alt_freq = get_alt_freq_using_loco_timer_channel();
   temp_def_flaot = get_charge_alt_frequency_val_from_eeprom();

   if(charge_alt_freq >= temp_def_flaot)      // check alt frq present  less than default value
   {
 //     charge_alt_freq_stop_count_ala++;
 //     if(charge_alt_freq_stop_count_ala > WAR_ALARM_CHECK_SET_COUNT)
  //    {
        charge_alt_freq_stop_count_ala=0;
        flag_charge_alt_freq_in_stop='1';
 //     }
   }
   else
   {
    flag_charge_alt_freq_in_stop='0';
    charge_alt_freq_stop_count_ala =0;
   }
   if(charge_alt_freq >= 600)
      charge_alt_freq = 600;

    apu_rpm = ( (1500.0 * (float)charge_alt_freq)/532.0);       //530 replaced by 490
    
   if((charge_alt_freq  >=475.0) && (charge_alt_freq  <=545.0))
    apu_rpm=1501;
    
   if(charge_alt_freq  > 545.0)
      apu_rpm=1503;
    
}
void compare_eng_oil_pressure_in_stop_sequence(void)
{
//-------------------------- check scanned eng oil pressure is higher than alarm limit----------------------------------------
   eng_fuel_press = scan_eng_fuel_pressure();
   temp_def_flaot =eng_fuel_press_ala_val_from_eeprom();
   if(eng_fuel_press < temp_def_flaot)       // check scanned eng oil press is greater than alarm limit
    {
   //   eng_oil_press_stop_count_ala=0;
      flag_eng_fuel_press_ala_in_stop='0';
    }
   else
    {
 //     eng_oil_press_stop_count_ala++;
 //     if(eng_oil_press_stop_count_ala > WAR_ALARM_CHECK_SET_COUNT)
 //     {
   //    eng_oil_press_stop_count_ala=0;
       flag_eng_fuel_press_ala_in_stop='1';
 //     }
    }
}

void check_apu_overload(void)   //_dc_current_sensing
{//-------------------------- check apu_current is less than warning limit-------------------------------------------
   apu_current = scan_dc_current_sensing();
   temp_def_flaot=get_apu_current_war_val_from_eeprom();
  if(apu_current > temp_def_flaot )
   {
      lov_count_war++;
      if(lov_count_war > WAR_ALARM_CHECK_SET_COUNT)
      {
         lov_count_war = 0;
         flag_apu_current_war='1';
      }
   }
   else
   {
      flag_apu_current_war='0';
      lov_count_war = 0;
   }
//-------------------------- check apu_current is less than alarm limit-------------------------------------------
   temp_def_flaot = get_apu_current_ala_val_from_eeprom();
   if(apu_current > temp_def_flaot )      //30.4 previously
   {
       lov_count_ala++;
       if(lov_count_ala > WAR_ALARM_CHECK_SET_COUNT)
       {
          lov_count_ala = 0;
          flag_apu_current_ala='1';
          flag_apu_current_war='0';
       }
   }
   else
   {
      flag_apu_current_ala='0';
      lov_count_ala = 0;
   }
}
void check_loco_bat_current(void)
{
 //-------------- check scanned comp_oil_temp is less than warning limit---------------------------------------------------------------------------------------------
   loco_bat_current = scan_loco_bat_current();
   temp_def_flaot = get_loco_bat_current_war_val_from_eeprom();
   if(loco_bat_current < temp_def_flaot)
   {
      comp_oil_temp_count_war=0;
      flag_loco_bat_current_war='0';
   }
   else
   {
       comp_oil_temp_count_war++;
       if(comp_oil_temp_count_war > WAR_ALARM_CHECK_SET_COUNT)
       {
          flag_loco_bat_current_war='1';
          comp_oil_temp_count_war=0;
       }
   }
//-------------- check scanned comp_oil_temp is less than alarm limit---------------------------------------------------
   temp_def_flaot = get_loco_bat_current_ala_val_from_eeprom();
  if(loco_bat_current < temp_def_flaot )
   {
      comp_oil_temp_count_ala=0;
      flag_loco_bat_current_ala='0';
   }
   else
   {
       comp_oil_temp_count_ala++;
       if(comp_oil_temp_count_ala> WAR_ALARM_CHECK_SET_COUNT)
       {
          comp_oil_temp_count_ala=0;
          flag_loco_bat_current_ala='1';
          flag_loco_bat_current_war='0';
       }
   }
}
void check_comp_air_pressure(void)  // this is MR pressure
{
  //-------------------------- check scanned comp air pressure is less than warning limit----------------------------------------
   comp_air_press = scan_comp_air_pressure();
   temp_def_flaot =get_comp_air_press_war_val_from_eeprom();
   if(comp_air_press < temp_def_flaot)
   {
      comp_air_press_count_war=0;
      flag_comp_air_press_war='0';
   }
   else
   {
      comp_air_press_count_war++;
      if(comp_air_press_count_war > WAR_ALARM_CHECK_SET_COUNT)
      {
         flag_comp_air_press_war='1';
         comp_air_press_count_war=0;
      }
   }
//-------------------------- check scanned  comp air pressure is less  than alarm limit----------------------------------------
   temp_def_flaot =get_comp_air_press_ala_val_from_eeprom();
  if(comp_air_press < temp_def_flaot)
   {
     comp_air_press_count_ala=0;
     flag_comp_air_press_ala='0';
   }
   else
   {
      comp_air_press_count_ala++;
      if(comp_air_press_count_ala > WAR_ALARM_CHECK_SET_COUNT)
      {
         comp_air_press_count_ala=0;
         flag_comp_air_press_ala='1';
         flag_comp_air_press_war='0';
      }
   }
}
void check_dc_battery(void)
{
    apu_74V_bat_vtg = scan_dc_battery();   // 64 V to 74 V battery sense
    if(apu_74V_bat_vtg >= 72)
    {   //-------------------------- check apu bat high vtg is less less than warning limit-------------------------------------------
         temp_def_flaot = get_apu_74_high_bat_war_val_from_eeprom();
        if(apu_74V_bat_vtg >= temp_def_flaot )
         {
            apu_dcvtg_count_war++;
            if(apu_dcvtg_count_war > WAR_ALARM_CHECK_SET_COUNT)
            {
               flag_apu_74V_bat_vtg_war='1';
               apu_dcvtg_count_war=0;
            }
         }
         else
         {
            flag_apu_74V_bat_vtg_war='0';
            apu_dcvtg_count_war=0;
         }
      //-------------------------- check apu bat high vtg is less is less than alarm limit-------------------------------------------
         temp_def_flaot = get_apu_74_high_bat_ala_val_from_eeprom();
         if(apu_74V_bat_vtg >= temp_def_flaot )
         {
             apu_dcvtg_count_ala++;
             if(apu_dcvtg_count_ala > WAR_ALARM_CHECK_SET_COUNT)
             {
                apu_dcvtg_count_ala=0;
                flag_apu_74V_bat_vtg_ala='1';
                flag_apu_74V_bat_vtg_war='0';
             }
         }
         else
         {
            flag_apu_74V_bat_vtg_ala='0';
            apu_dcvtg_count_ala=0;
         }
    }
    else
    {
       //-------------------------- check apu bat low vtg is less than war limit-------------------------------------------
         temp_def_flaot = get_apu_74_low_bat_war_val_from_eeprom();
         if(apu_74V_bat_vtg <= temp_def_flaot)
         {
            apu_dcvtg_count_war++;
            if(apu_dcvtg_count_war > WAR_ALARM_CHECK_SET_COUNT)
            {
               flag_apu_74V_bat_vtg_war='1';
               apu_dcvtg_count_war=0;
            }
         }
         else
         {
            flag_apu_74V_bat_vtg_war='0';
            apu_dcvtg_count_war=0;
         }
      //-------------------------- check loco bat low vtg is less than alarm limit-------------------------------------------
         temp_def_flaot = get_apu_74_low_bat_ala_val_from_eeprom();
        if(apu_74V_bat_vtg <= temp_def_flaot)
         {
             apu_dcvtg_count_ala++;
             if(apu_dcvtg_count_ala > WAR_ALARM_CHECK_SET_COUNT)
             {
                apu_dcvtg_count_ala=0;
                flag_apu_74V_bat_vtg_ala='1';
                flag_apu_74V_bat_vtg_war='0';
             }
         }
         else
         {
            flag_apu_74V_bat_vtg_ala='0';
            apu_dcvtg_count_ala=0;
         }
    }
}
//----this function checks 5 ECU inputs & sets warning & alarm flags according to inputs -------------------------------------------------------------------------------------------------------------------------------------------------
void check_ECU_inputs(void)
{
   if(ECU_1_ENG_OVER_SPEED == 0)    // pin status 0 means alarm condition normally Pin is pulled up
     flag_eng_over_speed='1';
   else
     flag_eng_over_speed='0';

   if(ECU_2_SERVICE_CHECK_LAMP == 0)   // pin status 0 means alarm condition normally Pin is pulled up
     flag_ser_checck_lamp='1';
   else
     flag_ser_checck_lamp='0';

   if(ECU_3_LOW_OIL_PRESS_LAMP == 0)  // pin status 0 means alarm condition normally Pin is pulled up
     flag_low_oil_press_lamp='1';
   else
     flag_low_oil_press_lamp='0';

   if(ECU_4_HIGH_COOL_TEMP_LAMP == 0)  // pin status 0 means alarm condition normally Pin is pulled up
     flag_high_cool_temp_lamp='1';
   else
     flag_high_cool_temp_lamp='0';

   if(ECU_5_WATER_IN_FUEL_LAMP == 0)  // pin status 0 means alarm condition normally Pin is pulled up
     flag_water_in_fuel_lamp='1';
   else
     flag_water_in_fuel_lamp='0';

   if(flag_eng_over_speed == '1' ||  flag_ser_checck_lamp == '1' || flag_low_oil_press_lamp == '1' || flag_high_cool_temp_lamp == '1' ||flag_water_in_fuel_lamp == '1' )
     GPIOG_ODR.B3=ON;
   else
   { // check_operation_mode();// check whether connect/disconnect is in manual mode
        if(flag_maintanance_hooter == '0')
         GPIOG_ODR.B3= OFF;
    }
}

void check_excitation_fail(void)
{
  float excitation_vtg_limit;
  excitation_vtg_limit = get_excitation_vtg_val_from_eeprom(); // get excitation limit vtg value from eeprom
  check_apu_low_bat_vtg();

  if((apu_bat_vtg > excitation_vtg_limit))  // apu 12 v battery vtg should be greater than excitation limit that means battery charging properly
   flag_excitation_fail='0';
  else
   flag_excitation_fail='1';

}

void check_rectifier_over_temp_fault(void)
{
  if( AIR_FILTER_SWITCH == 0 )
    flag_rect_over_temp='1';
  else
    flag_rect_over_temp='0';
}
void check_emegrgency_stop(void)
{
   if( EMERGENCY_STOP == 0 )
   {
    flag_emegrgency_stop ='1';
    flag_key_stop_eng = 0;
    flag_key_start_eng = 0;
   }
   else
    {
      flag_emegrgency_stop ='0';
    }
}

void check_mccb_feedback_status(void)
{
 if( MCCB_FEEDBACK == 0)
  {
    flag_MCCB_on_off = '1';  // MCCB is ON
  }
  else
  {
    flag_MCCB_on_off = '0'; //// MCCB is OFF
  }
}
void check_dc_contactor_feedback_status(void)
{
  if(DC_CONTACTOR_FEEDBACK == 0)
  {
    flag_dc_cont_on_off='1'; // dc contactor on
  }
  else
  {
    flag_dc_cont_on_off='0'; // dc contactor off
  }
}
void check_rectifier_temp(void)
{
 //-------------- check scanned rectifier_temp is less than warning limit----------------------------------------------------------------------------------------------
   rectifier_temp = scan_rectifier_temp();
   
 //  temp_def_int = get_eng_cool_temp_war_val_from_eeprom();
   
   if(rectifier_temp < 90 )
   {
      flag_rectifier_temp_war='0';
      rectifier_temp_count_war=0;
   }
   else
   {
     rectifier_temp_count_war++;
     if(eng_cool_temp_count_war > WAR_ALARM_CHECK_SET_COUNT)
     {
        rectifier_temp_count_war=0;
        flag_rectifier_temp_war='1';
     }
   }
//-------------- check scanned eng rectifier_temp is less than alarm limit-------
  //  temp_def_int = get_eng_cool_temp_ala_val_from_eeprom();
    if(rectifier_temp < 105 )
     {
        rectifier_temp_count_ala=0;
        flag_rectifier_temp_ala='0';
     }
     else
     {
         rectifier_temp_count_ala++;
         if(eng_cool_temp_count_ala > WAR_ALARM_CHECK_SET_COUNT)
         {
            rectifier_temp_count_ala=0;
            flag_rectifier_temp_ala='1';
            flag_rectifier_temp_war='0';
         }
     }
}