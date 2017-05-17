
 //-------------------------------Functions used in war_ala_check.c  file --------------------------------------------------------
void check_mr_pressure(void);
void check_bcp_pressure(void);
void check_loco_rpm(void);
void check_reverse_handle_position(void);
void check_loco_bat_vtg(void);
void check_apu_overload(void);
void check_dc_battery(void);

void compare_eng_oil_pressure(void);
void compare_eng_oil_pressure_in_stop_sequence(void);
void check_apu_low_bat_vtg(void);
void check_apu_high_bat_vtg(void);
//void check_apu_max_bat_vtg(void);
void check_eng_fuel_level(void);
void check_eng_cool_temp(void);

void check_charge_alt_freq(void);
void check_charge_alt_freq_in_stop_sequence(void);

void check_comp_air_temp(void);
void check_loco_bat_current(void);
void check_comp_air_pressure(void);
void check_ECU_inputs(void);
void check_loco_eng_oil_temp(void);
void check_loco_eng_water_temp(void);

float get_alt_freq_using_loco_timer_channel(void);
float get_loco_rpm_using_channel_pb4(void);
float get_loco_speed_using_channel_pd12(void);


void check_emegrgency_stop(void);
void check_operation_mode(void);
void check_excitation_fail(void);
void check_rectifier_over_temp_fault(void);
void check_connect_disconnect(void);
void check_mccb_feedback_status(void);
void check_dc_contactor_feedback_status(void);
void check_rectifier_temp(void);