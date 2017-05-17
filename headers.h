//----------------------PARAMETERS SPANS---------------------------------------------------------------------------------------------
#define FRAME_DATA_SIZE_FOR_CRC 183

#define AVG_COUNT  2000
#define APU_74_V_AVG_COUNT  3000
#define COOLENT_TEMP_AVG_COUNT 4000
#define RECTIFIER_TEMP_AVG_COUNT 4000
#define CURRENTS_AVG_COUNT 4000


#define WAR_ALARM_CHECK_SET_COUNT 5   // this will check parameter 5 times before setting warning or alarm

#define ON   0
#define OFF  1

//---------------------------PARAMETER DEFAULT VALUE ADDRES IN FLASH---------------------------------------------------------------------------------------
#define BASE_ADDRESS   0x00
#define BASE_ADDRESS1  0x10
#define BASE_ADDRESS2  0x20


#define  ENG_FUEL_TANK_DEF_VAL_HIGH_ADD                      BASE_ADDRESS
#define  ENG_FUEL_TANK_DEF_VAL_LOW_ADD                       BASE_ADDRESS

#define FUEL_SELECT_DEF_VAL_HIGH_ADD                           BASE_ADDRESS
#define FUEL_SELECT_DEF_VAL_LOW_ADD                          0x06

#define  ENG_FUEL_LEVEL_SEL_DEF_VAL_HIGH_ADD                 BASE_ADDRESS
#define  ENG_FUEL_LEVEL_SEL_DEF_VAL_LOW_ADD                  0x0C            //ADD 6 in 0x0c so that next value is 0x12

#define  ENG_FUEL_LEVEL_CHECK_DEF_VAL_HIGH_ADD               BASE_ADDRESS
#define  ENG_FUEL_LEVEL_CHECK_DEF_VAL_LOW_ADD                0x12

#define ENG_FUEL_WARNING_SPAN_LL_VAL_HIGH_ADD                BASE_ADDRESS
#define ENG_FUEL_WARNING_SPAN_LL_VAL_LOW_ADD                 0x18

#define ENG_FUEL_WARNING_SPAN_HL_VAL_HIGH_ADD                BASE_ADDRESS
#define ENG_FUEL_WARNING_SPAN_HL_VAL_LOW_ADD                 0x1E

#define ENG_FUEL_ALARM_SPAN_LL_VAL_HIGH_ADD                  BASE_ADDRESS
#define ENG_FUEL_ALARM_SPAN_LL_VAL_LOW_ADD                   0x24

#define ENG_FUEL_ALARM_SPAN_HL_VAL_HIGH_ADD                  BASE_ADDRESS
#define ENG_FUEL_ALARM_SPAN_HL_VAL_LOW_ADD                   0x2A

#define ENG_FUEL_WARNING_DEF_VAL_HIGH_ADD                    BASE_ADDRESS
#define ENG_FUEL_WARNING_DEF_VAL_LOW_ADD                     0x30

#define ENG_FUEL_ALARM_DEF_VAL_HIGH_ADD                      BASE_ADDRESS
#define ENG_FUEL_ALARM_DEF_VAL_LOW_ADD                       0x36

#define ENG_COOL_TEMP_SEL_DEF_VAL_HIGH_ADD                   BASE_ADDRESS
#define ENG_COOL_TEMP_SEL_DEF_VAL_LOW_ADD                    0x3C

#define ENG_COOL_TEMP_CHECK_DEF_VAL_HIGH_ADD                 BASE_ADDRESS
#define ENG_COOL_TEMP_CHECK_DEF_VAL_LOW_ADD                  0x42

#define ENG_OIL_PRESS_SEL_DEF_VAL_HIGH_ADD                   BASE_ADDRESS
#define ENG_OIL_PRESS_SEL_DEF_VAL_LOW_ADD                    0x48

#define ENG_OIL_PRESS_CHECK_DEF_VAL_HIGH_ADD                 BASE_ADDRESS
#define ENG_OIL_PRESS_CHECK_DEF_VAL_LOW_ADD                  0x4E

#define COMP_AIR_PRESS_SEL_DEF_VAL_HIGH_ADD                  BASE_ADDRESS
#define COMP_AIR_PRESS_SEL_DEF_VAL_LOW_ADD                   0x54

#define COMP_AIR_PRESS_CHECK_DEF_VAL_HIGH_ADD                BASE_ADDRESS
#define COMP_AIR_PRESS_CHECK_DEF_VAL_LOW_ADD                 0x5A

#define COMP_AIR_TEMP_SEL_DEF_VAL_HIGH_ADD                   BASE_ADDRESS
#define COMP_AIR_TEMP_SEL_DEF_VAL_LOW_ADD                    0x60

#define COMP_AIR_TEMP_CHECK_DEF_VAL_HIGH_ADD                 BASE_ADDRESS
#define COMP_AIR_TEMP_CHECK_DEF_VAL_LOW_ADD                  0x66

#define COMP_OIL_TEMP_SEL_DEF_VAL_HIGH_ADD                   BASE_ADDRESS
#define COMP_OIL_TEMP_SEL_DEF_VAL_LOW_ADD                    0x6C

#define COMP_OIL_TEMP_CHECK_DEF_VAL_HIGH_ADD                 BASE_ADDRESS
#define COMP_OIL_TEMP_CHECK_DEF_VAL_LOW_ADD                  0x72

#define MR_AIR_PRESS_CHECK_DEF_VAL_HIGH_ADD                  BASE_ADDRESS
#define MR_AIR_PRESS_CHECK_DEF_VAL_LOW_ADD                   0x78

#define BCP_PRESS_CHECK_DEF_VAL_HIGH_ADD                     BASE_ADDRESS
#define BCP_PRESS_CHECK_DEF_VAL_LOW_ADD                      0x7E

#define IDLE_LOCO_SPEED_CHECK_DEF_VAL_HIGH_ADD               BASE_ADDRESS
#define IDLE_LOCO_SPEED_CHECK_DEF_VAL_LOW_ADD                0x84

#define RESERVE_HANDLE_POS_CHECK_DEF_VAL_HIGH_ADD            BASE_ADDRESS
#define RESERVE_HANDLE_POS_CHECK_DEF_VAL_LOW_ADD             0x8A

#define LOCO_BAT_VTG_CHECK_DEF_VAL_HIGH_ADD                  BASE_ADDRESS
#define LOCO_BAT_VTG_CHECK_DEF_VAL_LOW_ADD                   0x90

#define OVERLOAD_CHECK_DEF_VAL_HIGH_ADD                      BASE_ADDRESS
#define OVERLOAD_CHECK_DEF_VAL_LOW_ADD                       0x96

#define APU_BAT_VTG_DEF_VAL_HIGH_ADD                         BASE_ADDRESS
#define APU_BAT_VTG_DEF_VAL_LOW_ADD                              0x9C

#define EXCITE_CHECK_DEF_VAL_HIGH_ADD                        BASE_ADDRESS
#define EXCITE_CHECK_DEF_VAL_LOW_ADD                         0xA2

#define RPM_CHECK_DEF_VAL_HIGH_ADD                           BASE_ADDRESS
#define RPM_CHECK_DEF_VAL_LOW_ADD                            0xA8

#define ENG_COOL_TEMP_WARNING_DEF_VAL_HIGH_ADD              BASE_ADDRESS
#define ENG_COOL_TEMP_WARNING_DEF_VAL_LOW_ADD                0xAE

#define ENG_COOL_TEMP_ALARM_DEF_VAL_HIGH_ADD                 BASE_ADDRESS
#define ENG_COOL_TEMP_ALARM_DEF_VAL_LOW_ADD                  0xB4

#define ENG_OIL_PRESS_WARNING_DEF_VAL_HIGH_ADD              BASE_ADDRESS
#define ENG_OIL_PRESS_WARNING_DEF_VAL_LOW_ADD               0xBA

#define ENG_OIL_PRESS_ALARM_DEF_VAL_HIGH_ADD                BASE_ADDRESS
#define ENG_OIL_PRESS_ALARM_DEF_VAL_LOW_ADD                 0xC0

#define COMP_AIR_PRESS_WARNING_DEF_VAL_HIGH_ADD              BASE_ADDRESS
#define COMP_AIR_PRESS_WARNING_DEF_VAL_LOW_ADD               0xC6

#define COMP_AIR_PRESS_ALARM_DEF_VAL_HIGH_ADD                BASE_ADDRESS
#define COMP_AIR_PRESS_ALARM_DEF_VAL_LOW_ADD                0xCC

#define COMP_AIR_TEMP_WARNING_DEF_VAL_HIGH_ADD               BASE_ADDRESS
#define COMP_AIR_TEMP_WARNING_DEF_VAL_LOW_ADD                0xD2

#define COMP_AIR_TEMP_ALARM_DEF_VAL_HIGH_ADD                BASE_ADDRESS
#define COMP_AIR_TEMP_ALARM_DEF_VAL_LOW_ADD                  0xD8

#define COMP_OIL_TEMP_WARNING_DEF_VAL_HIGH_ADD                BASE_ADDRESS
#define COMP_OIL_TEMP_WARNING_DEF_VAL_LOW_ADD                0xDE

#define COMP_OIL_TEMP_ALARM_DEF_VAL_HIGH_ADD                BASE_ADDRESS
#define COMP_OIL_TEMP_ALARM_DEF_VAL_LOW_ADD                0xE4

#define MR_AIR_PRESS_WARNING_DEF_VAL_HIGH_ADD               BASE_ADDRESS
#define MR_AIR_PRESS_WARNING_DEF_VAL_LOW_ADD                0xEA

#define MR_AIR_PRESS_ALARM_DEF_VAL_HIGH_ADD                 BASE_ADDRESS
#define MR_AIR_PRESS_ALARM_DEF_VAL_LOW_ADD                  0xF0

#define BCP_PRESS_WARNING_DEF_VAL_HIGH_ADD                 BASE_ADDRESS
#define BCP_PRESS_WARNING_DEF_VAL_LOW_ADD                  0xF6

#define BCP_PRESS_ALARM_DEF_VAL_HIGH_ADD                   BASE_ADDRESS
#define BCP_PRESS_ALARM_DEF_VAL_LOW_ADD                    0xFC

//-----------------------256 BYTES FINISHED SO CHABGE BASE ADDRESS TO BASE_ADDRESS1 -------------------------------------------------------------------------------------------------------------------------------

#define LOCO_BAT_HIGH_VTG_WARNING_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define LOCO_BAT_HIGH_VTG_WARNING_DEF_VAL_LOW_ADD          0x00

#define LOCO_BAT_HIGH_VTG_ALARM_DEF_VAL_HIGH_ADD           BASE_ADDRESS1
#define LOCO_BAT_HIGH_VTG_ALARM_DEF_VAL_LOW_ADD            0x06

#define LOCO_BAT_HIGH_VTG_DELAY_DEF_VAL_HIGH_ADD           BASE_ADDRESS1
#define LOCO_BAT_HIGH_VTG_DELAY_DEF_VAL_LOW_ADD             0x0C

#define LOCO_BAT_LOW_VTG_WARNING_DEF_VAL_HIGH_ADD        BASE_ADDRESS1
#define LOCO_BAT_LOW_VTG_WARNING_DEF_VAL_LOW_ADD            0x12

#define LOCO_BAT_LOW_VTG_ALARM_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define LOCO_BAT_LOW_VTG_ALARM_DEF_VAL_LOW_ADD             0x18

#define LOCO_BAT_LOW_VTG_DELAY_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define LOCO_BAT_LOW_VTG_DELAY_DEF_VAL_LOW_ADD              0x1E

#define OVERLOAD_WARNING_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define OVERLOAD_WARNING_DEF_VAL_LOW_ADD              0x24

#define OVERLOAD_ALARM_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define OVERLOAD_ALARM_DEF_VAL_LOW_ADD             0x2A

#define OVERLOAD_DELAY_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define OVERLOAD_DELAY_DEF_VAL_LOW_ADD              0x30

#define APU_LOW_BAT_WARNING_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define APU_LOW_BAT_WARNING_DEF_VAL_LOW_ADD             0x36

#define APU_HIGH_BAT_WARNING_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define APU_HIGH_BAT_WARNING_DEF_VAL_LOW_ADD             0x3C

#define APU_MAX_BAT_WARNING_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define APU_MAX_BAT_WARNING_DEF_VAL_LOW_ADD              0x42

#define APU_BAT_CHARGE_MIN_VTG_DEF_VAL_HIGH_ADD        BASE_ADDRESS1    ////excitation vtg limit
#define APU_BAT_CHARGE_MIN_VTG_DEF_VAL_LOW_ADD             0x48

#define COOLING_TIME_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define COOLING_TIME_DEF_VAL_LOW_ADD              0x4E

#define MAINTAINANCE_TIME_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define MAINTAINANCE_TIME_DEF_VAL_LOW_ADD             0x54

#define HOOTER_ON_TIME_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define HOOTER_ON_TIME_DEF_VAL_LOW_ADD             0x5A

#define HIGH_RPM_DELAY_DEF_VAL_HIGH_ADD        BASE_ADDRESS1
#define HIGH_RPM_DELAY_DEF_VAL_LOW_ADD             0x60

#define CRANK_TIME_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define CRANK_TIME_DEF_VAL_LOW_ADD             0x66

#define CRANK_REST_TIME_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define CRANK_REST_TIME_DEF_VAL_LOW_ADD              0x6C

#define CRANK_TRIES_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define CRANK_TRIES_DEF_VAL_LOW_ADD              0x72

#define RUN_UP_TIME_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define RUN_UP_TIME_DEF_VAL_LOW_ADD             0x78

#define WARM_UP_TIME_DEF_VAL_HIGH_ADD        BASE_ADDRESS1
#define WARM_UP_TIME_DEF_VAL_LOW_ADD             0x7E

#define STOP_TIME_DEF_VAL_HIGH_ADD        BASE_ADDRESS1
#define STOP_TIME_DEF_VAL_LOW_ADD             0X84

#define STOP_REST_TIME_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define STOP_REST_TIME_DEF_VAL_LOW_ADD             0x8A

#define STOP_TRIES_DEF_VAL_HIGH_ADD        BASE_ADDRESS1
#define STOP_TRIES_DEF_VAL_LOW_ADD             0X90

#define SITE_ID_DEF_VAL_HIGH_ADD         BASE_ADDRESS1
#define SITE_ID_DEF_VAL_LOW_ADD             0x96

#define LOW_RPM_DELAY_DEF_VAL_HIGH_ADD        BASE_ADDRESS1
#define LOW_RPM_DELAY_DEF_VAL_LOW_ADD              0x9C

#define LOCO_SPEED_WARNING_DEF_VAL_HIGH_ADD    BASE_ADDRESS1
#define LOCO_SPEED_WARNING_DEF_VAL_LOW_ADD        0xA2

#define LOCO_SPEED_ALARM_DEF_VAL_HIGH_ADD    BASE_ADDRESS1
#define LOCO_SPEED_ALARM_DEF_VAL_LOW_ADD        0xA8

#define GEAR_TEETH_DEF_VAL_HIGH_ADD          BASE_ADDRESS1
#define GEAR_TEETH_DEF_VAL_LOW_ADD            0xAE

#define PREHEAT_COUNTER_DEF_VAL_HIGH_ADD          BASE_ADDRESS1
#define PREHEAT_COUNTER_DEF_VAL_LOW_ADD            0xB4

#define CHARGE_ALT_FREQUENCY_DEF_VAL_HIGH_ADD   BASE_ADDRESS1
#define CHARGE_ALT_FREQUENCY_DEF_VAL_LOW_ADD     0xBA

#define  MINUTES_COUNT_VAL_HIGH_ADD           BASE_ADDRESS1
#define  MINUTES_COUNT_VAL_LOW_ADD           0xC0

#define  HOURS_COUNT_VAL_HIGH_ADD              BASE_ADDRESS2
#define  HOURS_COUNT_VAL_LOW_ADD               0x00

#define  MAINTAINANCE_HOUR_COUNT_VAL_HIGH_ADD    BASE_ADDRESS2
#define  MAINTAINANCE_HOUR_COUNT_VAL_LOW_ADD      0x06

//-----------------------when value reachec 0xFC use BASE_ADDRESS2---------------- -------------------------------------

//-------------------------------------------------------------------------------------------------------------------------------------------------
#define NEWLINE           UART6_Sendchar(10);
#define NEWLINE_1           UART6_Sendchar(13);


//---------Additions in headers.h file with GUI programming-----------------------------------------------------------------------------------------------------------------------------------------
#define LOCO_ENG_OIL_TEMP_WARNING_DEF_VAL_HIGH_ADD  BASE_ADDRESS2
#define LOCO_ENG_OIL_TEMP_WARNING_DEF_VAL_LOW_ADD    0x0C

#define LOCO_ENG_WATER_TEMP_WARNING_DEF_VAL_HIGH_ADD  BASE_ADDRESS2
#define LOCO_ENG_WATER_TEMP_WARNING_DEF_VAL_LOW_ADD    0x12

#define LOCO_ENG_OIL_TEMP_ALARM_DEF_VAL_HIGH_ADD  BASE_ADDRESS2
#define LOCO_ENG_OIL_TEMP_ALARM_DEF_VAL_LOW_ADD    0x18

#define LOCO_ENG_WATER_TEMP_ALARM_DEF_VAL_HIGH_ADD  BASE_ADDRESS2
#define LOCO_ENG_WATER_TEMP_ALARM_DEF_VAL_LOW_ADD    0x1E

#define LOCO_RPM_WARNING_DEF_VAL_HIGH_ADD    BASE_ADDRESS2
#define LOCO_RPM_WARNING_DEF_VAL_LOW_ADD        0x24

#define LOCO_RPM_ALARM_DEF_VAL_HIGH_ADD    BASE_ADDRESS2
#define LOCO_RPM_ALARM_DEF_VAL_LOW_ADD        0x2A


#define APU_74_BAT_HIGH_VTG_WARNING_DEF_VAL_HIGH_ADD         BASE_ADDRESS2
#define APU_74_BAT_HIGH_VTG_WARNING_DEF_VAL_LOW_ADD          0x30

#define APU_74_BAT_HIGH_VTG_ALARM_DEF_VAL_HIGH_ADD           BASE_ADDRESS2
#define APU_74_BAT_HIGH_VTG_ALARM_DEF_VAL_LOW_ADD            0x36

#define APU_74_BAT_LOW_VTG_WARNING_DEF_VAL_HIGH_ADD        BASE_ADDRESS2
#define APU_74_BAT_LOW_VTG_WARNING_DEF_VAL_LOW_ADD            0x3C

#define APU_74_BAT_LOW_VTG_ALARM_DEF_VAL_HIGH_ADD         BASE_ADDRESS2
#define APU_74_BAT_LOW_VTG_ALARM_DEF_VAL_LOW_ADD             0x42

#define LOCO_BAT_CURRENT_WARNING_DEF_VAL_HIGH_ADD        BASE_ADDRESS2
#define LOCO_BAT_CURRENT_WARNING_DEF_VAL_LOW_ADD            0x48

#define LOCO_BAT_CURRENT_ALARM_DEF_VAL_HIGH_ADD         BASE_ADDRESS2
#define LOCO_BAT_CURRENT_ALARM_DEF_VAL_LOW_ADD             0x4E


//----------------GUI calibrattion----------------------------------------------------------------------------------------------------------------------------------
#define LOCO_ENG_OIL_TEMP_GUI_VAL_HIGH_ADD  BASE_ADDRESS2
#define LOCO_ENG_OIL_TEMP_GUI_VAL_LOW_ADD    0x0C

#define LOCO_ENG_WATER_TEMP_GUI_VAL_HIGH_ADD  BASE_ADDRESS2
#define LOCO_ENG_WATER_TEMP_GUI_VAL_LOW_ADD    0x12

#define APU_74_BAT_GUI_VAL_HIGH_ADD         BASE_ADDRESS2
#define APU_74_BAT_GUI_VAL_LOW_ADD          0x30

#define LOCO_BAT_CURRENT_GUI_VAL_HIGH_ADD         BASE_ADDRESS2
#define LOCO_BAT_CURRENT_GUI_VAL_LOW_ADD             0x4E

#define COMP_AIR_PRESS_ALARM_DEF_VAL_HIGH_ADD                BASE_ADDRESS
#define COMP_AIR_PRESS_ALARM_DEF_VAL_LOW_ADD                0xCC



//----------------GUI calibrattion---Linear-------------------------------------------------------------------------------------------------------------------------------
#define APU_BAT_VTG_DIFFERENCE_VAL_HIGH_ADD                             BASE_ADDRESS2
#define APU_BAT_VTG_DIFFERENCE_VAL_LOW_ADD                              0x54

#define APU_74_BAT_DIFFERENCE_VAL_HIGH_ADD                              BASE_ADDRESS2
#define APU_74_BAT_DIFFERENCE_VAL_LOW_ADD                               0x5A

#define OVERLOAD_DIFFERENCE_VAL_HIGH_ADD                         BASE_ADDRESS2             //APU current
#define OVERLOAD_DIFFERENCE_VAL_LOW_ADD                          0x60

#define BCP_PRESS_DIFFERENCE_VAL_HIGH_ADD                      BASE_ADDRESS2
#define BCP_PRESS_DIFFERENCE_VAL_LOW_ADD                        0x66

#define LOCO_BAT_HIGH_VTG_DIFFERENCE_VAL_HIGH_ADD             BASE_ADDRESS2
#define LOCO_BAT_HIGH_VTG_DIFFERENCE_VAL_LOW_ADD              0x6C

#define LOCO_BAT_CURRENT_DIFFERENCE_VAL_HIGH_ADD                    BASE_ADDRESS2
#define LOCO_BAT_CURRENT_DIFFERENCE_VAL_LOW_ADD                      0x72

#define LOCO_ENG_OIL_TEMP_DIFFERENCE_VAL_HIGH_ADD                    BASE_ADDRESS2
#define LOCO_ENG_OIL_TEMP_DIFFERENCE_VAL_LOW_ADD                     0x78

#define LOCO_ENG_WATER_TEMP_DIFFERENCE_VAL_HIGH_ADD                  BASE_ADDRESS2
#define LOCO_ENG_WATER_TEMP_DIFFERENCE_VAL_LOW_ADD                    0x7E

#define COMP_AIR_PRESS_DIFFERENCE_VAL_HIGH_ADD                BASE_ADDRESS2
#define COMP_AIR_PRESS_DIFFERENCE_VAL_LOW_ADD                 0x84

#define ENG_FUEL_LEVEL_DIFFERENCE_VAL_HIGH_ADD                BASE_ADDRESS2
#define ENG_FUEL_LEVEL_DIFFERENCE_VAL_LOW_ADD                 0x8A

#define BASE_ADDRESS3  0x30

#define APU_BAT_VTG_DIFFERENCE_SIGN_VAL_HIGH_ADD                             BASE_ADDRESS3
#define APU_BAT_VTG_DIFFERENCE_SIGN_VAL_LOW_ADD                              0x00

#define APU_74_BAT_DIFFERENCE_SIGN_VAL_HIGH_ADD                              BASE_ADDRESS3
#define APU_74_BAT_DIFFERENCE_SIGN_VAL_LOW_ADD                               0x01

#define OVERLOAD_DIFFERENCE_SIGN_VAL_HIGH_ADD                         BASE_ADDRESS3             //APU current
#define OVERLOAD_DIFFERENCE_SIGN_VAL_LOW_ADD                          0x02

#define BCP_PRESS_DIFFERENCE_SIGN_VAL_HIGH_ADD                      BASE_ADDRESS3
#define BCP_PRESS_DIFFERENCE_SIGN_VAL_LOW_ADD                        0x03

#define LOCO_BAT_HIGH_VTG_DIFFERENCE_SIGN_VAL_HIGH_ADD             BASE_ADDRESS3
#define LOCO_BAT_HIGH_VTG_DIFFERENCE_SIGN_VAL_LOW_ADD              0x04

#define LOCO_BAT_CURRENT_DIFFERENCE_SIGN_VAL_HIGH_ADD                    BASE_ADDRESS3
#define LOCO_BAT_CURRENT_DIFFERENCE_SIGN_VAL_LOW_ADD                      0x05

#define LOCO_ENG_OIL_TEMP_DIFFERENCE_SIGN_VAL_HIGH_ADD                    BASE_ADDRESS3
#define LOCO_ENG_OIL_TEMP_DIFFERENCE_SIGN_VAL_LOW_ADD                     0x06

#define LOCO_ENG_WATER_TEMP_DIFFERENCE_SIGN_VAL_HIGH_ADD                  BASE_ADDRESS3
#define LOCO_ENG_WATER_TEMP_DIFFERENCE_SIGN_VAL_LOW_ADD                    0x07

#define COMP_AIR_PRESS_DIFFERENCE_SIGN_VAL_HIGH_ADD                BASE_ADDRESS3
#define COMP_AIR_PRESS_DIFFERENCE_SIGN_VAL_LOW_ADD                 0x08

#define ENG_FUEL_LEVEL_DIFFERENCE_SIGN_VAL_HIGH_ADD                BASE_ADDRESS3
#define ENG_FUEL_LEVEL_DIFFERENCE_SIGN_VAL_LOW_ADD                 0x09

//---------------device ID------------------------------------------------------------------------------------------------------------------------------

#define BASE_ADDRESS4  0x40

#define DEVICE_ID_HIGH_ADD   BASE_ADDRESS4
#define DEVICE_ID_LOW_ADD    0x00         // need 10 bytes for device ID so next should be 0x0B

//--------------comp Load unloading -----------------------------------------------------------------------------------------------------------------------------
#define BASE_ADDRESS5  0x50

#define COMP_LOADING_VALUE_HIGH_ADD    BASE_ADDRESS5
#define COMP_LOADING_VALUE_LOW_ADD     0x00

#define COMP_UNLOADING_VALUE_HIGH_ADD    BASE_ADDRESS5
#define COMP_UNLOADING_VALUE_LOW_ADD     0x06
//------------------FTP ON OFF -------------------------------------------------------------------------------------------------------------------------

#define FTP_ON_VALUE_HIGH_ADD    BASE_ADDRESS5
#define FTP_ON_VALUE_LOW_ADD     0x0C

#define FTP_OFF_VALUE_HIGH_ADD    BASE_ADDRESS5
#define FTP_OFF_VALUE_LOW_ADD     0x12

//-------------------------------------------------------------------------------------------------------------------------------------------