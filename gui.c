#include "headers.h"
#include "stdint.h"
#include "eeprom.h"
#include "my_uart.h"
#include "gui.h"
//--------------------------GLOBAL VARIABLES---------------------------------------------------------------------------------------------------------------
float gui_apu_74V_bat_vtg=0, gui_apu_current=0;
float gui_bcp_press=0,gui_apu_bat_vtg=0,gui_loco_bat_vtg=0,gui_loco_bat_current=0, gui_comp_air_press=0;
float temp_diff=0,old_diff=0;
//---------------------------EXTERN  VARIABLES-------------------------------------------------------------------------------------------------------------------------

extern uint8_t  flag_operation_mode;
extern uint8_t  flag_eng_cool_temp_war,flag_eng_cool_temp_ala,flag_loco_rpm_war,flag_loco_rpm_ala;
extern uint8_t  flag_eng_fuel_war,flag_eng_fuel_ala,flag_eng_fuel_press_war,flag_eng_fuel_press_ala;

extern uint8_t  flag_rev_hand_pos,flag_charge_alt_freq,flag_apu_low_bat_war,flag_apu_high_bat_war,flag_apu_max_bat_war;
extern uint8_t  flag_charge_alt_freq_in_stop,flag_eng_fuel_press_ala_in_stop,flag_comp_air_temp_ala,flag_comp_air_temp_war;
extern uint8_t  flag_comp_oil_temp_ala,flag_comp_oil_temp_war,flag_loco_bat_low_vtg_war,flag_loco_bat_low_vtg_ala;
extern uint8_t  flag_loco_bat_high_vtg_war,flag_loco_bat_high_vtg_ala, flag_loco_bat_current_war,flag_loco_bat_current_ala;
extern uint8_t  flag_comp_air_press_war,flag_comp_air_press_ala;
extern uint8_t  flag_apu_74V_bat_vtg_war,flag_apu_74V_bat_vtg_ala,flag_apu_current_war,flag_apu_current_ala;
extern uint8_t  flag_apu_eng_on_off,flag_emegrgency_stop,flag_fail_to_stop ,flag_fail_to_start,flag_all_timings,flag_apu_eng_on_off;
//---------------------------------------flags related to ECU signals-------------------------------------------------------------------------------
extern uint8_t  flag_eng_over_speed,flag_ser_checck_lamp,flag_low_oil_press_lamp;

extern uint8_t  cool_temp_buf[4],eng_fuel_level_buf[5],eng_fuel_press_buf[5],apu_bat_vtg_buf[5];
extern uint8_t  charge_alt_freq_buf[6],crank_tries_count_buf[4],stop_tries_count_buf[4], run_hours_buf[4],run_min_buf[4];
extern uint8_t  loco_bat_vtg_buf[5],loco_bat_current_buf[5],comp_air_press_buf[5],apu_rpm_buf[4],apu_74V_bat_vtg_buf[5];
extern uint8_t  apu_power_buf[7],apu_current_buf[5];
extern uint8_t  flag_isr,calibration_done_flag;

extern float apu_74V_bat_vtg,apu_current,apu_power;
extern float apu_rpm,temp_def_flaot,eng_fuel_level, eng_fuel_press,apu_bat_vtg,charge_alt_freq,loco_bat_vtg,loco_bat_current, comp_air_press;
extern unsigned long temp_def_int,crank_tries,crank_tries_count,cool_temp;

 //---------------------------------------------------------------------------------------------------------------------------------------------------



void update_parameter_in_eeprom(unsigned char  high_byte,unsigned char low_byte, unsigned char *x)
{
    int a,c ;
    c = strlen(x);
    for(a=0;a <c;a++)
    {
        Write_single_byte_to_eeprom(high_byte + a, low_byte + a ,x[a]);
        Delay_ms(1);
    }

}

void gui_interrupt() iv IVT_INT_USART6 ics ICS_AUTO
{
  uint8_t gui_buf[3];//temp_gui_buf[10],download_buf[500],x[10], rx_char;
  volatile int b;

  USART6_CR1bits.RXNEIE = 0;       // disable uart 6  rx interrupt
  NVIC_IntDisable(IVT_INT_USART6);  // enable interrupt vector
  USART1_CR1bits.RXNEIE = 0;       // disable uart 1  rx interrupt
  NVIC_IntDisable(IVT_INT_USART1);  // enable interrupt vector

  for(b=0;b<3;b++)
   gui_buf[b]=UART6_readchar();

 if((gui_buf[0]== '*' ) && (gui_buf[2]== '&') )
  {
      switch(gui_buf[1])
      {
          case 'P':// --------------Send present readings to Master(GUI)--------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    flag_isr='P';
                    break;
          case 'L':// --------------Send warning and alarm flags present readings to Master(GUI)--------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    flag_isr='L';
                    break;
          case 'W':// --------------Send default warning  values to Master(GUI)--------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    flag_isr='W';
                    break;
          case 'A':// --------------Send default alarm values to Master(GUI)--------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    flag_isr='A';
                    break;
          case 'T':// --------------send all timing parameters default values to GUI--------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    flag_isr='T';
                    break;
          case 'E':// --------------send Engine parameters to Master (GUI)--------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    flag_isr='E';
                    break;
          case 'D':// --------------receive latest default warning & alarm values from Master(GUI)  & write them in EEPROM--------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    flag_isr='D';
                    break;
          case 'C':// -------------Send parameter present values to GUI & then receive calibrated values & store them in EEPROM---------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    flag_isr='C';
                    break;
          case 'U':// ------------- receive calibrated values & store them in EEPROM---------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    flag_isr='U';
                    break;
          case 'I':// ------------- Read Device ID from EEPROM & send to GUI---------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    flag_isr='I';
                    break;
          case 'S':// ------------- Receive Device ID from  GUI--& Update it in EEPROM-------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    flag_isr='S';
                    break;
          case 'M': //------------- Read FTP ON-OFF limit, Load-unload limit, Fuel tank capacity from EEPROM & send to GUI--------------------------------------------
                    flag_isr='M';
                    break;
          case 'N': //------------- receive FTP ON-OFF limit, Load-unload limit, Fuel tank capacity from GUI & update them in EEPROM --------------------------------------------
                    flag_isr='N';
                    break;
          default : flag_isr='0';
                    break;
      }
  }
   GPIOG_ODR.B14=0;
   USART6_CR1bits.RXNEIE = 1;       // enable uart 6  rx interrupt
   NVIC_IntEnable(IVT_INT_USART6);  // enable interrupt vector
   USART1_CR1bits.RXNEIE = 1;       // enable uart 1 rx interrupt
   NVIC_IntEnable(IVT_INT_USART1);  // enable interrupt vector
}

void check_isr_and_reply_to_master(void)
{
  uint8_t temp_gui_buf[10],download_buf[500],x[10], rx_char,temp_diff_buff[6];
  volatile int b;
  float temp_def_f ;
  unsigned  long temp_rpm,temp_def_i;

  USART6_CR1bits.RXNEIE = 0;       // disable uart 6  rx interrupt
  NVIC_IntDisable(IVT_INT_USART6);  // enable interrupt vector
  USART1_CR1bits.RXNEIE = 0;       // disable uart 1  rx interrupt
  NVIC_IntDisable(IVT_INT_USART1);  // enable interrupt vector

      switch(flag_isr)
      {

          case 'P':// --------------Send present readings to Master(GUI)--------------------------------------------------------------------------------------------------------------------------------------------------------------------
                   GPIOG_ODR.B14=1;
                   UART6_Sendchar('$');

                   sprintf(apu_bat_vtg_buf,"%.2f",apu_bat_vtg);
                   if(apu_bat_vtg_buf[1]=='.')
                     sprintf(apu_bat_vtg_buf,"0%.2f",apu_bat_vtg);
                   UART6_PrintString(apu_bat_vtg_buf);
                   UART6_Sendchar('|');

                   sprintf(apu_74V_bat_vtg_buf,"%.1f",apu_74V_bat_vtg);
                   if(apu_74V_bat_vtg_buf[2]=='.')
                     sprintf(apu_74V_bat_vtg_buf,"0%.1f",apu_74V_bat_vtg);
                   else  if(apu_74V_bat_vtg_buf[1]=='.')
                     sprintf(apu_74V_bat_vtg_buf,"00%.1f",apu_74V_bat_vtg);

                   UART6_PrintString(apu_74V_bat_vtg_buf);
                   UART6_Sendchar('|');

                    sprintf(apu_current_buf,"%.1f",apu_current);
                    if(apu_current_buf[2]=='.')
                     sprintf(apu_current_buf,"0%.1f",apu_current);
                    else  if(apu_current_buf[1]=='.')
                     sprintf(apu_current_buf,"00%.1f",apu_current);
                    UART6_PrintString(apu_current_buf);
                    UART6_Sendchar('|');

                   sprintf(apu_power_buf,"%.2f",apu_power);
                   if(apu_power_buf[1]=='.')
                    sprintf(apu_power_buf,"0%.2f",apu_power);
                    UART6_PrintString(apu_power_buf);
                   UART6_Sendchar('|');

                   sprintf(eng_fuel_press_buf,"%.2f",eng_fuel_press);    //llop
                   if(eng_fuel_press_buf[1]=='.')
                     sprintf(eng_fuel_press_buf,"0%.2f",eng_fuel_press);
                   UART6_PrintString(eng_fuel_press_buf);
                   UART6_Sendchar('|');
                   //-----add excitation vtg----This is read only para read value from EEPROM & send in present reading------------------

                    temp_def_f = get_excitation_vtg_val_from_eeprom(); // get excitation limit vtg value from eeprom
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                   sprintf(charge_alt_freq_buf,"%.2f",charge_alt_freq);
                   if(charge_alt_freq_buf[2]=='.')
                     sprintf(charge_alt_freq_buf,"0%.2f",charge_alt_freq);
                   else if(charge_alt_freq_buf[1]=='.')
                     sprintf(charge_alt_freq_buf,"00%.2f",charge_alt_freq);
                   UART6_PrintString(charge_alt_freq_buf);
                   UART6_Sendchar('|');

                   temp_rpm = apu_rpm;
                   sprintf(apu_rpm_buf,"%4lu",temp_rpm);
                   UART6_PrintString(apu_rpm_buf);
                   UART6_Sendchar('|');

                   sprintf(cool_temp_buf,"%4lu",cool_temp);
                   UART6_PrintString(cool_temp_buf);
                   UART6_Sendchar('|');

                   sprintf(eng_fuel_level_buf,"%.2f",eng_fuel_level);
                   if(eng_fuel_level_buf[1]=='.')
                     sprintf(eng_fuel_level_buf,"0%.2f",eng_fuel_level);
                   UART6_PrintString(eng_fuel_level_buf);
                   UART6_Sendchar('|');
                  //----------------------LOCO Parameters--------------------------------------------------------------------------------
                   //---add reverse handle default pos
                   UART6_Sendchar('2');   //reverse handle position  // 2 means para not for this program
                   UART6_Sendchar('|');

                 //  sprintf(loco_rpm_buf,"%5d",temp_rpm);  //  replace temp_rpm   by loco_rpm     after testing
                 //  UART6_PrintString(loco_rpm_buf);
                   UART6_PrintString("0");
                   UART6_Sendchar('|');

                 //  sprintf(loco_rpm_buf,"%5d",temp_rpm);  //---add loco speed  //  replace temp_rpm   by loco_rpm     after testing
                 //  UART6_PrintString(loco_rpm_buf);
                   UART6_PrintString("0");
                   UART6_Sendchar('|');

                  // sprintf(bcp_press_buf,"%.2f",bcp_press);
                  // if(bcp_press_buf[1]=='.')
                  //   sprintf(bcp_press_buf,"0%.2f",bcp_press);
                  // UART6_PrintString(bcp_press_buf);
                   UART6_PrintString("0");
                   UART6_Sendchar('|');

                   sprintf(loco_bat_vtg_buf,"%.1f",loco_bat_vtg);
                   if(loco_bat_vtg_buf[2]=='.')
                     sprintf(loco_bat_vtg_buf,"0%.1f",loco_bat_vtg);
                   else if(loco_bat_vtg_buf[1]=='.')
                     sprintf(loco_bat_vtg_buf,"00%.1f",loco_bat_vtg);
                   UART6_PrintString(loco_bat_vtg_buf);
                   UART6_Sendchar('|');

                   sprintf(loco_bat_current_buf,"%.1f",loco_bat_current);
                   if(loco_bat_current_buf[2]=='.')
                     sprintf(loco_bat_current_buf,"0%.1f",loco_bat_current);
                   else  if(loco_bat_current_buf[1]=='.')
                     sprintf(loco_bat_current_buf,"00%.1f",loco_bat_current);
                   UART6_PrintString(loco_bat_current_buf);
                   UART6_Sendchar('|');

                  // sprintf(loco_eng_water_temp_buf,"%4d",loco_eng_water_temp);
                 //  UART6_PrintString(loco_eng_water_temp_buf);
                   UART6_PrintString("0");
                   UART6_Sendchar('|');

                  // sprintf(loco_eng_oil_temp_buf,"%4d",loco_eng_oil_temp);
                 //  UART6_PrintString(loco_eng_oil_temp_buf);
                   UART6_PrintString("0");
                   UART6_Sendchar('|');

                   sprintf(comp_air_press_buf,"%.2f",comp_air_press);   //this is mr pressure
                   if(comp_air_press_buf[1]=='.')
                     sprintf(comp_air_press_buf,"0%.2f",comp_air_press);
                   UART6_PrintString(comp_air_press_buf);

                   UART6_Sendchar('|');
                   UART6_Sendchar('2');        //this is manual connect / disconnect
                   UART6_Sendchar('|');
                   UART6_Sendchar('2');   // status of AESS ready signal
                   UART6_Sendchar('|');
                   UART6_Sendchar('2');   // status of APU enable signal
                   UART6_Sendchar('|');
                   UART6_Sendchar('2');   // status of APU disable signal
                   
                    UART6_Sendchar('#');
                    break;
          case 'L':// --------------Send warning and alarm flags present readings to Master(GUI)--------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    GPIOG_ODR.B14=1;
                    UART6_Sendchar('$');
                    UART6_Sendchar(flag_apu_high_bat_war);
                    UART6_Sendchar(flag_apu_Low_bat_war);
                    UART6_Sendchar('|');
                    UART6_Sendchar(flag_apu_high_bat_war);
                    UART6_Sendchar(flag_apu_Low_bat_war);
                    UART6_Sendchar('|');
                    UART6_Sendchar(flag_apu_74V_bat_vtg_war);
                    UART6_Sendchar(flag_apu_74V_bat_vtg_ala);
                    UART6_Sendchar('|');
                    UART6_Sendchar(flag_apu_74V_bat_vtg_war);
                    UART6_Sendchar(flag_apu_74V_bat_vtg_ala);
                    UART6_Sendchar('|');
                    UART6_Sendchar(flag_apu_current_war);
                    UART6_Sendchar(flag_apu_current_ala);
                    UART6_Sendchar('|');
                    UART6_Sendchar(flag_eng_fuel_press_war);
                    UART6_Sendchar(flag_eng_fuel_press_ala);
                    UART6_Sendchar('|');
                    UART6_Sendchar(flag_eng_cool_temp_war);
                    UART6_Sendchar(flag_eng_cool_temp_ala);
                    UART6_Sendchar('|');
                    UART6_Sendchar(flag_eng_fuel_war);
                    UART6_Sendchar(flag_eng_fuel_ala);
                    UART6_Sendchar('|');
                    UART6_Sendchar('0');
                    UART6_Sendchar('0');
                    UART6_Sendchar('|');
                    UART6_Sendchar(flag_loco_bat_high_vtg_war);
                    UART6_Sendchar(flag_loco_bat_high_vtg_ala);
                    UART6_Sendchar('|');
                    UART6_Sendchar(flag_loco_bat_low_vtg_war);
                    UART6_Sendchar(flag_loco_bat_low_vtg_ala);
                    UART6_Sendchar('|');
                    UART6_Sendchar(flag_loco_bat_current_war);
                    UART6_Sendchar(flag_loco_bat_current_ala);
                    UART6_Sendchar('|');
                    UART6_Sendchar('0');
                    UART6_Sendchar('0');
                    UART6_Sendchar('|');
                    UART6_Sendchar('0');
                    UART6_Sendchar('0');
                    UART6_Sendchar('|');
                    UART6_Sendchar(flag_comp_air_press_war);
                    UART6_Sendchar(flag_comp_air_press_ala);
          /*        UART6_Sendchar('|');
                    UART6_Sendchar(flag_eng_over_speed);
                    UART6_Sendchar(flag_ser_checck_lamp);
                    UART6_Sendchar(flag_low_oil_press_lamp);
                    UART6_Sendchar(flag_high_cool_temp_lamp);
                    UART6_Sendchar(flag_water_in_fuel_lamp);
           */
                    UART6_Sendchar('#');
                    break;
          case 'W':// --------------Send default warning  values to Master(GUI)--------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    GPIOG_ODR.B14=1;
                    UART6_Sendchar('$');
       
                    memset(temp_gui_buf, '\0', 10);
                    temp_def_f  = get_apu_74_high_bat_war_val_from_eeprom();     // change to apu 74  batt high  vtg war after testing
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f = get_apu_74_low_bat_war_val_from_eeprom();    // change to apu 74  batt low  vtg war after testing
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f = get_apu_current_war_val_from_eeprom();    // apu current war value
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                    //there is no apu power warning
                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f = eng_fuel_press_war_val_from_eeprom();
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                    //there is no excitation vtg warning
                    //there is no alt freq  warning
                    //there is no apu rpm  warning
                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_eng_cool_temp_war_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_eng_fuel_level_war_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                  //----------------------LOCO Parameters--------------------------------------------------------------------------------
                    //there is no reverse handle warning
                    //there is no loco rpm warning
                    //there is no loco speed warning
                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f = get_bcp_press_war_val_from_eeprom();
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f =get_loco_high_bat_war_val_from_eeprom();
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f = get_loco_low_bat_war_val_from_eeprom();
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f = get_loco_bat_current_war_val_from_eeprom();
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_loco_eng_water_temp_war_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_loco_eng_oil_temp_war_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f = get_comp_air_press_war_val_from_eeprom(); // this is mr pressure
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);

                    UART6_Sendchar('#');
                    break;
          case 'A':// --------------Send default alarm values to Master(GUI)--------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    GPIOG_ODR.B14=1;
                    UART6_Sendchar('$');
                    flag_isr='1';
                    
                    memset(temp_gui_buf, '\0', 10);
                    temp_def_f = get_apu_high_bat_war_val_from_eeprom();  //this max war is nothing but an alarm 12V batt
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                    memset(temp_gui_buf, '\0', 10);
                    temp_def_f = get_apu_low_bat_war_val_from_eeprom();  //this max war is nothing but an alarm 12V batt
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f =get_apu_74_high_bat_ala_val_from_eeprom();
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f = get_apu_74_low_bat_ala_val_from_eeprom();
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f = get_apu_current_ala_val_from_eeprom();
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f = eng_fuel_press_ala_val_from_eeprom();
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f = get_excitation_vtg_val_from_eeprom(); // get excitation limit vtg value from eeprom
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                    memset(temp_gui_buf, '\0', 10);
                    temp_def_f = get_charge_alt_frequency_val_from_eeprom();
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                    memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_eng_cool_temp_ala_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_eng_fuel_level_ala_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                    // add loco rpm & loco speed


                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f = get_bcp_press_ala_val_from_eeprom();
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f =get_loco_high_bat_ala_val_from_eeprom();
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f = get_loco_low_bat_ala_val_from_eeprom();
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_f = get_loco_bat_current_ala_val_from_eeprom();
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_loco_eng_water_temp_ala_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_loco_eng_oil_temp_ala_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                    memset(temp_gui_buf, '\0', 10);
                    temp_def_f = get_comp_air_press_ala_val_from_eeprom();   //this is nthing but mr pressure
                    sprintf(temp_gui_buf,"%.2f",temp_def_f);
                    UART6_PrintString(temp_gui_buf);

                    UART6_Sendchar('#');
                    break;
          case 'T':// --------------send all timing parameters default values to GUI--------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    GPIOG_ODR.B14 = 1;
                    UART6_Sendchar('$');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_crank_counter_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_crank_rest_counter_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_preheat_counter_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_crank_tries_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_stop_counter_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_stop_tries_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_warm_up_counter_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_run_up_counter_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                    memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_maintainance_hour_count_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                   
                    UART6_Sendchar('|');
                    UART6_PrintString("0");   // this para is loco scan time which is applicable only in ALCO but as GUI is common to all this para is included
                    
                    UART6_Sendchar('#');
                    break;
          case 'E':// --------------send Engine parameters to Master (GUI)--------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    GPIOG_ODR.B14 = 1;
                    UART6_Sendchar('$');
                     //--------------- send eng status before sending check all conditions------------------------------------------
                     if((flag_apu_eng_on_off == '0') && (flag_fail_to_start == '1') )    // eng off  & it failed to start then unhealthy
                     {
                           UART6_Sendchar('1');   // 1 ==   UNHEALTHY
                     }
                     else if((flag_fail_to_stop == '1') || ( flag_emegrgency_stop == '1' ) || comp_air_press > 11.0)
                     {
                           UART6_Sendchar('1');   // 1 ==   UNHEALTHY
                     }
                     else
                     {
                             if(flag_apu_eng_on_off == '1')    // 1 means eng on
                             {
                                  if( flag_eng_fuel_press_ala== '1'|| flag_charge_alt_freq =='1'|| flag_emegrgency_stop == '1')
                                   {
                                         UART6_Sendchar('1');   // 1 ==   UNHEALTHY
                                   }
                                   else
                                   {
                                         UART6_Sendchar('0');   // 1== healthy
                                   }
                             }
                             else
                             {
                                    if(flag_eng_fuel_press_ala_in_stop=='1'|| flag_charge_alt_freq_in_stop =='1' || flag_emegrgency_stop == '1')
                                     {

                                            if((flag_all_timings == '1' || flag_all_timings == '2' || flag_all_timings == '3'|| flag_all_timings == '4' || flag_all_timings == '5') )
                                            {
                                                   UART6_Sendchar('0');   // 1== healthy
                                            }
                                            else
                                            {
                                                  UART6_Sendchar('1');   // 1 ==   UNHEALTHY
                                            }
                                     }
                                     else
                                     {
                                          UART6_Sendchar('0');   // 1== healthy
                                     }
                             }
                     }
                   //-----------------------------------------------
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_run_hours_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                     memset(temp_gui_buf, '\0', 10);
                    temp_def_i = get_run_minutes_val_from_eeprom();
                    sprintf(temp_gui_buf,"%lu",temp_def_i);
                    UART6_PrintString(temp_gui_buf);
                    UART6_Sendchar('|');

                    UART6_Sendchar(flag_operation_mode); //flag_operation_mode='1';  // APU in AUTO mode

                    UART6_Sendchar('#');
                    break;
          case 'D':// --------------receive latest default warning & alarm values from Master(GUI)  & write them in EEPROM--------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    GPIOG_ODR.B14 = 0;
                    b=0;
                    memset(download_buf, '\0', 500);
                    do
                    {
                       download_buf[b]=UART6_readchar();
                       rx_char = download_buf[b];
                       b++;
                    }while(rx_char != '@');
                //---------------apu para warnings-------------------------
                    strcpy(x, strtok(download_buf, "|"));
                    update_parameter_in_eeprom(APU_74_BAT_HIGH_VTG_WARNING_DEF_VAL_HIGH_ADD , APU_74_BAT_HIGH_VTG_WARNING_DEF_VAL_LOW_ADD,x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(APU_74_BAT_LOW_VTG_WARNING_DEF_VAL_HIGH_ADD , APU_74_BAT_LOW_VTG_WARNING_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(OVERLOAD_WARNING_DEF_VAL_HIGH_ADD,OVERLOAD_WARNING_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(ENG_OIL_PRESS_WARNING_DEF_VAL_HIGH_ADD,ENG_OIL_PRESS_WARNING_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(ENG_COOL_TEMP_WARNING_DEF_VAL_HIGH_ADD,ENG_COOL_TEMP_WARNING_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(ENG_FUEL_WARNING_DEF_VAL_HIGH_ADD,ENG_FUEL_WARNING_DEF_VAL_LOW_ADD, x);

                    //---------------loco para warnings-------------------------
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(BCP_PRESS_WARNING_DEF_VAL_HIGH_ADD,BCP_PRESS_WARNING_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(LOCO_BAT_HIGH_VTG_WARNING_DEF_VAL_HIGH_ADD,LOCO_BAT_HIGH_VTG_WARNING_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(LOCO_BAT_LOW_VTG_WARNING_DEF_VAL_HIGH_ADD,LOCO_BAT_LOW_VTG_WARNING_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(LOCO_BAT_CURRENT_WARNING_DEF_VAL_HIGH_ADD , LOCO_BAT_CURRENT_WARNING_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(LOCO_ENG_WATER_TEMP_WARNING_DEF_VAL_HIGH_ADD,LOCO_ENG_WATER_TEMP_WARNING_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(LOCO_ENG_OIL_TEMP_WARNING_DEF_VAL_HIGH_ADD,LOCO_ENG_OIL_TEMP_WARNING_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(COMP_AIR_PRESS_WARNING_DEF_VAL_HIGH_ADD,COMP_AIR_PRESS_WARNING_DEF_VAL_LOW_ADD, x);

                     //---------------apu para alarms-------------------------
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(APU_HIGH_BAT_WARNING_DEF_VAL_HIGH_ADD,APU_HIGH_BAT_WARNING_DEF_VAL_LOW_ADD, x);//this warning is an alarm
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(APU_LOW_BAT_WARNING_DEF_VAL_HIGH_ADD,APU_LOW_BAT_WARNING_DEF_VAL_LOW_ADD, x);//this warning is an alarm
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(APU_74_BAT_HIGH_VTG_ALARM_DEF_VAL_HIGH_ADD , APU_74_BAT_HIGH_VTG_ALARM_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(APU_74_BAT_LOW_VTG_ALARM_DEF_VAL_HIGH_ADD , APU_74_BAT_LOW_VTG_ALARM_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(OVERLOAD_ALARM_DEF_VAL_HIGH_ADD,OVERLOAD_ALARM_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(ENG_OIL_PRESS_ALARM_DEF_VAL_HIGH_ADD,ENG_OIL_PRESS_ALARM_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(APU_BAT_CHARGE_MIN_VTG_DEF_VAL_HIGH_ADD,APU_BAT_CHARGE_MIN_VTG_DEF_VAL_LOW_ADD, x);     //excitation vtg
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(CHARGE_ALT_FREQUENCY_DEF_VAL_HIGH_ADD,CHARGE_ALT_FREQUENCY_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(ENG_COOL_TEMP_ALARM_DEF_VAL_HIGH_ADD,ENG_COOL_TEMP_ALARM_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(ENG_FUEL_ALARM_DEF_VAL_HIGH_ADD,ENG_FUEL_ALARM_DEF_VAL_LOW_ADD, x);

                     //---------------loco para alarms-------------------------
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(BCP_PRESS_ALARM_DEF_VAL_HIGH_ADD,BCP_PRESS_ALARM_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(LOCO_BAT_HIGH_VTG_ALARM_DEF_VAL_HIGH_ADD,LOCO_BAT_HIGH_VTG_ALARM_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(LOCO_BAT_LOW_VTG_ALARM_DEF_VAL_HIGH_ADD,LOCO_BAT_LOW_VTG_ALARM_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(LOCO_BAT_CURRENT_ALARM_DEF_VAL_HIGH_ADD , LOCO_BAT_CURRENT_ALARM_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(LOCO_ENG_WATER_TEMP_ALARM_DEF_VAL_HIGH_ADD,LOCO_ENG_WATER_TEMP_ALARM_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(LOCO_ENG_OIL_TEMP_ALARM_DEF_VAL_HIGH_ADD,LOCO_ENG_OIL_TEMP_ALARM_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(COMP_AIR_PRESS_ALARM_DEF_VAL_HIGH_ADD,COMP_AIR_PRESS_ALARM_DEF_VAL_LOW_ADD, x);


                   //---------------timing para------------------------
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(CRANK_TIME_DEF_VAL_HIGH_ADD,CRANK_TIME_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(CRANK_REST_TIME_DEF_VAL_HIGH_ADD,CRANK_REST_TIME_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(PREHEAT_COUNTER_DEF_VAL_HIGH_ADD,PREHEAT_COUNTER_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(CRANK_TRIES_DEF_VAL_HIGH_ADD,CRANK_TRIES_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(STOP_TIME_DEF_VAL_HIGH_ADD,STOP_TIME_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(STOP_TRIES_DEF_VAL_HIGH_ADD,STOP_TRIES_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(WARM_UP_TIME_DEF_VAL_HIGH_ADD,WARM_UP_TIME_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(RUN_UP_TIME_DEF_VAL_HIGH_ADD,RUN_UP_TIME_DEF_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(MAINTAINANCE_HOUR_COUNT_VAL_HIGH_ADD,MAINTAINANCE_HOUR_COUNT_VAL_LOW_ADD, x);
                    strcpy(x, strtok(0, "@"));

                   GPIOG_ODR.B14 = 1;
                   UART6_PrintString(" -------------------  Thanks All values updated in eeprom successfully --------------------- ");
                   break;
          case 'C':// --------Send latest values to GUI------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                   GPIOG_ODR.B14=1;
                   UART6_Sendchar('$');

                   sprintf(apu_bat_vtg_buf,"%.2f",apu_bat_vtg);
                   if(apu_bat_vtg_buf[1]=='.')
                     sprintf(apu_bat_vtg_buf,"0%.2f",apu_bat_vtg);
                   UART6_PrintString(apu_bat_vtg_buf);
                   UART6_Sendchar('|');

                   sprintf(apu_74V_bat_vtg_buf,"%.1f",apu_74V_bat_vtg);
                   if(apu_74V_bat_vtg_buf[2]=='.')
                     sprintf(apu_74V_bat_vtg_buf,"0%.1f",apu_74V_bat_vtg);
                   else  if(apu_74V_bat_vtg_buf[1]=='.')
                     sprintf(apu_74V_bat_vtg_buf,"00%.1f",apu_74V_bat_vtg);

                   UART6_PrintString(apu_74V_bat_vtg_buf);
                   UART6_Sendchar('|');

                    sprintf(apu_current_buf,"%.1f",apu_current);
                    if(apu_current_buf[2]=='.')
                     sprintf(apu_current_buf,"0%.1f",apu_current);
                    else  if(apu_current_buf[1]=='.')
                     sprintf(apu_current_buf,"00%.1f",apu_current);
                    UART6_PrintString(apu_current_buf);
                    UART6_Sendchar('|');
                  //----------------------LOCO Parameters--------------------------------------------------------------------------------


                 //  sprintf(bcp_press_buf,"%.2f",bcp_press);
                 //  if(bcp_press_buf[1]=='.')
                 //    sprintf(bcp_press_buf,"0%.2f",bcp_press);
                   UART6_PrintString("0");
                  // UART6_PrintString(bcp_press_buf);
                   UART6_Sendchar('|');

                   sprintf(loco_bat_vtg_buf,"%.1f",loco_bat_vtg);
                   if(loco_bat_vtg_buf[2]=='.')
                     sprintf(loco_bat_vtg_buf,"0%.1f",loco_bat_vtg);
                   else if(loco_bat_vtg_buf[1]=='.')
                     sprintf(loco_bat_vtg_buf,"00%.1f",loco_bat_vtg);
                   UART6_PrintString(loco_bat_vtg_buf);
                   UART6_Sendchar('|');

                   sprintf(loco_bat_current_buf,"%.1f",loco_bat_current);
                   if(loco_bat_current_buf[2]=='.')
                     sprintf(loco_bat_current_buf,"0%.1f",loco_bat_current);
                   else  if(loco_bat_current_buf[1]=='.')
                     sprintf(loco_bat_current_buf,"00%.1f",loco_bat_current);
                   UART6_PrintString(loco_bat_current_buf);
                   UART6_Sendchar('|');

                 //  sprintf(loco_eng_water_temp_buf,"%4d",loco_eng_water_temp);
                  // UART6_PrintString(loco_eng_water_temp_buf);
                   UART6_PrintString("0");
                   UART6_Sendchar('|');

                 //  sprintf(loco_eng_oil_temp_buf,"%4d",loco_eng_oil_temp);
                 //  UART6_PrintString(loco_eng_oil_temp_buf);
                   UART6_PrintString("0");
                   UART6_Sendchar('|');

                   sprintf(comp_air_press_buf,"%.2f",comp_air_press);   //this is mr pressure
                   if(comp_air_press_buf[1]=='.')
                     sprintf(comp_air_press_buf,"0%.2f",comp_air_press);
                   UART6_PrintString(comp_air_press_buf);
                   UART6_Sendchar('|');
                   
                   sprintf(eng_fuel_level_buf,"%.2f",eng_fuel_level);
                   if(eng_fuel_level_buf[1]=='.')
                     sprintf(eng_fuel_level_buf,"0%.2f",eng_fuel_level);
                   UART6_PrintString(eng_fuel_level_buf);
                   
                    UART6_Sendchar('#');
                   break;
               case 'U':   //-------------now read calibrated values from GUI---write them in EEPROM---------------------------------------
                   temp_diff=0;
                    b=0;
                    memset(download_buf, '\0', 500);
                    do
                    {
                       download_buf[b]=UART6_readchar();
                       rx_char = download_buf[b];
                       b++;
                    }while(rx_char != '@');

                       //------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    strcpy(x, strtok(download_buf, "|"));
                    temp_diff =  atof(x);
                   if(calibration_done_flag == '0')
                   {
                     old_diff = 0;
                   }
                   else
                   {
                     old_diff =   get_GUI_calibrated_val_from_eeprom(APU_BAT_VTG_DIFFERENCE_VAL_HIGH_ADD , APU_BAT_VTG_DIFFERENCE_VAL_LOW_ADD); // this is diff
                     if (read_single_byte_from_eeprom(APU_BAT_VTG_DIFFERENCE_SIGN_VAL_HIGH_ADD ,APU_BAT_VTG_DIFFERENCE_SIGN_VAL_LOW_ADD ) == '1')
                      old_diff = old_diff *(-1.0);
                   }
                   temp_diff = temp_diff + old_diff;
                    
                    if(temp_diff < 0)
                      Write_single_byte_to_eeprom(APU_BAT_VTG_DIFFERENCE_SIGN_VAL_HIGH_ADD , APU_BAT_VTG_DIFFERENCE_SIGN_VAL_LOW_ADD ,'1'); // diff is minus
                    else
                       Write_single_byte_to_eeprom(APU_BAT_VTG_DIFFERENCE_SIGN_VAL_HIGH_ADD , APU_BAT_VTG_DIFFERENCE_SIGN_VAL_LOW_ADD  ,'0'); // diff is positive

                    temp_diff = fabs(temp_diff);
                    sprintf(temp_diff_buff,"%.2f",temp_diff);
                    if(temp_diff_buff[1]=='.')
                      sprintf(temp_diff_buff,"0%.2f",temp_diff);
                    update_parameter_in_eeprom(APU_BAT_VTG_DIFFERENCE_VAL_HIGH_ADD , APU_BAT_VTG_DIFFERENCE_VAL_LOW_ADD,temp_diff_buff);
                    //-------------------------------------------------------------------------------------------------------------
             
                   strcpy(x, strtok(0, "|"));
                   temp_diff =  atof(x);

                 if(calibration_done_flag == '0')
                 {
                   old_diff = 0;
                 }
                 else
                 {
                   old_diff =  get_GUI_calibrated_val_from_eeprom(APU_74_BAT_DIFFERENCE_VAL_HIGH_ADD , APU_74_BAT_DIFFERENCE_VAL_LOW_ADD); // this is diff
                   if (read_single_byte_from_eeprom(APU_74_BAT_DIFFERENCE_SIGN_VAL_HIGH_ADD ,APU_74_BAT_DIFFERENCE_SIGN_VAL_LOW_ADD ) == '1')
                    old_diff = old_diff *(-1.0);
                 }
                 temp_diff = temp_diff + old_diff;

                   if(temp_diff < 0)
                      Write_single_byte_to_eeprom(APU_74_BAT_DIFFERENCE_SIGN_VAL_HIGH_ADD , APU_74_BAT_DIFFERENCE_SIGN_VAL_LOW_ADD ,'1'); // diff is minus
                   else
                       Write_single_byte_to_eeprom(APU_74_BAT_DIFFERENCE_SIGN_VAL_HIGH_ADD , APU_74_BAT_DIFFERENCE_SIGN_VAL_LOW_ADD  ,'0'); // diff is positive

                  temp_diff = fabs(temp_diff);
                  sprintf(temp_diff_buff,"%.2f",temp_diff);
                  if(temp_diff_buff[1]=='.')
                    sprintf(temp_diff_buff,"0%.2f",temp_diff);
                  update_parameter_in_eeprom(APU_74_BAT_DIFFERENCE_VAL_HIGH_ADD , APU_74_BAT_DIFFERENCE_VAL_LOW_ADD,temp_diff_buff);
                    
                    //-------------------------------------------------------------------------------------------------------------

                   strcpy(x, strtok(0, "|"));
                   temp_diff =  atof(x);
                   if(calibration_done_flag == '0')
                   {
                     old_diff = 0;
                   }
                   else
                   {
                     old_diff =   get_GUI_calibrated_val_from_eeprom(OVERLOAD_DIFFERENCE_VAL_HIGH_ADD , OVERLOAD_DIFFERENCE_VAL_LOW_ADD); // this is diff
                     if (read_single_byte_from_eeprom(OVERLOAD_DIFFERENCE_SIGN_VAL_HIGH_ADD ,OVERLOAD_DIFFERENCE_SIGN_VAL_LOW_ADD ) == '1')
                      old_diff = old_diff *(-1.0);
                   }
                   temp_diff = temp_diff + old_diff;
                   
                   if(temp_diff < 0)
                      Write_single_byte_to_eeprom(OVERLOAD_DIFFERENCE_SIGN_VAL_HIGH_ADD , OVERLOAD_DIFFERENCE_SIGN_VAL_LOW_ADD ,'1'); // diff is minus
                   else
                       Write_single_byte_to_eeprom(OVERLOAD_DIFFERENCE_SIGN_VAL_HIGH_ADD , OVERLOAD_DIFFERENCE_SIGN_VAL_LOW_ADD  ,'0'); // diff is positive

                  temp_diff = fabs(temp_diff);
                  sprintf(temp_diff_buff,"%.2f",temp_diff);
                  if(temp_diff_buff[1]=='.')
                    sprintf(temp_diff_buff,"0%.2f",temp_diff);
                  update_parameter_in_eeprom(OVERLOAD_DIFFERENCE_VAL_HIGH_ADD , OVERLOAD_DIFFERENCE_VAL_LOW_ADD,temp_diff_buff);


                    //-------------------------------------------------------------------------------------------------------------

                   strcpy(x, strtok(0, "|"));
                   temp_diff =  atof(x);
                   if(calibration_done_flag == '0')
                   {
                     old_diff = 0;
                   }
                   else
                   {
                     old_diff =   get_GUI_calibrated_val_from_eeprom(BCP_PRESS_DIFFERENCE_VAL_HIGH_ADD , BCP_PRESS_DIFFERENCE_VAL_LOW_ADD); // this is diff
                     if (read_single_byte_from_eeprom(BCP_PRESS_DIFFERENCE_SIGN_VAL_HIGH_ADD ,BCP_PRESS_DIFFERENCE_SIGN_VAL_LOW_ADD ) == '1')
                      old_diff = old_diff *(-1.0);
                   }
                   temp_diff = temp_diff + old_diff;
                   
                   if(temp_diff < 0)
                      Write_single_byte_to_eeprom(BCP_PRESS_DIFFERENCE_SIGN_VAL_HIGH_ADD , BCP_PRESS_DIFFERENCE_SIGN_VAL_LOW_ADD ,'1'); // diff is minus
                   else
                       Write_single_byte_to_eeprom(BCP_PRESS_DIFFERENCE_SIGN_VAL_HIGH_ADD , BCP_PRESS_DIFFERENCE_SIGN_VAL_LOW_ADD  ,'0'); // diff is positive

                  temp_diff = fabs(temp_diff);
                  sprintf(temp_diff_buff,"%.2f",temp_diff);
                  if(temp_diff_buff[1]=='.')
                    sprintf(temp_diff_buff,"0%.2f",temp_diff);
                   update_parameter_in_eeprom(BCP_PRESS_DIFFERENCE_VAL_HIGH_ADD , BCP_PRESS_DIFFERENCE_VAL_LOW_ADD,temp_diff_buff);
                  
                  //-------------------------------------------------------------------------------------------------------------

                   strcpy(x, strtok(0, "|"));
                   temp_diff =  atof(x);
                   if(calibration_done_flag == '0')
                   {
                     old_diff = 0;
                   }
                   else
                   {
                     old_diff =   get_GUI_calibrated_val_from_eeprom(LOCO_BAT_HIGH_VTG_DIFFERENCE_VAL_HIGH_ADD , LOCO_BAT_HIGH_VTG_DIFFERENCE_VAL_LOW_ADD); // this is diff
                     if (read_single_byte_from_eeprom(LOCO_BAT_HIGH_VTG_DIFFERENCE_SIGN_VAL_HIGH_ADD , LOCO_BAT_HIGH_VTG_DIFFERENCE_SIGN_VAL_LOW_ADD ) == '1')
                      old_diff = old_diff *(-1.0);
                   }
                   temp_diff = temp_diff + old_diff;
                   
                   if(temp_diff < 0)
                      Write_single_byte_to_eeprom(LOCO_BAT_HIGH_VTG_DIFFERENCE_SIGN_VAL_HIGH_ADD , LOCO_BAT_HIGH_VTG_DIFFERENCE_SIGN_VAL_LOW_ADD ,'1'); // diff is minus
                   else
                       Write_single_byte_to_eeprom(LOCO_BAT_HIGH_VTG_DIFFERENCE_SIGN_VAL_HIGH_ADD , LOCO_BAT_HIGH_VTG_DIFFERENCE_SIGN_VAL_LOW_ADD  ,'0'); // diff is positive

                  temp_diff = fabs(temp_diff);
                  sprintf(temp_diff_buff,"%.2f",temp_diff);
                  if(temp_diff_buff[1]=='.')
                    sprintf(temp_diff_buff,"0%.2f",temp_diff);
                 update_parameter_in_eeprom(LOCO_BAT_HIGH_VTG_DIFFERENCE_VAL_HIGH_ADD , LOCO_BAT_HIGH_VTG_DIFFERENCE_VAL_LOW_ADD,temp_diff_buff);
                  
                  
               //-------------------------------------------------------------------------------------------------------------

                   strcpy(x, strtok(0, "|"));
                   temp_diff =  atof(x);
                   if(calibration_done_flag == '0')
                   {
                     old_diff = 0;
                   }
                   else
                   {
                     old_diff =   get_GUI_calibrated_val_from_eeprom(LOCO_BAT_CURRENT_DIFFERENCE_VAL_HIGH_ADD , LOCO_BAT_CURRENT_DIFFERENCE_VAL_LOW_ADD); // this is diff
                     if (read_single_byte_from_eeprom(LOCO_BAT_CURRENT_DIFFERENCE_SIGN_VAL_HIGH_ADD , LOCO_BAT_CURRENT_DIFFERENCE_SIGN_VAL_LOW_ADD ) == '1')
                      old_diff = old_diff *(-1.0);
                   }
                   temp_diff = temp_diff + old_diff;
                   if(temp_diff < 0)
                      Write_single_byte_to_eeprom(LOCO_BAT_CURRENT_DIFFERENCE_SIGN_VAL_HIGH_ADD , LOCO_BAT_CURRENT_DIFFERENCE_SIGN_VAL_LOW_ADD ,'1'); // diff is minus
                   else
                       Write_single_byte_to_eeprom(LOCO_BAT_CURRENT_DIFFERENCE_SIGN_VAL_HIGH_ADD , LOCO_BAT_CURRENT_DIFFERENCE_SIGN_VAL_LOW_ADD  ,'0'); // diff is positive

                  temp_diff = fabs(temp_diff);
                  sprintf(temp_diff_buff,"%.2f",temp_diff);
                  if(temp_diff_buff[1]=='.')
                    sprintf(temp_diff_buff,"0%.2f",temp_diff);
                   update_parameter_in_eeprom(LOCO_BAT_CURRENT_DIFFERENCE_VAL_HIGH_ADD , LOCO_BAT_CURRENT_DIFFERENCE_VAL_LOW_ADD,temp_diff_buff);
                  
                  
                   //-------------------------------------------------------------------------------------------------------------

                   strcpy(x, strtok(0, "|"));
                   temp_diff =  atof(x);
                   if(calibration_done_flag == '0')
                   {
                     old_diff = 0;
                   }
                   else
                   {
                     old_diff =   get_GUI_calibrated_val_from_eeprom(LOCO_ENG_OIL_TEMP_DIFFERENCE_VAL_HIGH_ADD , LOCO_ENG_OIL_TEMP_DIFFERENCE_VAL_LOW_ADD); // this is diff
                     if (read_single_byte_from_eeprom(LOCO_ENG_OIL_TEMP_DIFFERENCE_SIGN_VAL_HIGH_ADD , LOCO_ENG_OIL_TEMP_DIFFERENCE_SIGN_VAL_LOW_ADD) == '1')
                      old_diff = old_diff *(-1.0);
                   }
                   temp_diff = temp_diff + old_diff;
                   if(temp_diff < 0)
                      Write_single_byte_to_eeprom(LOCO_ENG_OIL_TEMP_DIFFERENCE_SIGN_VAL_HIGH_ADD , LOCO_ENG_OIL_TEMP_DIFFERENCE_SIGN_VAL_LOW_ADD ,'1'); // diff is minus
                   else
                       Write_single_byte_to_eeprom(LOCO_ENG_OIL_TEMP_DIFFERENCE_SIGN_VAL_HIGH_ADD , LOCO_ENG_OIL_TEMP_DIFFERENCE_SIGN_VAL_LOW_ADD  ,'0'); // diff is positive

                  temp_diff = fabs(temp_diff);
                  sprintf(temp_diff_buff,"%.2f",temp_diff);
                  if(temp_diff_buff[1]=='.')
                    sprintf(temp_diff_buff,"0%.2f",temp_diff);
                 update_parameter_in_eeprom(LOCO_ENG_OIL_TEMP_DIFFERENCE_VAL_HIGH_ADD , LOCO_ENG_OIL_TEMP_DIFFERENCE_VAL_LOW_ADD,temp_diff_buff);
                  
                  
                   //-------------------------------------------------------------------------------------------------------------

                   strcpy(x, strtok(0, "|"));
                   temp_diff =  atof(x);
                   if(calibration_done_flag == '0')
                   {
                     old_diff = 0;
                   }
                   else
                   {
                     old_diff =   get_GUI_calibrated_val_from_eeprom(LOCO_ENG_WATER_TEMP_DIFFERENCE_VAL_HIGH_ADD , LOCO_ENG_WATER_TEMP_DIFFERENCE_VAL_LOW_ADD); // this is diff
                     if (read_single_byte_from_eeprom(LOCO_ENG_WATER_TEMP_DIFFERENCE_SIGN_VAL_HIGH_ADD , LOCO_ENG_WATER_TEMP_DIFFERENCE_SIGN_VAL_LOW_ADD ) == '1')
                      old_diff = old_diff *(-1.0);
                   }
                   temp_diff = temp_diff + old_diff;
                   if(temp_diff < 0)
                      Write_single_byte_to_eeprom(LOCO_ENG_WATER_TEMP_DIFFERENCE_SIGN_VAL_HIGH_ADD , LOCO_ENG_WATER_TEMP_DIFFERENCE_SIGN_VAL_LOW_ADD ,'1'); // diff is minus
                   else
                       Write_single_byte_to_eeprom(LOCO_ENG_WATER_TEMP_DIFFERENCE_SIGN_VAL_HIGH_ADD , LOCO_ENG_WATER_TEMP_DIFFERENCE_SIGN_VAL_LOW_ADD  ,'0'); // diff is positive

                  temp_diff = fabs(temp_diff);
                  sprintf(temp_diff_buff,"%.2f",temp_diff);
                  if(temp_diff_buff[1]=='.')
                    sprintf(temp_diff_buff,"0%.2f",temp_diff);
                  update_parameter_in_eeprom(LOCO_ENG_WATER_TEMP_DIFFERENCE_VAL_HIGH_ADD , LOCO_ENG_WATER_TEMP_DIFFERENCE_VAL_LOW_ADD,temp_diff_buff);
                  

                   //-------------------------------------------------------------------------------------------------------------

                   strcpy(x, strtok(0, "|"));
                   temp_diff =  atof(x);
                   if(calibration_done_flag == '0')
                   {
                     old_diff = 0;
                   }
                   else
                   {
                     old_diff =   get_GUI_calibrated_val_from_eeprom(COMP_AIR_PRESS_DIFFERENCE_VAL_HIGH_ADD , COMP_AIR_PRESS_DIFFERENCE_VAL_LOW_ADD); // this is diff
                     if (read_single_byte_from_eeprom(COMP_AIR_PRESS_DIFFERENCE_SIGN_VAL_HIGH_ADD , COMP_AIR_PRESS_DIFFERENCE_SIGN_VAL_LOW_ADD  ) == '1')
                      old_diff = old_diff *(-1.0);
                   }
                   temp_diff = temp_diff + old_diff;
                   if(temp_diff < 0)
                      Write_single_byte_to_eeprom(COMP_AIR_PRESS_DIFFERENCE_SIGN_VAL_HIGH_ADD , COMP_AIR_PRESS_DIFFERENCE_SIGN_VAL_LOW_ADD ,'1'); // diff is minus
                   else
                       Write_single_byte_to_eeprom(COMP_AIR_PRESS_DIFFERENCE_SIGN_VAL_HIGH_ADD , COMP_AIR_PRESS_DIFFERENCE_SIGN_VAL_LOW_ADD  ,'0'); // diff is positive

                  temp_diff = fabs(temp_diff);
                  sprintf(temp_diff_buff,"%.2f",temp_diff);
                  if(temp_diff_buff[1]=='.')
                    sprintf(temp_diff_buff,"0%.2f",temp_diff);
                  update_parameter_in_eeprom(COMP_AIR_PRESS_DIFFERENCE_VAL_HIGH_ADD , COMP_AIR_PRESS_DIFFERENCE_VAL_LOW_ADD,temp_diff_buff);

                   //-------------------------------------------------------------------------------------------------------------

                   strcpy(x, strtok(0, "@"));
                   temp_diff =  atof(x);
                   if(calibration_done_flag == '0')
                   {
                     old_diff = 0;
                   }
                   else
                   {
                     old_diff = get_GUI_calibrated_val_from_eeprom(ENG_FUEL_LEVEL_DIFFERENCE_VAL_HIGH_ADD , ENG_FUEL_LEVEL_DIFFERENCE_VAL_LOW_ADD); // this is diff
                     if (read_single_byte_from_eeprom(ENG_FUEL_LEVEL_DIFFERENCE_SIGN_VAL_HIGH_ADD , ENG_FUEL_LEVEL_DIFFERENCE_SIGN_VAL_LOW_ADD  ) == '1')
                      old_diff = old_diff *(-1.0);
                   }
                   temp_diff = temp_diff + old_diff;
                   if(temp_diff < 0)
                      Write_single_byte_to_eeprom(ENG_FUEL_LEVEL_DIFFERENCE_SIGN_VAL_HIGH_ADD , ENG_FUEL_LEVEL_DIFFERENCE_SIGN_VAL_LOW_ADD ,'1'); // diff is minus
                   else
                       Write_single_byte_to_eeprom(ENG_FUEL_LEVEL_DIFFERENCE_SIGN_VAL_HIGH_ADD , ENG_FUEL_LEVEL_DIFFERENCE_SIGN_VAL_LOW_ADD  ,'0'); // diff is positive

                  temp_diff = fabs(temp_diff);
                  sprintf(temp_diff_buff,"%.2f",temp_diff);
                  if(temp_diff_buff[1]=='.')
                    sprintf(temp_diff_buff,"0%.2f",temp_diff);
                  update_parameter_in_eeprom(ENG_FUEL_LEVEL_DIFFERENCE_VAL_HIGH_ADD , ENG_FUEL_LEVEL_DIFFERENCE_VAL_LOW_ADD,temp_diff_buff);
                  
                   Write_single_byte_to_eeprom(0xFF , 0xFF  ,'1'); // calibrartion done
                   GPIOG_ODR.B14 = 1;
                   UART6_PrintString(" -------------------  Thanks calibration done successfully --------------------- ");

                break;
          case 'I':// ------------- Read Device ID from EEPROM & send to GUI---------------------------------------------------------------------------------------------------------------------------------------------------------------------
                    GPIOG_ODR.B14 = 1;
                    for(b=0;b<10;b++)
                    {
                      UART6_Sendchar(read_single_byte_from_eeprom(DEVICE_ID_HIGH_ADD + b, DEVICE_ID_LOW_ADD+ b));
                    }
                     break;
          case 'S':// ------------- Receive Device ID from  GUI--& Update it in EEPROM-------------------------------------------------------------------------------------------------------------------------------------------------------------------
                      b=0;
                      memset(download_buf, '\0', 500);
                      do
                      {
                         download_buf[b] = UART6_readchar();
                         rx_char = download_buf[b];
                         b++;
                      }while(rx_char != '@');
                      download_buf[10]='\0';
                     update_parameter_in_eeprom(DEVICE_ID_HIGH_ADD , DEVICE_ID_LOW_ADD, download_buf);
                 
                     break;
          case 'M': //------------- Read FTP ON-OFF limit, Load-unload limit, Fuel tank capacity from EEPROM & send to GUI--------------------------------------------
                     GPIOG_ODR.B14 = 1;
                     UART6_Sendchar('$');
                     temp_def_f = get_GUI_calibrated_val_from_eeprom(FTP_ON_VALUE_HIGH_ADD , FTP_ON_VALUE_LOW_ADD);
                     memset(temp_gui_buf, '\0', 10);
                     sprintf(temp_gui_buf,"%.2f",temp_def_f);
                     UART6_PrintString(temp_gui_buf);
                     UART6_Sendchar('|');

                     temp_def_f = get_GUI_calibrated_val_from_eeprom(FTP_OFF_VALUE_HIGH_ADD , FTP_OFF_VALUE_LOW_ADD);
                     memset(temp_gui_buf, '\0', 10);
                     sprintf(temp_gui_buf,"%.2f",temp_def_f);
                     UART6_PrintString(temp_gui_buf);
                     UART6_Sendchar('|');
                     
                     temp_def_f = get_GUI_calibrated_val_from_eeprom(COMP_LOADING_VALUE_HIGH_ADD , COMP_LOADING_VALUE_LOW_ADD);
                     memset(temp_gui_buf, '\0', 10);
                     sprintf(temp_gui_buf,"%.2f",temp_def_f);
                     UART6_PrintString(temp_gui_buf);
                     UART6_Sendchar('|');

                     temp_def_f = get_GUI_calibrated_val_from_eeprom(COMP_UNLOADING_VALUE_HIGH_ADD , COMP_UNLOADING_VALUE_LOW_ADD);
                     memset(temp_gui_buf, '\0', 10);
                     sprintf(temp_gui_buf,"%.2f",temp_def_f);
                     UART6_PrintString(temp_gui_buf);
                     UART6_Sendchar('|');
             
                     temp_def_f = get_GUI_calibrated_val_from_eeprom(ENG_FUEL_TANK_DEF_VAL_HIGH_ADD , ENG_FUEL_TANK_DEF_VAL_LOW_ADD);
                     memset(temp_gui_buf, '\0', 10);
                     sprintf(temp_gui_buf,"%.2f",temp_def_f);
                     UART6_PrintString(temp_gui_buf);
                     UART6_Sendchar('#');
                     
                     break;
                     
          case 'N': //------------- receive FTP ON-OFF limit, Load-unload limit, Fuel tank capacity from GUI & update them in EEPROM --------------------------------------------
                      b=0;
                      memset(download_buf, '\0', 500);
                      do
                      {
                         download_buf[b] = UART6_readchar();
                         rx_char = download_buf[b];
                         b++;
                      }while(rx_char != '@');

                    strcpy(x, strtok(download_buf, "|"));
                    update_parameter_in_eeprom(FTP_ON_VALUE_HIGH_ADD , FTP_ON_VALUE_LOW_ADD,x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(FTP_OFF_VALUE_HIGH_ADD , FTP_OFF_VALUE_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(COMP_LOADING_VALUE_HIGH_ADD , COMP_LOADING_VALUE_LOW_ADD, x);
                    strcpy(x, strtok(0, "|"));
                    update_parameter_in_eeprom(COMP_UNLOADING_VALUE_HIGH_ADD , COMP_UNLOADING_VALUE_LOW_ADD, x);
                    strcpy(x, strtok(0, "@"));
                    update_parameter_in_eeprom(ENG_FUEL_TANK_DEF_VAL_HIGH_ADD , ENG_FUEL_TANK_DEF_VAL_LOW_ADD, x);
                    
                   GPIOG_ODR.B14 = 1;
                   UART6_PrintString(" -------------------  Thanks settings updated successfully --------------------- ");
                    break;
      }
      
   GPIOG_ODR.B14=0;
   USART6_CR1bits.RXNEIE = 1;       // enable uart 6  rx interrupt
   NVIC_IntEnable(IVT_INT_USART6);  // enable interrupt vector
   USART1_CR1bits.RXNEIE = 1;       // enable uart 1 rx interrupt
   NVIC_IntEnable(IVT_INT_USART1);  // enable interrupt vector
   flag_isr='0';
}