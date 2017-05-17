#include "headers.h"
#include "stdint.h"
#include "my_uart.h"
#include "freq_capture.h"


//--------------------------GLOBAL VARIABLES---------------------------------------------------------------------------------------------------------------
unsigned int flag_4=0 ,done_flag_4=0,flag_2=0 ,done_flag_2=0,flag_1=0 ,done_flag_3=0,flag_3=0 ;
unsigned int done_flag_1=0,charge_alt_freq_avg=0;
float temp_charge_alt_freq=0.0;
double end_time_4 = 0, start_time_4=0,end_time_3 = 0, start_time_3=0,  end_time_2 = 0,start_time_2=0,end_time_1 = 0,start_time_1=0;
double  pulse_ticks_4 = 0,pulse_ticks_3 = 0,pulse_ticks_2 = 0,pulse_ticks_1 = 0,stop_time_1 = 0;
double calc_freq_capture_1, calc_freq_capture_2,calc_freq_capture_3,calc_freq_capture_4,temp,arr_value;
//---------------------------EXTERN  VARIABLES-------------------------------------------------------------------------------------------------------------------------

extern float  charge_alt_freq,apu_rpm;

 //---------------------------------------------------------------------------------------------------------------------------------------------------


float get_charge_alt_frequency(void)
{
  arr_value=  1.0/ 65535.0;
  if(done_flag_1==1)
   {
    pulse_ticks_1= end_time_1 - start_time_1;
    pulse_ticks_1= (fabs(pulse_ticks_1) );

    temp =arr_value * (double)pulse_ticks_1;
    calc_freq_capture_1 = ( 1.0 /temp);
   return calc_freq_capture_1;
   }
  else
   return 0.0;
}
void Timer2_interrupt() iv IVT_INT_TIM2
{
    TIM2_SR.UIF = 0;
    if(TIM2_SRbits.CC1IF == 1)
    {
         TIM2_SRbits.CC1IF =0;
         flag_2++;
         if(flag_2==1)
           start_time_2 = TIM2_CCR1;
         if(flag_2==2)
         {
            flag_2=0;
            end_time_2 = TIM2_CCR1;
            done_flag_2=1;
         }
    }
}

float get_alt_freq_using_loco_timer_channel(void)
{
 return get_loco_rpm_using_channel_pb4();
}

//--------------------LOCO RPM On Channel PB4----------------------------------------------------------------------------------------------------------------------------------------------

void Timer3_interrupt() iv IVT_INT_TIM3
{
  TIM3_SR.UIF = 0;
  if(TIM3_SRbits.CC1IF == 1)
  {
   TIM3_SRbits.CC1IF =0;
   flag_3++;
   if(flag_3==1)
     start_time_3 = TIM3_CCR1;
   if(flag_3==2)
   {
    flag_3=0;
    end_time_3 = TIM3_CCR1;
    done_flag_3=1;
   }
  }
}

float get_loco_rpm_using_channel_pb4(void)
{
// uint8_t buf[10],rx_data[5];


  arr_value=  1.0/ 65535.0;
  if(done_flag_3==1)
   {
    if(end_time_3 > start_time_3)
      pulse_ticks_3 = end_time_3 - start_time_3;
    else
       pulse_ticks_3=  start_time_3 -end_time_3;

    pulse_ticks_3= (fabs(pulse_ticks_3) );
    temp = arr_value * (double)pulse_ticks_3;
    calc_freq_capture_3 = ( 1.0 /temp);

    done_flag_3=0;
    return calc_freq_capture_3;
   }
 if(done_flag_3==0)
 {
  return 0.0;
 }
}
//--------------------LOCO SPEED On Channel PD12----------------------------------------------------------------------------------------------------------------------------------------------
void Timer4_interrupt() iv IVT_INT_TIM4
{
  TIM4_SR.UIF = 0;
  if(TIM4_SRbits.CC1IF == 1)
  {
     TIM4_SRbits.CC1IF =0;
     flag_4++;
     if(flag_4==1)
       start_time_4 = TIM4_CCR1;
     if(flag_4==2)
     {
      flag_4=0;
      end_time_4 = TIM4_CCR1;
      done_flag_4=1;
     }
  }
}

