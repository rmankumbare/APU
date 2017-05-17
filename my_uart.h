//-------------------------------------------------------------------------------------------------------------------------------------------------
#define NEWLINE           UART6_Sendchar(10);
#define NEWLINE_1           UART6_Sendchar(13);
//------------------------------UART------------------------------------------------------------------------------------------
 void UART1_Sendchar(unsigned char send_data);
 void UART1_PrintString(unsigned char *pcString);
 unsigned char UART1_readchar( void);

 void UART6_Sendchar(unsigned char send_data);
 void UART6_PrintString(unsigned char *pcString);
 unsigned char UART6_readchar( void);
 