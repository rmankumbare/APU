#include "headers.h"
#include "stdint.h"
#include "loco_apu_functions.h"
#include "war_ala_check.h"
#include "port_mapping.h"
#include "main.h"
#include "gui.h"
#include "eeprom.h"
//------------------GLOBAL VARIABLES---------------------------------------------------------------------------------------------------------------
int dc_cont_start_stop_once=0,fuel_check_count=0;
unsigned long f;
float ftp_on_value,ftp_off_value;

//---------------------------EXTERN  VARIABLES-------------------------------------------------------------------------------------------------------------------------
extern uint8_t  flag_apu_trip_fault,flag_dc_cont_on_off;
extern uint8_t  flag_eng_cool_temp_war,flag_eng_cool_temp_ala,flag_loco_rpm_war,flag_loco_rpm_ala,flag_all_timings;
extern uint8_t  flag_eng_fuel_war,flag_eng_fuel_ala,flag_eng_fuel_press_war,flag_eng_fuel_press_ala,flag_emegrgency_stop;
extern uint8_t  flag_comp_load_unload;
extern uint8_t  flag_rev_hand_pos,flag_charge_alt_freq,flag_apu_low_bat_war,flag_apu_high_bat_war,flag_apu_max_bat_war;
extern uint8_t  flag_charge_alt_freq_in_stop,flag_eng_fuel_press_ala_in_stop,flag_comp_air_temp_ala,flag_comp_air_temp_war;
extern uint8_t  flag_comp_oil_temp_ala,flag_comp_oil_temp_war,flag_loco_bat_low_vtg_war,flag_loco_bat_low_vtg_ala;
extern uint8_t  flag_loco_bat_high_vtg_war,flag_loco_bat_high_vtg_ala, flag_loco_bat_current_war,flag_loco_bat_current_ala,flag_eng_trip,flag_mr_pressure_trip;

extern uint8_t flag_llop_open,flag_eng_temp_open;
extern uint8_t rs_485_query,lag_dc_cont,flag_isr,flag_ftp_on_off;
//---------------------------------------flags related to ECU signals-------------------------------------------------------------------------------
extern uint8_t  flag_eng_over_speed,flag_ser_checck_lamp,flag_low_oil_press_lamp,flag_high_cool_temp_lamp,flag_water_in_fuel_lamp;
extern unsigned long temp_def_int,crank_tries,crank_tries_count,cool_temp;
extern unsigned long flag_key_start_eng,flag_key_stop_eng;
extern float apu_74V_bat_vtg;
extern float apu_current,apu_rpm,mr_press,temp_def_flaot,bcp_press,eng_fuel_level, eng_fuel_press,apu_bat_vtg,charge_alt_freq,loco_bat_vtg,loco_bat_current, comp_air_press,apu_rpm,apu_74V_bat_vtg,apu_power;

 extern int sibas_mode;
 //---------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------------
void check_485_query_and_send_frame(void)
{
 if(rs_485_query==1)
  {
    rs485_frame();
    rs_485_query=0;
  }
  if(flag_isr!='0')
  {
     check_isr_and_reply_to_master();
     flag_isr='0';
  }
}
unsigned long check_all_apu_eng_para_in_apu_eng_on_mode(void)
{
  int r,m;
  float comp_loading_value=0,comp_unloading_value=0;
  flag_charge_alt_freq_in_stop='0';
  flag_eng_fuel_press_ala_in_stop='0';
  fuel_check_count++;
  
  check_485_query_and_send_frame(); //check for frame query & send if query received from master
  check_eng_cool_temp();
  check_485_query_and_send_frame(); //check for frame query & send if query received from master
  check_rectifier_temp();
  check_485_query_and_send_frame(); //check for frame query & send if query received from master
  if (fuel_check_count > 5)
   {
     check_eng_fuel_level();
     fuel_check_count=0;
   }
   ftp_on_value  =   get_GUI_calibrated_val_from_eeprom(FTP_ON_VALUE_HIGH_ADD , FTP_ON_VALUE_LOW_ADD);
   ftp_off_value =   get_GUI_calibrated_val_from_eeprom(FTP_OFF_VALUE_HIGH_ADD , FTP_OFF_VALUE_LOW_ADD);
   
//  if(sibas_mode == 0) // APU is not in SIBAS mode i.e ethernet not initilised
//  {
    if(eng_fuel_level < ftp_on_value)     //5
    { 
      GPIOG_ODR.B7=ON;         //FTP ON
      flag_ftp_on_off='1';
    }
    if(eng_fuel_level > ftp_off_value)       //15
    {
       GPIOG_ODR.B7=OFF;        //FTP OFF
       flag_ftp_on_off='0';
    }
//  }

  check_485_query_and_send_frame(); //check for frame query & send if query received from master
  check_apu_low_bat_vtg();
  check_485_query_and_send_frame(); //check for frame query & send if query received from master
  check_apu_high_bat_vtg();

  check_485_query_and_send_frame(); //check for frame query & send if query received from master
  check_comp_air_pressure();
  
  comp_loading_value  = get_GUI_calibrated_val_from_eeprom(COMP_LOADING_VALUE_HIGH_ADD , COMP_LOADING_VALUE_LOW_ADD);
  comp_unloading_value = get_GUI_calibrated_val_from_eeprom(COMP_UNLOADING_VALUE_HIGH_ADD , COMP_UNLOADING_VALUE_LOW_ADD);
  
  if(sibas_mode == 0) // APU is not in SIBAS mode i.e ethernet not initilised
  {
      if(comp_air_press > comp_unloading_value)   //10
      {
         GPIOG_ODR.B8=ON;         //COMP_LOADING ON ( unloading )
         flag_comp_load_unload='0'; // comp unloading
      }
      else if(comp_air_press < comp_loading_value)    //8.0
      {
         GPIOG_ODR.B8=OFF;        //COMP_LOADING OFF
         flag_comp_load_unload='1'; // comp loading
      }
  }//------------------DC CONTACTOR ON _OFF LOGIC-------------------------------------------------------------------------------------------------------------------------------
  check_apu_overload();      check_485_query_and_send_frame(); //check for frame query & send if query received from master
  check_loco_bat_current();  check_485_query_and_send_frame(); //check for frame query & send if query received from master
  check_dc_battery();

  check_loco_bat_vtg(); check_485_query_and_send_frame(); //check for frame query & send if query received from master
  check_rectifier_over_temp_fault();
  if(apu_current > 114.0)
   {
      if(dc_cont_start_stop_once == 1)
      {
           for(r=0;r<25;r++)
           {
               check_ECU_inputs();
               check_485_query_and_send_frame(); //check for frame query & send if query received from master
               compare_eng_oil_pressure();check_485_query_and_send_frame(); //check for frame query & send if query received from master
               check_charge_alt_freq(); check_485_query_and_send_frame(); //check for frame query & send if query received from master
               check_comp_air_pressure();
               if(flag_key_stop_eng == 1 || flag_key_start_eng == 1  || flag_charge_alt_freq=='1' || (comp_air_press > 11.0) )  //|| flag_eng_fuel_press_ala=='1'
                break;
               Delay_ms(100);
           }
       }
      GPIOG_ODR.B4=OFF;       //DC contactor OFF
      dc_cont_start_stop_once = 2;
   }
   else if( apu_74V_bat_vtg > 84.0 )
   {
      if(dc_cont_start_stop_once == 1)
       {
           for(r=0;r<25;r++)
           {
               check_ECU_inputs();
               check_485_query_and_send_frame(); //check for frame query & send if query received from master
               compare_eng_oil_pressure();check_485_query_and_send_frame(); //check for frame query & send if query received from master
               check_charge_alt_freq(); check_485_query_and_send_frame(); //check for frame query & send if query received from master
               check_comp_air_pressure();
               if(flag_key_stop_eng == 1 || flag_key_start_eng == 1  || flag_charge_alt_freq=='1' || (comp_air_press > 11.0) ) //|| flag_eng_fuel_press_ala=='1'
                break;
               Delay_ms(100);
           }
       }
       dc_cont_start_stop_once = 2;
       GPIOG_ODR.B4=OFF;       //DC contactor OFF
  }
  else
  {
       if(dc_cont_start_stop_once == 2)
       {
           for(r=0;r<25;r++)
           {
             check_ECU_inputs();
             check_485_query_and_send_frame(); //check for frame query & send if query received from master
             compare_eng_oil_pressure();check_485_query_and_send_frame(); //check for frame query & send if query received from master
             check_charge_alt_freq(); check_485_query_and_send_frame(); //check for frame query & send if query received from master
             check_comp_air_pressure();
             if(flag_key_stop_eng == 1 || flag_key_start_eng == 1  || flag_charge_alt_freq=='1' || (comp_air_press > 11.0)) //|| flag_eng_fuel_press_ala=='1'
              break;
             Delay_ms(100);
           }
        }
        GPIOG_ODR.B4=ON;       //DC contactor on
        dc_cont_start_stop_once= 1;
  }
  check_ECU_inputs();  //----this function checks 5 ECU inputs & sets warning & alarm flags according to inputs ------------------------------------------------------
  check_485_query_and_send_frame(); //check for frame query & send if query received from master
  apu_power = (apu_74V_bat_vtg  * apu_current);    // APU POWER
  apu_power = apu_power / 1000.0;
  check_dc_contactor_feedback_status();
  check_mccb_feedback_status();
//-------------------------- check all eng parameters are within limits or not-----------------------bat is only war i.e war==ala in bat vtg case-----------------------
 check_charge_alt_freq();
 compare_eng_oil_pressure();
//-------------------------- check all eng parameters are within limits or not-----------------------bat is only war i.e war==ala in bat vtg case-----------------------
  flag_eng_trip='0';
  check_emegrgency_stop();
  if(flag_emegrgency_stop =='1')
   {
     flag_apu_trip_fault='4';// APU tripped due to emergency
     return 0;
   }

  for(m=0;m<5;m++)
  {
    check_charge_alt_freq();
    compare_eng_oil_pressure();
    if(flag_all_timings=='5')
     break;
  }
 /*
  if(flag_eng_fuel_press_ala=='1')
  {
      flag_eng_trip='1';
      flag_apu_trip_fault='1';// APU tripped due to LLOP
    //  return 0;
  }
  else
  { */
      if(flag_charge_alt_freq=='1')
      {
          flag_eng_trip='2';
          flag_apu_trip_fault='2';// APU tripped due to LOW RPM
          return 0;
      }
      else
      {}
 // }
  check_comp_air_pressure();
  if( (comp_air_press > 11.0))
  {
          flag_mr_pressure_trip='1';
          flag_apu_trip_fault='3';// APU tripped due to MR Pressure
          return 0;
  }
  else
  { 
      flag_mr_pressure_trip='0';
  }
   return 1; // within limit
}

unsigned long check_all_apu_eng_para_in_apu_eng_off_mode(void)
{
    compare_eng_oil_pressure_in_stop_sequence();
    check_485_query_and_send_frame(); //check for frame query & send if query received from master
    check_charge_alt_freq_in_stop_sequence(); //check_charge_alt_freq_in_stop_sequence();check_charge_alt_freq_in_stop_sequence();check_charge_alt_freq_in_stop_sequence();
    check_485_query_and_send_frame(); //check for frame query & send if query received from master
    check_eng_cool_temp();  check_485_query_and_send_frame(); //check for frame query & send if query received from master
    check_rectifier_temp();  check_485_query_and_send_frame(); //check for frame query & send if query received from master
    check_eng_fuel_level(); check_485_query_and_send_frame(); //check for frame query & send if query received from master
     ftp_on_value  =   get_GUI_calibrated_val_from_eeprom(FTP_ON_VALUE_HIGH_ADD , FTP_ON_VALUE_LOW_ADD);
     ftp_off_value =   get_GUI_calibrated_val_from_eeprom(FTP_OFF_VALUE_HIGH_ADD , FTP_OFF_VALUE_LOW_ADD);

 // if(sibas_mode == 0) // APU is not in SIBAS mode i.e ethernet not initilised
 // {
    if(eng_fuel_level < ftp_on_value)     //5
    {
      GPIOG_ODR.B7=ON;         //FTP ON
      flag_ftp_on_off='1';
    }
    if(eng_fuel_level > ftp_off_value)       //15
    {
       GPIOG_ODR.B7=OFF;        //FTP OFF
       flag_ftp_on_off='0';
    }
//  }

    check_apu_low_bat_vtg(); check_485_query_and_send_frame(); //check for frame query & send if query received from master
    check_apu_high_bat_vtg();check_485_query_and_send_frame(); //check for frame query & send if query received from master
    check_comp_air_pressure(); check_485_query_and_send_frame(); //check for frame query & send if query received from master
  //------------------DC CONTACTOR ON _OFF LOGIC-------------------------------------------------------------------------------------------------------------------------------
    check_apu_overload();  check_485_query_and_send_frame(); //check for frame query & send if query received from master
    check_dc_battery();
    check_ECU_inputs();  //----this function checks 5 ECU inputs & sets warning & alarm flags according to inputs ---------------------------------------
    check_rectifier_over_temp_fault();
    check_485_query_and_send_frame(); //check for frame query & send if query received from master

    apu_power = (apu_74V_bat_vtg  * apu_current);    // APU POWER
    apu_power = apu_power / 1000.0;
    check_dc_contactor_feedback_status();
    check_mccb_feedback_status();
  //-------------------------- check all eng parameters are within limits or not-----------------------bat is only war i.e war==ala in bat vtg case-----------------------
   if(flag_eng_fuel_ala=='1' || flag_eng_fuel_press_ala_in_stop=='1'  || flag_eng_cool_temp_ala=='1'   ||  flag_charge_alt_freq_in_stop=='1' )
     return 0;
    else
     return 1; // within limit
}
unsigned long check_loco_para_before_apu_start(void)
{
  check_loco_bat_current();  check_485_query_and_send_frame(); //check for frame query & send if query received from master
  check_loco_bat_vtg();      check_485_query_and_send_frame();  //check for frame query & send if query received from maste
  check_rectifier_over_temp_fault();
  check_ECU_inputs();  //----this function checks 5 ECU inputs & sets warning & alarm flags according to inputs ---------------------------------------
  return 1;   // all initial loco  conditions are satisfactory
}