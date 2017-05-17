
#include "headers.h"

void port_configurations()
{ /*
  GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_3);   // PB 3
  GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_4);   // PB 4
  GPIO_Digital_Output(&GPIOG_BASE, _GPIO_PINMASK_ALL); // Set PORT G as digital output
  GPIO_Digital_Output(&GPIOE_BASE, _GPIO_PINMASK_13);   // PE 13

  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_0);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_1);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_2);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_3);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_4);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_5);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_6);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_7);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_8);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_9);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_10);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_11);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_12);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_14);
*/

  GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_3);   // PB 3
//  GPIO_Digital_Output(&GPIOB_BASE, _GPIO_PINMASK_4);   // PB 4
  GPIO_Digital_Output(&GPIOG_BASE, _GPIO_PINMASK_ALL); // Set PORT G as digital output
  GPIO_Digital_Output(&GPIOE_BASE, _GPIO_PINMASK_13);   // PE 13

  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_0);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_1);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_2);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_3);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_4);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_5);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_6);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_7);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_8);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_9);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_10);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_11);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_12);
  GPIO_Digital_input(&GPIOE_BASE, _GPIO_PINMASK_14);
  
}
