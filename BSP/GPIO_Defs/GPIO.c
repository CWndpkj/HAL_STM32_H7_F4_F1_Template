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
#if USE_BOARD > 0
    else if ( gpio > 47 && gpio < 64 ) {
        return GPIOD;
    }
    else if ( gpio > 63 && gpio < 80 ) {
        return GPIOE;
    }
    else if ( gpio > 79 && gpio < 96 ) {
        return GPIOF;
    }
    else if ( gpio > 95 && gpio > 102 ) {
        return GPIOG;
    }
#endif
    else {
        return GPIOA;
    }
}

uint16_t GPIO_get_pin( GPIO_t gpio )
{
    return ( ( u16 )0x1 ) << ( gpio % 16 );
}