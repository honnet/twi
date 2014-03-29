#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "nrf_gpio.h"
#include "board.h"
#include "nrf_delay.h"
#include "MPU9150Lib.h"
#include "tools.h"


MPU9150Lib MPU;

//  MPU_UPDATE_RATE defines the rate (in Hz) at which the MPU updates the sensor data and DMP output
#define MPU_UPDATE_RATE  (20)

//  MAG_UPDATE_RATE defines the rate (in Hz) at which the MPU updates the magnetometer data
//  MAG_UPDATE_RATE should be less than or equal to the MPU_UPDATE_RATE
#define MAG_UPDATE_RATE  (10)

//  MPU_MAG_MIX defines the influence that the magnetometer has on the yaw output.
//  The magnetometer itself is quite noisy so some mixing with the gyro yaw can help
//  significantly. Some example values are defined below:
#define  MPU_MAG_MIX_GYRO_ONLY          0                   // just use gyro yaw
#define  MPU_MAG_MIX_MAG_ONLY           1                   // just use magnetometer and no gyro yaw
#define  MPU_MAG_MIX_GYRO_AND_MAG       10                  // a good mix value
#define  MPU_MAG_MIX_GYRO_AND_SOME_MAG  50                  // mainly gyros with a bit of mag correction

//  MPU_LPF_RATE is the low pas filter rate and can be between 5 and 188Hz
#define MPU_LPF_RATE   40

void init(void)
{
    nrf_gpio_cfg_output(LED);

    DP_cnf(RTS_PIN_NUMBER, TX_PIN_NUMBER, CTS_PIN_NUMBER, RX_PIN_NUMBER, false);
    DP_str("retval\n");
    DP_int_ln(3);

    int retval = MPU.init(MPU_UPDATE_RATE, MPU_MAG_MIX_GYRO_AND_MAG, MAG_UPDATE_RATE, MPU_LPF_RATE); // start the MPU
    DP_int_ln(retval);
}

int main(void)
{
    init();

    while(1)
    {
/*
        if (MPU.read()) {                                        // get the latest data if ready yet
//          MPU.printQuaternion(MPU.m_rawQuaternion);            // print the raw quaternion from the dmp
//          MPU.printVector(MPU.m_rawMag);                       // print the raw mag data
//          MPU.printVector(MPU.m_rawAccel);                     // print the raw accel data
//          MPU.printAngles(MPU.m_dmpEulerPose);                 // the Euler angles from the dmp quaternion
//          MPU.printVector(MPU.m_calAccel);                     // print the calibrated accel data
//          MPU.printVector(MPU.m_calMag);                       // print the calibrated mag data
            MPU.printAngles(MPU.m_fusedEulerPose);               // print the output of the data fusion

            DP_str("\n");
        }
        DP_int(heading);
        DP_str("\t");

        for (int i = 0; i < 90; i++)
        {
            if (i < heading / 4) // max = 360/4 = 90
                DP_str("+");
            else
                DP_str(" ");
        }
        DP_str("|\r\n");
*/
        nrf_gpio_pin_toggle(LED);
        nrf_delay_ms(50);
    }
}

