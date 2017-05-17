#include "headers.h"
#include "stdint.h"
#include "loco_apu_functions.h"
#include "eeprom.h"
#include "my_adc.h"
#include "my_uart.h"
#include "war_ala_check.h"
#include "freq_capture.h"
#include "internal_rtc.h"
#include "port_mapping.h"
#include "time_init.h"
#include "main.h"
#include "crc.h"
#include "gui.h"

 //-------------------------------------14 DIGITAL OUTPUTS ON PORT G -----------------------------------------------------------------------------------------------------------
sbit RS485_MASTER_SLAVE   at GPIOB_ODR.B3;
sbit RS485_MASTER_SLAVE_1 at GPIOG_ODR.B14;

sbit OUTPUT_FAN_CONTACTOR at GPIOG_ODR.B0;
sbit CRANK_OUTPUT         at GPIOG_ODR.B1;
sbit BATTERY_CHARGING     at GPIOG_ODR.B2;
sbit HOOTER               at GPIOG_ODR.B3;
sbit DC_CONTACTOR         at GPIOG_ODR.B4;
sbit AUX_2_OUTPUT         at GPIOG_ODR.B5;
sbit COMMON_FAULT         at GPIOG_ODR.B6;
sbit FTP_OUTPUT           at GPIOG_ODR.B7;
sbit COMP_LOADING         at GPIOG_ODR.B8;
sbit FUEL_SOLENOID        at GPIOG_ODR.B9;
sbit PREHEAT_OUTPUT       at GPIOG_ODR.B10;
sbit APU_START            at GPIOG_ODR.B11;
sbit APU_STATUS           at GPIOG_ODR.B12;
sbit LOCO_ENG_SHUTDOWN    at GPIOG_ODR.B13;

sbit EXCITATION_PULSE_OUTPUT         at GPIOE_ODR.B13;
//---------------------------------FLAGS----------------------------------------------------------------------------------------------------------
uint8_t  flag_eng_cool_temp_war='0',flag_eng_cool_temp_ala='0',flag_dc_cont_on_off='0',flag_ini_para_check='0';
uint8_t  flag_eng_fuel_war='0',flag_eng_fuel_ala='0',flag_eng_fuel_press_war='0',flag_eng_fuel_press_ala='0',flag_MCCB_on_off='0';
uint8_t  flag_crank_disc_llop='0',flag_crank_disc_alt_frq='0',flag_ini_stop_seq='0';
uint8_t  flag_charge_alt_freq='0',flag_apu_low_bat_war='0',flag_apu_high_bat_war='0',flag_apu_max_bat_war='0';
uint8_t  flag_charge_alt_freq_in_stop='0',flag_eng_fuel_press_ala_in_stop='0';
uint8_t  flag_operation_mode='0', flag_stop_success='0',flag_stop_auto_manual='0';//flag_apu_eng_on_off='0';
uint8_t  flag_comp_air_press_war='0',flag_comp_air_press_ala='0', flag_comp_load_unload='0';
uint8_t  flag_loco_bat_low_vtg_war='0',flag_loco_bat_low_vtg_ala='0',flag_loco_bat_high_vtg_war='0',flag_loco_bat_high_vtg_ala='0';
uint8_t  flag_loco_bat_current_war='0',flag_loco_bat_current_ala='0',flag_rectifier_temp_open='0';
uint8_t  flag_apu_74V_bat_vtg_war='0',flag_apu_74V_bat_vtg_ala='0',flag_rect_over_temp='0';
uint8_t  flag_excitation_fail='0',flag_fail_to_start='0',flag_fail_to_stop='0',flag_rectifier_temp_war='0',flag_rectifier_temp_ala='0';
uint8_t  flag_emegrgency_stop ='0',flag_eng_trip='0',flag_mr_pressure_trip='0',flag_all_timings='0',flag_maintanance_hooter='0';
uint8_t  flag_llop_open='0',flag_eng_temp_open='0',flag_apu_current_open='0',flag_fuel_save_mode_exit='0';
//---------------------------------------flags related to ECU signals-------------------------------------------------------------------------------
uint8_t  flag_eng_over_speed='0',flag_ser_checck_lamp='0',flag_low_oil_press_lamp='0',flag_high_cool_temp_lamp='0',flag_water_in_fuel_lamp='0';
uint8_t  flag_apu_current_war='0',flag_apu_current_ala='0',flag_apu_connect_dis='0',flag_apu_trip_fault='0';
//-----------------------------Buffers for RS485 frame structure-----------------------------------------------------------------------------------
uint8_t  charge_alt_freq_buf[6],crank_tries_count_buf[4],stop_tries_count_buf[4],run_hours_buf[4],run_min_buf[4];
uint8_t  loco_bat_vtg_buf[5],loco_bat_current_buf[5],comp_air_press_buf[5],apu_rpm_buf[4],apu_74V_bat_vtg_buf[5];
uint8_t  cool_temp_buf[4],rectifier_temp_buf[4],eng_fuel_level_buf[5],eng_fuel_press_buf[5],apu_bat_vtg_buf[5];
uint8_t  apu_power_buf[7],apu_current_buf[5];
uint8_t  flag_isr='0',calibration_done_flag='0',flag_ftp_on_off='0',flag_loco_emergency_stop='0';

//------------------GLOBAL VARIABLES---------------------------------------------------------------------------------------------------------------
uint8_t  buf[10],rs_485_read_buf[3],rs_485_query;
int sibas_mode=0;
unsigned long temp_def_int=0,crank_tries=0,crank_tries_count=0,stop_tries=0,stop_tries_count=0,cool_temp=0,rectifier_temp=0;
unsigned long flag_run_hour_int=0,flag_run_minutes_int=0;
unsigned long flag_key_start_eng=0,flag_key_stop_eng=0;
unsigned long min_count_1=0,hour_count_1=0,maintainance_hour_count=0;
float temp_def_flaot=0,eng_fuel_level=0, eng_fuel_press=0,apu_bat_vtg=0,charge_alt_freq=0,loco_bat_vtg=0,loco_bat_current=0;
float comp_air_press=0,apu_rpm=0,apu_74V_bat_vtg=0,apu_power,apu_current=0;
//---------------------------EXTERN  VARIABLES-------------------------------------------------------------------------------------------------------------------------
extern unsigned long preheat_counter,crank_counter,crank_rest_counter,warm_up_counter,run_up_counter;
extern unsigned long stop_counter,stop_rest_counter,excitation_pulse_counter;
extern uint8_t flag_apu_eng_on_off='0';
extern int dc_cont_start_stop_once;
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void initial_port_config(void)
 {
   OUTPUT_FAN_CONTACTOR  =1;
   CRANK_OUTPUT          =1;
   BATTERY_CHARGING      =1;
   HOOTER                =1;
   DC_CONTACTOR          =1;
   AUX_2_OUTPUT          =1;
   COMMON_FAULT          =1;
   FTP_OUTPUT            =1;
   COMP_LOADING          =1;
   FUEL_SOLENOID         =1;
   PREHEAT_OUTPUT        =1;
   APU_START             =1;
   APU_STATUS            =1;
   LOCO_ENG_SHUTDOWN     =1;
   EXCITATION_PULSE_OUTPUT =0;
 }


void interrupt() iv IVT_INT_USART1 ics ICS_AUTO       // rs 485 ie UART6 ISR
{
  rs_485_read_buf[0]=UART1_readchar();
  if(rs_485_read_buf[0]== 'A')
   {
     rs_485_read_buf[0]='0';
     rs_485_query=1;
   }
  else if((rs_485_read_buf[0]== 'C'))
  {
    rs_485_read_buf[0]='0';
    flag_key_stop_eng=1;
    flag_key_start_eng = 0;
  }
  else if(rs_485_read_buf[0]== 'B')
  {
   rs_485_read_buf[0]='0';
   if((flag_fail_to_start=='0') && (flag_charge_alt_freq_in_stop=='0') && (flag_eng_fuel_press_ala_in_stop=='0')&&(flag_stop_success=='0') && (flag_emegrgency_stop =='0'))
   {
    flag_key_start_eng = 1;
    flag_key_stop_eng=0;
   }
  }
  else if(rs_485_read_buf[0]== 'D')
  {
     COMMON_FAULT=OFF;
     flag_apu_trip_fault='0';
     flag_fail_to_start='0';
     flag_fail_to_stop ='0';
     flag_stop_success='0';
     flag_key_start_eng = 0;
     flag_charge_alt_freq = '0';
     flag_eng_fuel_press_ala ='0';
     flag_mr_pressure_trip='0';
     flag_excitation_fail='0';
     flag_maintanance_hooter='0';
  }
  else if(rs_485_read_buf[0]== 'E')
  {
    sibas_mode=1;
    FTP_OUTPUT= ON;    //signal from LCU
    flag_ftp_on_off='1';
  }
  else if(rs_485_read_buf[0]== 'F')
  {
    sibas_mode=1;
    FTP_OUTPUT= OFF;   //signal from LCU
    flag_ftp_on_off='0';
  }
  else if(rs_485_read_buf[0]== 'G')
  {
      sibas_mode=1;
      if(flag_apu_eng_on_off == '1') //  '1' == means engine on
      {
        COMP_LOADING=OFF;         //COMP ( loading ) signal from LCU
        flag_comp_load_unload='1'; // comp loading
      }
  }
  else if(rs_485_read_buf[0]== 'H')
  {
    sibas_mode=1;
    if(flag_apu_eng_on_off == '1') //  '1' == means engine on
    {
      COMP_LOADING=ON;         //COMP ( unloading ) signal from LCU
      flag_comp_load_unload='0'; // comp unloading
    }
  }
  else if(rs_485_read_buf[0]== 'I')
  {
     sibas_mode=1;
     flag_key_start_eng = 0;
     flag_loco_emergency_stop ='1';   // Emergency stop signal from LCU
  }
  else if(rs_485_read_buf[0]== 'J')
  {
     sibas_mode=1;
     flag_key_start_eng = 0;
     flag_loco_emergency_stop ='0';   // Emergency stop signal from LCU removed
  }

  else if(rs_485_read_buf[0]== 'Z')
  {
     sibas_mode=1;
  }
  else
  {}
  if( (flag_emegrgency_stop =='1'))
     flag_key_start_eng = 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------
void rs485_frame(void)
{//----------------------------------float parameters-------------------------------------------------------------------------------------------------------------
 unsigned long apu_rpm_temp;
 uint8_t frame_buf[200],crc_byte=0;
 int f;
 RS485_MASTER_SLAVE=1;  // enter RS485 transmission mode

//  mr_press=0.0;
  memset(frame_buf, '\0', 200);
//  sprintf(mr_press_buf,"%.2f",mr_press);
//  if(mr_press_buf[1]=='.')
//     sprintf(mr_press_buf,"0%.2f",mr_press);
//  strncat(frame_buf,mr_press_buf,5);
    strncat(frame_buf,"00.00",5);

/* sprintf(bcp_press_buf,"%.2f",bcp_press);
 if(bcp_press_buf[1]=='.')
   sprintf(bcp_press_buf,"0%.2f",bcp_press);
 strncat(frame_buf,bcp_press_buf,5);
*/
 strncat(frame_buf,"00.00",5);

 sprintf(loco_bat_vtg_buf,"%.1f",loco_bat_vtg);
 if(loco_bat_vtg_buf[2]=='.')
   sprintf(loco_bat_vtg_buf,"0%.1f",loco_bat_vtg);
 else if(loco_bat_vtg_buf[1]=='.')
   sprintf(loco_bat_vtg_buf,"00%.1f",loco_bat_vtg);

 strncat(frame_buf,loco_bat_vtg_buf,5);
 
 sprintf(loco_bat_current_buf,"%.1f",loco_bat_current);
 if(loco_bat_current_buf[2]=='.')
   sprintf(loco_bat_current_buf,"0%.1f",loco_bat_current);
 else  if(loco_bat_current_buf[1]=='.')
   sprintf(loco_bat_current_buf,"00%.1f",loco_bat_current);
   
 strncat(frame_buf,loco_bat_current_buf,5);
 
 sprintf(eng_fuel_level_buf,"%.2f",eng_fuel_level);
 if(eng_fuel_level_buf[1]=='.')
   sprintf(eng_fuel_level_buf,"0%.2f",eng_fuel_level);

 strncat(frame_buf,eng_fuel_level_buf,5);
 
 sprintf(eng_fuel_press_buf,"%.2f",eng_fuel_press);
 if(eng_fuel_press_buf[1]=='.')
   sprintf(eng_fuel_press_buf,"0%.2f",eng_fuel_press);

 strncat(frame_buf,eng_fuel_press_buf,5);
 
 sprintf(apu_bat_vtg_buf,"%.2f",apu_bat_vtg);
 if(apu_bat_vtg_buf[1]=='.')
   sprintf(apu_bat_vtg_buf,"0%.2f",apu_bat_vtg);

 strncat(frame_buf,apu_bat_vtg_buf,5);
 
 sprintf(apu_74V_bat_vtg_buf,"%.1f",apu_74V_bat_vtg);
 if(apu_74V_bat_vtg_buf[2]=='.')
   sprintf(apu_74V_bat_vtg_buf,"0%.1f",apu_74V_bat_vtg);
 else  if(apu_74V_bat_vtg_buf[1]=='.')
   sprintf(apu_74V_bat_vtg_buf,"00%.1f",apu_74V_bat_vtg);

strncat(frame_buf,apu_74V_bat_vtg_buf,5);

  sprintf(comp_air_press_buf,"%.2f",comp_air_press);
 if(comp_air_press_buf[1]=='.')
   sprintf(comp_air_press_buf,"0%.2f",comp_air_press);

 strncat(frame_buf,comp_air_press_buf,5);
 
// sprintf(loco_rpm_buf,"%5d",loco_rpm);
// strncat(frame_buf,loco_rpm_buf,5);
 strncat(frame_buf,"00000",5);

/*
 sprintf(comp_oil_temp_buf,"%4d",comp_oil_temp);
 strncat(frame_buf,comp_oil_temp_buf,4);

 sprintf(comp_air_temp_buf,"%4d",comp_air_temp);
 strncat(frame_buf,comp_air_temp_buf,4);
*/
 strncat(frame_buf,"0000",4);
 strncat(frame_buf,"0000",4);
 
 sprintf(crank_tries_count_buf,"%4lu",crank_tries_count);

 strncat(frame_buf,crank_tries_count_buf,4);
 
 sprintf(stop_tries_count_buf,"%4lu",stop_tries_count);

 
 strncat(frame_buf,stop_tries_count_buf,4);
 
 sprintf(rectifier_temp_buf,"%4lu",rectifier_temp);
 strncat(frame_buf,rectifier_temp_buf,4);

 sprintf(cool_temp_buf,"%4lu",cool_temp);
 strncat(frame_buf,cool_temp_buf,4);
 
 apu_rpm_temp = apu_rpm;
 sprintf(apu_rpm_buf,"%5lu",apu_rpm_temp);

 strncat(frame_buf,apu_rpm_buf,5);

//--------------------FLAGS------------------------------------------------------------------------------------------------------------------------------
 f=79;
 frame_buf[f++]= '0';
 frame_buf[f++]= flag_rectifier_temp_open;
 frame_buf[f++]= flag_rectifier_temp_war;
 frame_buf[f++]= flag_rectifier_temp_ala;
 frame_buf[f++]= flag_eng_cool_temp_war;
 frame_buf[f++]= flag_eng_cool_temp_ala;
 frame_buf[f++]= '0';
 frame_buf[f++]= '0';
 frame_buf[f++]= flag_eng_fuel_war;
 frame_buf[f++]= flag_eng_fuel_ala;
 frame_buf[f++]= flag_eng_fuel_press_war;
 frame_buf[f++]= flag_eng_fuel_press_ala;
 frame_buf[f++]= flag_charge_alt_freq;
 frame_buf[f++]= flag_apu_low_bat_war;
 frame_buf[f++]= '0';
 frame_buf[f++]= flag_ini_stop_seq;
 frame_buf[f++]= '0';

 frame_buf[f++]= flag_apu_high_bat_war;
 frame_buf[f++]= flag_apu_max_bat_war;
 frame_buf[f++]= flag_charge_alt_freq_in_stop;
 frame_buf[f++]= flag_eng_fuel_press_ala_in_stop;
 frame_buf[f++]= flag_operation_mode;
 frame_buf[f++]= flag_stop_success;
 frame_buf[f++]= flag_stop_auto_manual;
 frame_buf[f++]= flag_apu_eng_on_off;
 frame_buf[f++]= '0';
 frame_buf[f++]= '0';
 frame_buf[f++]= flag_apu_current_war;
 frame_buf[f++]= flag_apu_current_ala;
 frame_buf[f++]= flag_comp_air_press_war;
 frame_buf[f++]= flag_comp_air_press_ala;
 frame_buf[f++]= flag_loco_bat_low_vtg_war;
 frame_buf[f++]= flag_loco_bat_low_vtg_ala;
 frame_buf[f++]= flag_loco_bat_high_vtg_war;
 frame_buf[f++]= flag_loco_bat_high_vtg_ala;
 frame_buf[f++]= flag_loco_bat_current_war;
 frame_buf[f++]= flag_loco_bat_current_ala;
 frame_buf[f++]= '0';
 frame_buf[f++]= '0';
 frame_buf[f++]= '0';
 frame_buf[f++]= '0';
 frame_buf[f++]= flag_apu_connect_dis;
 frame_buf[f++]= '0';
 frame_buf[f++]= '0';
 frame_buf[f++]= '0';
 frame_buf[f++]= flag_comp_load_unload;
 frame_buf[f++]= flag_maintanance_hooter;
  
 frame_buf[f++]= flag_eng_over_speed;
 frame_buf[f++]= flag_ser_checck_lamp;
 frame_buf[f++]= flag_low_oil_press_lamp;
 frame_buf[f++]= flag_high_cool_temp_lamp;
 frame_buf[f++]= flag_water_in_fuel_lamp;

 frame_buf[f++]= '0';
 frame_buf[f++]= '0';
 frame_buf[f++]= flag_apu_74V_bat_vtg_war;
 frame_buf[f++]= flag_apu_74V_bat_vtg_ala;
 frame_buf[f++]= '0';
 frame_buf[f++]= flag_loco_emergency_stop;
 frame_buf[f++]= flag_ftp_on_off;
 frame_buf[f++]= flag_fail_to_start;
 frame_buf[f++]= flag_fail_to_stop;
 frame_buf[f++]= flag_fuel_save_mode_exit;
 frame_buf[f++]= flag_MCCB_on_off;
 frame_buf[f++]= flag_eng_temp_open;

 frame_buf[f++]= flag_llop_open;
 frame_buf[f++]= flag_dc_cont_on_off;
 frame_buf[f++]= flag_emegrgency_stop;
 frame_buf[f++]= flag_mr_pressure_trip;
 frame_buf[f++]= flag_excitation_fail;
 frame_buf[f++]= flag_apu_trip_fault;
 frame_buf[f++]= flag_rect_over_temp;
 frame_buf[f++]= flag_all_timings;

 sprintf(charge_alt_freq_buf,"%.2f",charge_alt_freq);
 if(charge_alt_freq_buf[2]=='.')
   sprintf(charge_alt_freq_buf,"0%.2f",charge_alt_freq);
 else if(charge_alt_freq_buf[1]=='.')
   sprintf(charge_alt_freq_buf,"00%.2f",charge_alt_freq);

 strncat(frame_buf,charge_alt_freq_buf,6);
 
  sprintf(apu_current_buf,"%.1f",apu_current);
  if(apu_current_buf[2]=='.')
   sprintf(apu_current_buf,"0%.1f",apu_current);
  else  if(apu_current_buf[1]=='.')
   sprintf(apu_current_buf,"00%.1f",apu_current);

  strncat(frame_buf,apu_current_buf,5);

 sprintf(apu_power_buf,"%.2f",apu_power);
 if(apu_power_buf[1]=='.')
  sprintf(apu_power_buf,"0%.2f",apu_power);

 strncat(frame_buf,apu_power_buf,5);
 
// sprintf(loco_eng_water_temp_buf,"%4d",loco_eng_water_temp);
// strncat(frame_buf,loco_eng_water_temp_buf,4);
  strncat(frame_buf,"0000",4);

// sprintf(loco_eng_oil_temp_buf,"%4d",loco_eng_oil_temp);
// strncat(frame_buf,loco_eng_oil_temp_buf,4);
 strncat(frame_buf,"0000",4);

 sprintf(run_min_buf,"%4lu",min_count_1);
 strncat(frame_buf,run_min_buf,4);
  
 sprintf(run_hours_buf,"%4lu",hour_count_1);
 strncat(frame_buf,run_hours_buf,4);

  crc_byte = CRC8CCITT(frame_buf);    //  calculate crc
  UART1_Sendchar('*');
  UART1_PrintString(frame_buf);
  UART1_Sendchar(crc_byte);
  UART1_Sendchar('&');

 RS485_MASTER_SLAVE=0;  // enter RS485 reception  mode
 Delay_ms(1);
}


 void system_init(void)
 {
  port_configurations();
  initial_port_config();
  dc_cont_start_stop_once= 2; // dccont is initially off
  my_adc_init();
  UART1_Init_Advanced(115200, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART1_PB67);
  UART6_Init_Advanced(115200, _UART_8_BIT_DATA, _UART_NOPARITY, _UART_ONE_STOPBIT, &_GPIO_MODULE_USART6_PC67);
//  RS485_MASTER_SLAVE_1 = 1;
 // UART6_PrintString("  United   ");
  RS485_MASTER_SLAVE=0;
  RS485_MASTER_SLAVE_1 = 0;

  USART1_CR1bits.RXNEIE = 1;       // enable uart 1  rx interrupt
  NVIC_IntEnable(IVT_INT_USART1);  // enable interrupt vector
  USART6_CR1bits.RXNEIE = 1;       // enable uart 6  rx interrupt
  NVIC_IntEnable(IVT_INT_USART6);  // enable interrupt vector

  I2C1_Init_Advanced(400000, &_GPIO_MODULE_I2C1_PB89);

  my_rtc_init();
  check_rtc_minutes();
  calibration_done_flag = read_single_byte_from_eeprom (0xFF,0xFF);
  get_run_minutes_val_from_eeprom(); // read from eeprom
  get_run_hours_val_from_eeprom();   // read from eeprom
  maintainance_hour_count = get_maintainance_hour_count_val_from_eeprom();

  if(hour_count_1 == maintainance_hour_count)
   flag_maintanance_hooter='1';

  all_timers_init();
  DISABLE_TIMER_5;
  DISABLE_TIMER_6;
  DISABLE_TIMER_7;
  DISABLE_TIMER_8;
 }
void main(void)
{
   system_init();
   flag_dc_cont_on_off='0'; // dc contactor is initially off
   flag_apu_eng_on_off='0';  // apu eng OFF : flag used to decide apu eng on or off to display llop & frq alarms in on & off case
   check_eng_fuel_level();
   //---------------------------MAIN ALGORITHM------------------------------------------------------------------------------------------------------------------------
   while(1)
   {
      if(flag_maintanance_hooter == '1')
        HOOTER=ON;
      manual_mode();
      if(flag_ini_stop_seq == '1' )    // enter page 3 of flowchart ie stop sequence
      {
           check_485_query_and_send_frame(); //check for frame query & send if query received from master
           flag_stop_auto_manual='0'; // apu stop is through keypad i.e. manual mode now do not display alarms in this mode
           stop_apu();   // initiate stop sequence
      }
   }
}
void manual_mode(void)
{
      flag_ini_stop_seq='0';
      flag_key_stop_eng=0;
      flag_key_start_eng = 0;

      check_485_query_and_send_frame();
      check_all_apu_eng_para_in_apu_eng_off_mode();
      check_485_query_and_send_frame();
      check_emegrgency_stop();
      if(flag_emegrgency_stop =='1')
       return;

      while((flag_key_start_eng == 0) || flag_fail_to_start =='1' ||(flag_eng_fuel_press_ala_in_stop=='1') || (flag_charge_alt_freq_in_stop=='1') || (flag_emegrgency_stop =='1'))   // wait till start eng manually key pressed from keypad && alt frequency less than 75  && oil pressure < 2.1 bar
      {
        check_comp_air_pressure();
        check_emegrgency_stop();
        check_all_apu_eng_para_in_apu_eng_off_mode();
        check_emegrgency_stop();
        check_loco_para_before_apu_start();

        if( (flag_emegrgency_stop =='1')  )
         flag_key_start_eng = 0;
      }
      check_emegrgency_stop();
      if((flag_emegrgency_stop =='0') && (flag_key_start_eng == 1))
      {
         start_apu_engine();  // start APU in manual mode
      }
}
void stop_cranking_sequence_due_to_emergency_or_stop_key (void)
{
   flag_key_stop_eng=0;
   flag_key_start_eng = 0;
   DC_CONTACTOR=OFF;
   dc_cont_start_stop_once= 2; // dccont is initially off
   FUEL_SOLENOID=OFF;
   EXCITATION_PULSE_OUTPUT=0;   // remove excitation pulse
   CRANK_OUTPUT=OFF;
   COMP_LOADING=OFF;         //COMP_LOADING OFF ( loading )
   flag_comp_load_unload='1'; // comp loading
   PREHEAT_OUTPUT=OFF;   // PREHEAT RELAY ON
   DISABLE_TIMER_7;   // stop preheat timer
   DISABLE_TIMER_8;   // stop crank timer
   flag_all_timings='0';  // flag for dispaly board to indicate that preheat timer finished
}
void start_apu_engine(void)
{
  uint8_t break_flag='0', flag_start_run_up_counter='0';
  volatile int m=0;
  int  temp_tries=0;
  flag_key_stop_eng=0;
  flag_all_timings='0';  // flag for dispaly board to indicate that stop timer started
  flag_fail_to_start='0';   //  no fail to start
  FUEL_SOLENOID=ON;
  COMMON_FAULT=OFF;
  crank_tries_count=0;
  temp_tries=0;
  flag_ini_stop_seq='0';
  flag_crank_disc_llop='0';
  flag_crank_disc_alt_frq ='0';
  //---------------------CRANKING-------------------------------------------------------------------------------------------------------------------------
  preheat_counter = get_preheat_counter_val_from_eeprom();
  ENABLE_TIMER_7;    // start preheat counter
  COMP_LOADING=ON;         //COMP_LOADING ON ( unloading )
  flag_comp_load_unload='0'; // comp unloading
  PREHEAT_OUTPUT=ON;   // PREHEAT RELAY ON
//  UART6_PrintString("  preheat   ");
  while(preheat_counter > 0)
  {
     check_485_query_and_send_frame(); //check for frame query & send if query received from master
     flag_all_timings='1';  // flag for dispaly board to indicate that preheat timer started
     check_emegrgency_stop();
     if((flag_key_stop_eng == 1) || (flag_emegrgency_stop =='1')) // If stop key pressed then stop start sequence and return & get ready to start again
     {
         stop_cranking_sequence_due_to_emergency_or_stop_key();
         return;
      }
  }
   DISABLE_TIMER_7;   // stop preheat timer
   PREHEAT_OUTPUT=OFF;   // PREHEAT RELAY OFF
   flag_all_timings='2';  // flag for dispaly board to indicate that crank timer started
   crank_tries = get_crank_tries_val_from_eeprom();
    do
    {
       flag_crank_disc_llop='0'; flag_crank_disc_alt_frq='0';
       flag_excitation_fail='0';  // flag for dispaly board to indicate that excitation timer started
       EXCITATION_PULSE_OUTPUT=1;
       CRANK_OUTPUT=ON;
       crank_tries_count++;             // increment crank tries when crank rest timer elapses
       temp_tries++;
       check_all_apu_eng_para_in_apu_eng_off_mode();
       check_loco_para_before_apu_start();
       crank_counter=0;
       crank_counter = get_crank_counter_val_from_eeprom();
       ENABLE_TIMER_7;    // start crank counter
     //  UART6_PrintString("  cranking   ");
       while(crank_counter > 0)    // check llop & freq till crank  time elapses
       {
             compare_eng_oil_pressure_in_stop_sequence();
             check_charge_alt_freq_in_stop_sequence();
             check_eng_cool_temp();
             check_emegrgency_stop();
             if((flag_key_stop_eng == 1) || (flag_emegrgency_stop =='1'))  // If stop key pressed then stop start sequence and return & get ready to start again
             {
                 stop_cranking_sequence_due_to_emergency_or_stop_key();
                 return;
             }
             else
             {}
             if(flag_charge_alt_freq_in_stop == '1' )
             {
                 check_485_query_and_send_frame(); //check for frame query & send if query received from master
                 flag_crank_disc_alt_frq= '1';                //  disconnect crank
                 CRANK_OUTPUT=OFF;
             }
             else if(flag_eng_fuel_press_ala_in_stop == '1')
             {
                  check_485_query_and_send_frame(); //check for frame query & send if query received from master
                  flag_crank_disc_llop='1';
                  CRANK_OUTPUT=OFF;
             }
             else
             {
                CRANK_OUTPUT=ON;
                flag_crank_disc_llop='0';
                flag_crank_disc_alt_frq ='0';
             }
             check_emegrgency_stop();
             if((flag_key_stop_eng == 1) || (flag_emegrgency_stop =='1')) // If stop key pressed then stop start sequence and return & get ready to start again
             {
                 stop_cranking_sequence_due_to_emergency_or_stop_key();
                 return;
             }
             if(flag_crank_disc_alt_frq == '1' && flag_crank_disc_llop=='1' )      // check alt frq present  within limits
              break;
            check_485_query_and_send_frame(); //check for frame query & send if query received from master
       }
       DISABLE_TIMER_7;   // stop crank timer
      // flag_all_timings='0';  // flag for dispaly board to indicate that crank timer stopped
       CRANK_OUTPUT=OFF;
       EXCITATION_PULSE_OUTPUT=0;

       check_485_query_and_send_frame(); //check for frame query & send if query received from master
       if(flag_crank_disc_alt_frq == '1' || flag_crank_disc_llop=='1' )      // check alt frq present  within limits
       {
          break;
       }
       else
       {
           check_all_apu_eng_para_in_apu_eng_off_mode();
           check_loco_para_before_apu_start();
        //   UART6_PrintString("  crank rest   ");
           check_485_query_and_send_frame(); //check for frame query & send if query received from master
           crank_rest_counter = get_crank_rest_counter_val_from_eeprom();
           ENABLE_TIMER_7;          // start crank_rest timer
           flag_all_timings='3';  // flag for dispaly board to indicate that crank_rest timer started
           while(crank_rest_counter > 0)    // check llop & freq till crank rest time elapses
           {
                  compare_eng_oil_pressure_in_stop_sequence();
                  check_charge_alt_freq_in_stop_sequence();
                  check_eng_cool_temp();
                 if(flag_charge_alt_freq_in_stop == '1' )
                 {
                       check_485_query_and_send_frame(); //check for frame query & send if query received from master
                       flag_crank_disc_alt_frq='1';                //  disconnect crank
                       break_flag='1';
                       break;
                 }
                 else if(flag_eng_fuel_press_ala_in_stop == '1')
                 {
                       check_485_query_and_send_frame(); //check for frame query & send if query received from master
                       flag_crank_disc_llop='1';
                       break_flag='1';
                       break;
                 }
                 check_485_query_and_send_frame(); //check for frame query & send if query received from master
                 check_emegrgency_stop();
                if((flag_key_stop_eng == 1) || (flag_emegrgency_stop =='1'))  // If stop key pressed then stop  start sequence & ready to start again
                 {
                     stop_cranking_sequence_due_to_emergency_or_stop_key();
                     return;
                 }
           }
           DISABLE_TIMER_7;
         //  flag_all_timings='0';  // flag for dispaly board to indicate that crank_rest timer stopped
         //  UART6_PrintString("  crank rest end ");
       }
       if(break_flag == '1')
         break;         // break from do while loop
    }while(temp_tries < crank_tries );   // end of while i.e. cranking

    check_485_query_and_send_frame(); //check for frame query & send if query received from master
    if((flag_crank_disc_alt_frq == '0' )&&(flag_crank_disc_llop == '0'))      // when crank tries exceeds default limit
    {
        flag_all_timings='0';  // flag for dispaly board to indicate that crank_rest timer stopped
        flag_ini_stop_seq='0';
        flag_fail_to_start='1'; // fail to start
        flag_apu_eng_on_off='0';  // apu eng tried to start but failed so display alarms in eng on mode i.e flag_eng_fuel_press_ala : flag used to decide apu eng on or off to display llop & frq alarms in on & off case
        // display start fail & corresponding flag
        CRANK_OUTPUT=OFF;
        DC_CONTACTOR=OFF;
        dc_cont_start_stop_once= 2; // dccont is initially off
        FUEL_SOLENOID=OFF;
        flag_comp_load_unload='1'; // comp loading
        COMP_LOADING=OFF;        //COMP_LOADING OFF
        COMMON_FAULT=ON;
        HOOTER = ON;
        check_485_query_and_send_frame(); //check for frame query & send if query received from master
        Delay_ms(3000);
        HOOTER = OFF;
        check_all_apu_eng_para_in_apu_eng_off_mode();
        check_loco_para_before_apu_start();
        check_485_query_and_send_frame(); //check for frame query & send if query received from master
    }
    else     // when crank disconnected
    {
       CRANK_OUTPUT=OFF;
       check_485_query_and_send_frame(); //check for frame query & send if query received from master
       warm_up_counter = get_warm_up_counter_val_from_eeprom();
       ENABLE_TIMER_7;        // start warm up timer
       flag_all_timings='4';  // flag for dispaly board to indicate that warm_up_ timer enabled
       while(warm_up_counter > 0)
       {
             for(m=0;m<6;m++)
             {
                check_485_query_and_send_frame(); //check for frame query & send if query received from master
                compare_eng_oil_pressure_in_stop_sequence();
                check_charge_alt_freq_in_stop_sequence();
                check_eng_cool_temp();
             }
           //  if((flag_eng_fuel_press_ala_in_stop == '1') && (flag_charge_alt_freq_in_stop == '1'))
              if(flag_charge_alt_freq_in_stop == '1')
              {
                check_485_query_and_send_frame(); //check for frame query & send if query received from master
                flag_start_run_up_counter='1';
                flag_all_timings='5';  // flag for dispaly board to indicate that run up timer started
                flag_ini_stop_seq='0';
                 break;
              }
             check_485_query_and_send_frame(); //check for frame query & send if query received from master
       }
     DISABLE_TIMER_7;        // stop warm up timer
    // flag_all_timings='0';  // flag for dispaly board to indicate that warm_up timer stopped
       if(flag_start_run_up_counter == '1')
       {
           check_485_query_and_send_frame(); //check for frame query & send if query received from master
           run_up_counter = get_run_up_counter_val_from_eeprom();   // start run up counter once warm up finishes
           ENABLE_TIMER_7;     //start run up timer
           flag_all_timings='5';  // flag for dispaly board to indicate that run up timer started
           check_485_query_and_send_frame();
           while(run_up_counter > 0)
           {
                check_485_query_and_send_frame();
                flag_key_stop_eng == 0;
               // UART6_PrintString("  run up   ");
                check_485_query_and_send_frame(); //check for frame query & send if query received from master
                COMP_LOADING=OFF;         //COMP_LOADING OFF ( loading )
                flag_comp_load_unload='1'; // comp loading
                if(run_up_counter > 5)
                {
                    if(check_all_apu_eng_para_in_apu_eng_on_mode() == 1)       // all apu eng parameters are satisfactory
                    {
                        DISABLE_TIMER_7;     //stop run up timer
                        flag_key_start_eng=0;
                        check_485_query_and_send_frame(); //check for frame query & send if query received from master
                        flag_all_timings='0';  // flag for dispaly board to indicate that run_up timer stopped
                        flag_apu_eng_on_off='1';  // apu eng ON : flag used to decide apu eng on or off to display llop & frq alarms in on & off case
                        LOCO_ENG_SHUTDOWN=OFF;
                        check_485_query_and_send_frame(); //check for frame query & send if query received from master
                        flag_key_stop_eng=0;
                        while(1)                  // everything running properly in APU ENG
                        {
                             update_rtc_minutes();
                             check_485_query_and_send_frame();
                             check_excitation_fail(); // check  excitaion input present or not
                             flag_fail_to_start='0';   //  No fail to start
                             APU_STATUS=0;  // APU is healthy
                             check_loco_para_before_apu_start();
                             flag_apu_eng_on_off='1';  // apu eng ON : flag used to decide apu eng on or off to display llop & frq alarms in on & off case
                             check_emegrgency_stop();
                             if(flag_emegrgency_stop =='1')
                                flag_apu_trip_fault='4';// APU tripped due to emergency
                                
                             if(check_all_apu_eng_para_in_apu_eng_on_mode() == 0 || flag_emegrgency_stop =='1' || flag_key_stop_eng == 1 || flag_loco_emergency_stop == '1')  // As APU in manual mode initiate stop sequence due to alarm
                              {
                                  flag_stop_auto_manual='0'; // apu stop is due to parameter alarm in manual mode
                                  flag_ini_stop_seq='1';
                                  break;
                              }
                              else
                              {  
                                 flag_ini_stop_seq='0';
                              }
                        }
                    }
                    else
                     {
                       flag_ini_stop_seq ='1';
                     }
                }
                else
                {//run up counter  < 5 stay idle
                }
                if(flag_ini_stop_seq=='1')
                {
                  break;
                }
           }
           DISABLE_TIMER_7;     //stop run up timer
           flag_all_timings='0';  // flag for dispaly board to indicate that run_up timer stopped
       }
        else
         flag_ini_stop_seq='1';
    }
 
 }
void stop_apu(void)     //  initiate stop sequence
{
   volatile unsigned long  j=0,f,temp_stop_tries=0;
   unsigned char break_flag='0';
   COMP_LOADING=ON;         //COMP_LOADING ON ( unloading )
   flag_comp_load_unload='0'; // comp unloading
//   DC_CONTACTOR=OFF;
   dc_cont_start_stop_once= 2; // dccont is initially off
   FUEL_SOLENOID=OFF;
   DISABLE_TIMER_8;         //  stop ( stop )counter
   DISABLE_TIMER_7;     // stop run hours counter
   flag_stop_success='0';
   stop_tries_count=0;
   temp_stop_tries=0;

   write_run_minutes_in_eeprom();
   write_run_hours_in_eeprom();
   
   stop_tries = get_stop_tries_val_from_eeprom();
   flag_all_timings='6';  // flag for dispaly board to indicate that stop timer started
   do
   {
 //    DC_CONTACTOR=OFF;
     stop_counter = get_stop_counter_val_from_eeprom();
     ENABLE_TIMER_7;     // start stop counter
     while(stop_counter > 0)
     {
          check_loco_para_before_apu_start();
          check_485_query_and_send_frame(); //check for frame query & send if query received from master
          for(j=0;j<6;j++)
          {
             compare_eng_oil_pressure();
             check_charge_alt_freq();
          }
          if((flag_charge_alt_freq == '1') && (flag_eng_fuel_press_ala == '1'))      // check alt frq less than def limit or not //  check  eng oil press less than alarm value
          {
              flag_stop_success='1';    // flag 1 when stop successful
              if(flag_eng_trip=='1')
                flag_charge_alt_freq='0';
              if(flag_eng_trip=='2')
                flag_eng_fuel_press_ala='0';
              break;
          }
      }
      DISABLE_TIMER_7;     // stop (stop  timer )
      stop_tries_count++;
      temp_stop_tries++;
   }while(temp_stop_tries < stop_tries );   // end of while i.e. stopping after default tries
   DISABLE_TIMER_7;         //  stop ( stop )counter
   flag_all_timings='0';  // flag for dispaly board to indicate that stop timer stopped
   if(flag_mr_pressure_trip=='1'|| flag_key_stop_eng == 1)
   {
     flag_eng_fuel_press_ala='0';
     flag_charge_alt_freq='0';
   }

   if(flag_stop_success=='1')  // stop successful
   {
       DC_CONTACTOR=OFF;
       flag_comp_load_unload='1'; // comp loading
       COMP_LOADING=OFF;         //COMP_LOADING OFF( loading )
       flag_fail_to_stop='0';    //  stop successful
       flag_all_timings='0';  // flag for dispaly board to indicate that stop timer started
       check_485_query_and_send_frame(); //check for frame query & send if query received from master
       flag_apu_eng_on_off='0';  // apu eng OFF : flag used to decide apu eng on or off to display llop & frq alarms in on & off case  show    flag_eng_fuel_press_ala
       DISABLE_TIMER_7;
       if(flag_key_stop_eng== 1)           //if eng stopped manually HOOTER should remian OFF
       {
          flag_key_stop_eng=0;
          flag_stop_success='0';   // as APU eng stopped by key no need to acknoledge key as there is no common fault
       }
       else
       {
          HOOTER = ON;
          COMMON_FAULT = ON;
          for(f=0;f<30;f++)
          {
           Delay_ms(100);
           check_485_query_and_send_frame(); //check for frame query & send if query received from master
          }
          HOOTER = OFF;
       }
       LOCO_ENG_SHUTDOWN = OFF;
       check_loco_para_before_apu_start();
       check_all_apu_eng_para_in_apu_eng_off_mode();
       check_485_query_and_send_frame(); //check for frame query & send if query received from master
   }
   else
   {
   //    DC_CONTACTOR=OFF;
       flag_fail_to_stop='1';    //fail to stop
       flag_all_timings='0';  // flag for dispaly board to indicate that stop timer stopped
       flag_apu_eng_on_off='1';  // apu eng tried to be off but could not so display llop & frq alarms in off mode  ie show flag_eng_fuel_press_ala_in_stop  : flag used to decide apu eng on or off to display llop & frq alarms in on & off case
       HOOTER = ON;
       COMMON_FAULT = ON;
       for(f=0;f<30;f++)
       {
         Delay_ms(100);
         check_485_query_and_send_frame(); //check for frame query & send if query received from master
       }
       HOOTER = OFF;
       LOCO_ENG_SHUTDOWN = OFF;
       while(1)
       {
          COMMON_FAULT = ON;
          check_emegrgency_stop();
          APU_STATUS=1;  // APU is Unhealthy
          check_loco_para_before_apu_start();
          check_all_apu_eng_para_in_apu_eng_on_mode();
          check_485_query_and_send_frame(); //check for frame query & send if query received from master

          compare_eng_oil_pressure();
          check_charge_alt_freq();
          check_485_query_and_send_frame(); //check for frame query & send if query received from master
          if((flag_eng_fuel_press_ala == '1' && flag_charge_alt_freq == '1') || flag_emegrgency_stop =='1' )    //  check  eng oil press within limits  // check alt frq present  within limits
          {
               while(flag_fail_to_stop == '1')
               {
                   check_loco_para_before_apu_start();
                   if(flag_fail_to_stop=='1')
                    check_all_apu_eng_para_in_apu_eng_on_mode();
                   check_emegrgency_stop();
                   if(flag_emegrgency_stop =='1')
                    break;
               }
               flag_apu_eng_on_off='0';   // APU ENG stopped successfully as llop & freq is reduced after fail to stop therefore eng is OFF
           //    COMP_LOADING=OFF;         //COMP_LOADING OFF( loading )
           //    flag_comp_load_unload='1'; // comp loading
               COMMON_FAULT = OFF;
               DC_CONTACTOR=OFF;
               break;
          }
          else
           flag_fail_to_stop ='1';
       }
   }
   flag_key_stop_eng=0;
   write_run_minutes_in_eeprom();
   write_run_hours_in_eeprom();
}