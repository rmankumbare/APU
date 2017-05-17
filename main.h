
//-----------------------------SYSTEM-----------------------------------------------------------------------------------------
void system_init(void);
void  start_apu_engine(void);
void stop_apu(void);
void rs485_frame(void);
unsigned long check_loco_para_in_30_sec_timer(void);
void check_loco_para_in_30_sec_timer_for_driver_intervention(void);
void fuel_save_mode(void);
void algo_after_9_minutes_time_finished(void);
void manual_mode(void);
void auto_mode(void);