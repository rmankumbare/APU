
//----------------------ADC -----------------------------------------------------------------------------------------------------
#define  ADC_COMP_AIR_PRESSURE               ADC1_Get_Sample(0);
#define  ADC_COMP_OIL_TEMP                   ADC1_Get_Sample(1);
#define  ADC_COMP_AIR_DISCHARGE_PRESSURE     ADC1_Get_Sample(2);
#define  ADC_COMP_DISCHARGE_OUTPUT_TEMP      ADC1_Get_Sample(3);
#define  ADC_ENG_FUEL_LEVEL                  ADC1_Get_Sample(4);
#define  ADC_COOLENT_TEMP                    ADC1_Get_Sample(5);
#define  ADC_MR_PRESSURE                     ADC1_Get_Sample(6);
#define  ADC_VOLT_ADJ                        ADC1_Get_Sample(7);
#define  ADC_ENGINE_FUEL_PRESSURE            ADC1_Get_Sample(10);
#define  ADC_AMBIENT_TEMP                    ADC1_Get_Sample(12);
#define  ADC_APU_BATTERY_SENSE               ADC1_Get_Sample(13);
#define  ADC_DC_CURRENT_SENSING              ADC1_Get_Sample(14);
#define  ADC_LOCO_BAT_DC_VTG_SENSE           ADC1_Get_Sample(15);

#define  ADC_RECTIFIER_TEMP                  ADC1_Get_Sample(9);  // canopy temp channel in schematic is used as rect temp

#define  ADC_DC_BATTERY                      ADC3_Get_Sample(14);
#define  ADC_BCP_PRESSURE                    ADC3_Get_Sample(15);
//#define  APU_ENABLE                          ADC1_Get_Sample(8);



//--------------------------------------------------------------------------------------------------------------------------------
void my_adc_init(void);

float scan_mr_pressure(void);
float scan_bcp_pressure(void);
unsigned long scan_coolent_temp();
float scan_voltage_adj();
float scan_dc_current_sensing();
float scan_loco_bat_dc_vtg_sense();
float scan_dc_battery();
float scan_engine_fuel_level();
unsigned long scan_comp_air_temp();
float scan_eng_fuel_pressure();
unsigned long scan_ambient_temp();
float apu_battery_sense();
float scan_comp_air_pressure();
float scan_loco_bat_current();
unsigned long scan_loco_eng_oil_temp();
unsigned long scan_loco_eng_water_temp();
unsigned long scan_rectifier_temp();