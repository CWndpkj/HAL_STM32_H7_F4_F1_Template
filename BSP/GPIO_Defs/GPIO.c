#include "GPIO.h"
GPIO_TypeDef* GPIO_get_port( GPIO_t gpio )
{
    if ( gpio < 16 ) {
        return GPIOA;
    }
    else if ( gpio > 15 && gpio < 32 ) {
        return GPIOB;
    }
    else if ( gpio > 31 && gpio < 48 ) {
        return GPIOC;
    }
    else {
        return GPIOA;
    }
}

uint16_t GPIO_get_pin( GPIO_t gpio )
{
    return ( ( u16 )0x1 ) << ( gpio % 16 );
}