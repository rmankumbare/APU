
  //------------------------------EEPROM---------------------------------------------------------------------------------------
 void write_all_default_values_in_eeprom(void);
 void write_run_minutes_in_eeprom(void);
 void write_run_hours_in_eeprom(void);
 void write_maintainance_hour_count_val_in_eeprom(void);

 void Write_single_byte_to_eeprom(unsigned char  high_byte,unsigned char low_byte, unsigned char data_byte);
 unsigned char read_single_byte_from_eeprom(unsigned char  high_byte,unsigned char low_byte);
 void write_eng_fuel_war_ala_spans_in_eeprom(unsigned long span_data , short int id);

 float get_mr_press_war_val_from_eeprom(void);
 float get_mr_press_ala_val_from_eeprom(void);
 unsigned long get_loco_rpm_war_val_from_eeprom(void);
 unsigned long get_loco_rpm_ala_val_from_eeprom(void);
 float get_bcp_press_war_val_from_eeprom(void);
 float get_bcp_press_ala_val_from_eeprom(void);
 unsigned long get_eng_cool_temp_war_val_from_eeprom(void);  // ==  main eng water temp
 unsigned long get_eng_cool_temp_ala_val_from_eeprom(void);  // ==  main eng water temp
 unsigned long get_eng_fuel_level_war_val_from_eeprom(void);
 unsigned long get_eng_fuel_level_ala_val_from_eeprom(void);
 float eng_fuel_press_war_val_from_eeprom(void);    // == ENG OIL PRESSURE
 float eng_fuel_press_ala_val_from_eeprom(void);    // == ENG OIL PRESSURE
 unsigned long get_preheat_counter_val_from_eeprom(void);
 unsigned long get_crank_counter_val_from_eeprom(void);
 unsigned long get_crank_rest_counter_val_from_eeprom(void);
 unsigned long get_warm_up_counter_val_from_eeprom(void);
 unsigned long get_run_up_counter_val_from_eeprom(void);
 unsigned long get_stop_counter_val_from_eeprom(void);
 unsigned long get_stop_rest_counter_val_from_eeprom(void);
 unsigned long get_crank_tries_val_from_eeprom(void);
 unsigned long get_stop_tries_val_from_eeprom(void);
 float get_charge_alt_frequency_val_from_eeprom(void);
 float get_apu_low_bat_war_val_from_eeprom(void);
 float get_apu_high_bat_war_val_from_eeprom(void);
 float get_apu_max_bat_war_val_from_eeprom(void);
 unsigned long get_comp_air_temp_war_val_from_eeprom(void);
 unsigned long get_comp_air_temp_ala_val_from_eeprom(void);
 unsigned long get_comp_oil_temp_war_val_from_eeprom(void);
 unsigned long get_comp_oil_temp_ala_val_from_eeprom(void);
 float get_loco_low_bat_war_val_from_eeprom(void);
 float get_loco_low_bat_ala_val_from_eeprom(void);
 float get_loco_high_bat_war_val_from_eeprom(void);
 float get_loco_high_bat_ala_val_from_eeprom(void);
 float get_loco_bat_current_war_val_from_eeprom(void);
 float get_loco_bat_current_ala_val_from_eeprom(void);
 float get_comp_air_press_ala_val_from_eeprom(void);
 float get_comp_air_press_war_val_from_eeprom(void);

 float get_excitation_vtg_val_from_eeprom(void);
 unsigned long get_maintainance_hour_count_val_from_eeprom(void);
 
 
  unsigned long get_loco_eng_water_temp_ala_val_from_eeprom(void);
 unsigned long get_loco_eng_water_temp_war_val_from_eeprom(void);
 unsigned long get_loco_eng_oil_temp_war_val_from_eeprom(void);
 unsigned long get_loco_eng_oil_temp_ala_val_from_eeprom(void);
 unsigned long get_loco_speed_war_val_from_eeprom(void);
 unsigned long get_loco_speed_ala_val_from_eeprom(void);
 float get_apu_current_war_val_from_eeprom(void);
 float get_apu_current_ala_val_from_eeprom(void);
 float get_apu_74_low_bat_war_val_from_eeprom(void);
 float get_apu_74_low_bat_ala_val_from_eeprom(void);
 float get_apu_74_high_bat_war_val_from_eeprom(void);
 float get_apu_74_high_bat_ala_val_from_eeprom(void);
    unsigned long get_run_minutes_val_from_eeprom(void);
 unsigned long get_run_hours_val_from_eeprom(void);
 
 
  float get_GUI_calibrated_val_from_eeprom(unsigned char  high_byte,unsigned char low_byte);
  
  