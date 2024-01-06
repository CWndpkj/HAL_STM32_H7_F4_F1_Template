#include "vl53l0x.h"
#include "I2C_Helper.h"

extern I2C_HandleTypeDef hi2c3;
VL53L0X_Dev_t vl53l0x_dev = { .Id = 1, .DevLetter = 'F', .I2cDevAddr = 0x52 };
VL53L0X_RangingMeasurementData_t RangingMeasurementData;

void vl53l0x_init()
{
    // 初始化GPIO,I2C
    I2C_DEV_t i2c_dev;
    i2c_dev.I2C_x = I2C_3;
    I2C_Helper_FailRecover( &i2c_dev );
    I2C_Helper_Init( &hi2c3, &i2c_dev );

    VL53L0X_WrByte( &vl53l0x_dev, 0x88, 0x0 );
    uint16_t Id;
    VL53L0X_RdWord( &vl53l0x_dev, VL53L0X_REG_IDENTIFICATION_MODEL_ID, &Id );

    VL53L0X_DataInit( &vl53l0x_dev );
    vl53l0x_dev.Present = 1;
    VL53L0X_StaticInit( &vl53l0x_dev );
}

int LeakyFactorFix8 = ( int )( 0.6 * 256 );
void Sensor_SetNewRange( VL53L0X_Dev_t* pDev, VL53L0X_RangingMeasurementData_t* pRange )
{
    if ( pRange->RangeStatus == 0 ) {
        if ( pDev->LeakyFirst ) {
            pDev->LeakyFirst = 0;
            pDev->LeakyRange = pRange->RangeMilliMeter;
        }
        else {
            pDev->LeakyRange = ( pDev->LeakyRange * LeakyFactorFix8 + ( 256 - LeakyFactorFix8 ) * pRange->RangeMilliMeter ) >> 8;
        }
    }
    else {
        pDev->LeakyFirst = 1;
    }
}

void SetupSingleShot( RangingConfig_e rangingConfig )
{
    int status;
    uint8_t VhvSettings;
    uint8_t PhaseCal;
    uint32_t refSpadCount;
    uint8_t isApertureSpads;
    FixPoint1616_t signalLimit    = ( FixPoint1616_t )( 0.25 * 65536 );
    FixPoint1616_t sigmaLimit     = ( FixPoint1616_t )( 18 * 65536 );
    uint32_t timingBudget         = 33000;
    uint8_t preRangeVcselPeriod   = 14;
    uint8_t finalRangeVcselPeriod = 10;

    status = VL53L0X_StaticInit( &vl53l0x_dev );
    if ( status ) {
        printf( "VL53L0X_StaticInit %d failed\n", status );
    }

    status = VL53L0X_PerformRefCalibration( &vl53l0x_dev, &VhvSettings, &PhaseCal );
    if ( status ) {
        printf( "VL53L0X_PerformRefCalibration failed\n" );
    }

    status = VL53L0X_PerformRefSpadManagement( &vl53l0x_dev, &refSpadCount, &isApertureSpads );
    if ( status ) {
        printf( "VL53L0X_PerformRefSpadManagement failed\n" );
    }

    status = VL53L0X_SetDeviceMode( &vl53l0x_dev, VL53L0X_DEVICEMODE_SINGLE_RANGING );  // Setup in single ranging mode
    if ( status ) {
        printf( "VL53L0X_SetDeviceMode failed\n" );
    }

    status = VL53L0X_SetLimitCheckEnable( &vl53l0x_dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, 1 );  // Enable Sigma limit
    if ( status ) {
        printf( "VL53L0X_SetLimitCheckEnable failed\n" );
    }

    status = VL53L0X_SetLimitCheckEnable( &vl53l0x_dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, 1 );  // Enable Signa limit
    if ( status ) {
        printf( "VL53L0X_SetLimitCheckEnable failed\n" );
    }
    /* Ranging configuration */
    switch ( rangingConfig ) {
    case LONG_RANGE:
        signalLimit           = ( FixPoint1616_t )( 0.1 * 65536 );
        sigmaLimit            = ( FixPoint1616_t )( 60 * 65536 );
        timingBudget          = 33000;
        preRangeVcselPeriod   = 18;
        finalRangeVcselPeriod = 14;
        break;
    case HIGH_ACCURACY:
        signalLimit           = ( FixPoint1616_t )( 0.25 * 65536 );
        sigmaLimit            = ( FixPoint1616_t )( 18 * 65536 );
        timingBudget          = 200000;
        preRangeVcselPeriod   = 14;
        finalRangeVcselPeriod = 10;
        break;
    case HIGH_SPEED:
        signalLimit           = ( FixPoint1616_t )( 0.25 * 65536 );
        sigmaLimit            = ( FixPoint1616_t )( 32 * 65536 );
        timingBudget          = 20000;
        preRangeVcselPeriod   = 14;
        finalRangeVcselPeriod = 10;
        break;
    default:
        printf( "Not Supported" );
    }

    status = VL53L0X_SetLimitCheckValue( &vl53l0x_dev, VL53L0X_CHECKENABLE_SIGNAL_RATE_FINAL_RANGE, signalLimit );
    if ( status ) {
        printf( "VL53L0X_SetLimitCheckValue failed\n" );
    }

    status = VL53L0X_SetLimitCheckValue( &vl53l0x_dev, VL53L0X_CHECKENABLE_SIGMA_FINAL_RANGE, sigmaLimit );
    if ( status ) {
        printf( "VL53L0X_SetLimitCheckValue failed\n" );
    }

    status = VL53L0X_SetMeasurementTimingBudgetMicroSeconds( &vl53l0x_dev, timingBudget );
    if ( status ) {
        printf( "VL53L0X_SetMeasurementTimingBudgetMicroSeconds failed\n" );
    }

    status = VL53L0X_SetVcselPulsePeriod( &vl53l0x_dev, VL53L0X_VCSEL_PERIOD_PRE_RANGE, preRangeVcselPeriod );
    if ( status ) {
        printf( "VL53L0X_SetVcselPulsePeriod failed\n" );
    }

    status = VL53L0X_SetVcselPulsePeriod( &vl53l0x_dev, VL53L0X_VCSEL_PERIOD_FINAL_RANGE, finalRangeVcselPeriod );
    if ( status ) {
        printf( "VL53L0X_SetVcselPulsePeriod failed\n" );
    }

    status = VL53L0X_PerformRefCalibration( &vl53l0x_dev, &VhvSettings, &PhaseCal );
    if ( status ) {
        printf( "VL53L0X_PerformRefCalibration failed\n" );
    }

    vl53l0x_dev.LeakyFirst = 1;
}
void vl53l0x_mesure()
{

    int over = 0;
    int status;
    char StrDisplay[ 5 ];
    // char c;
    //  int i;
    //  int nSensorToUse;
    //  int SingleSensorNo = 0;

    /* Setup all sensors in Single Shot mode */
    SetupSingleShot( HIGH_ACCURACY );

    /* Start ranging until blue button is pressed */
    do {
        /* only one sensor */
        /* Call All-In-One blocking API function */
        status = VL53L0X_PerformSingleRangingMeasurement( &vl53l0x_dev, &RangingMeasurementData );
        if ( status == 0 ) {
            /* Push data logging to UART */
            // printf( "%d,%u,%d,%d,%d\r\n", vl53l0x_dev.Id, HAL_GetTick(), RangingMeasurementData.RangeStatus, RangingMeasurementData.RangeMilliMeter, RangingMeasurementData.SignalRateRtnMegaCps );
            Sensor_SetNewRange( &vl53l0x_dev, &RangingMeasurementData );
            /* Display distance in cm */
            if ( RangingMeasurementData.RangeStatus == 0 ) {
                sprintf( StrDisplay, "%3dc", ( int )vl53l0x_dev.LeakyRange / 10 );
            }
            else {
                sprintf( StrDisplay, "----" );
                StrDisplay[ 0 ] = vl53l0x_dev.DevLetter;
            }
        }
        else {
        }
        printf( "%s\r\n", StrDisplay );
    } while ( !over );
    /* Wait button to be un-pressed to decide if it is a short or long press */
}
float vl53l0x_get_dis()
{

    return 0;
}