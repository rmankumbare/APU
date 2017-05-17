#include "headers.h"
#include "stdint.h"
#include "crc.h"
//------------calculation of CRC of given data------------------------------------------------------------------------------------------------------------------------
void MakeCRC(char *BitString, char* Res)
{
        char CRC[8];
        volatile int i;
        char DoInvert;

        // Initialization before calculation
        for (i = 0; i < 8; ++i)
                CRC[i] = 0;
        for (i = 0; i < strlen(BitString); ++i)
        {
                DoInvert = ('1' == BitString[i]) ^ CRC[7];         // XOR required
                CRC[7] = CRC[6];
                CRC[6] = CRC[5];
                CRC[5] = CRC[4];
                CRC[4] = CRC[3];
                CRC[3] = CRC[2];
                CRC[2] = CRC[1] ^ DoInvert;
                CRC[1] = CRC[0] ^ DoInvert;
                CRC[0] = DoInvert;
        }
        // Convert binary to ASCII
        for (i = 0; i < 8; ++i)
                Res[7 - i] = CRC[i] ? '1' : '0';
        // Set string terminator
        Res[8] = 0;
        return;
}

unsigned char CRC8CCITT(char* frame)
{
 volatile int i,j;
 unsigned char c=0;
 char crc[9] = {0};
 char byteBuf[9] = {0};
 char input[FRAME_DATA_SIZE_FOR_CRC*8+1] = {0};                        //FRAME_DATA_SIZE_FOR_CRC*8 for input bits, last 1 is null terminator

 //1.Convert the input into bit representation, byte by byte
 for(i=0 ; i<FRAME_DATA_SIZE_FOR_CRC ; i++)
 {
    memset(byteBuf,0,9);
    for( j=7; j>=0; --j)
    {
                byteBuf[7-j] = (frame[i] & (1 << j)) ? '1' : '0' ;
    }
        strcat(input,byteBuf);
 }

 //2.Perform CRC algorithm
 MakeCRC(input, crc);

 //3.Convert CRC to char
 for(j=0 ; j<8;j++)
 {
         c = c * 2;
         if(crc[j] == '1')
                 c = c + 1;
 }

 //5.Return char of crc
 return c;
}