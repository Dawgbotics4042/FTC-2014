#ifndef __HTPB_H__
#define __HTPB_H__

#pragma systemFile

#ifndef __COMMON_H__
#include "common.h"
#endif

#define HTPB_I2C_ADDR 0x10      /*!< Protoboard I2C device address **may or may not be 0x10** */
#define HTPB_OFFSET   0x42
#define HTPB_A0_U     0x00      /*!< Address of upper bits of first ADC, bits 9-2 */
#define HTPB_A0_L     0x01      /*!< Address of lower bits of first ADC, bits 1-0 */
#define HTPB_DIGIN    0x0A      /*!< Address of digital inputs */
#define HTPB_DIGOUT   0x0B      /*!< Address of digital outputs */
#define HTPB_DIGCTRL  0x0C      /*!< Controls direction of digital ports */
#define HTPB_SRATE    0x0D      /*!< Controls sample rate, default set to 10ms */

tByteArray HTPB_I2CRequest;    /*!< Array to hold I2C command data */
tByteArray HTPB_I2CReply;      /*!< Array to hold I2C reply data */

bool HTPBsetupIO(tSensors link, ubyte mask);
int HTPBreadADC(tSensors link, byte channel, byte width);
#endif


/**
 * Configure the ports for input or output according to the mask.
 * @param link the HTPB port number
 * @param mask the specified digital ports, 0 = input, 1 = output
 * @return true if no error occured, false if it did
 */
bool HTPBsetupIO(tSensors link, ubyte mask) {
    memset(HTPB_I2CRequest, 0, sizeof(tByteArray));

    HTPB_I2CRequest[0] = 3;                           // Message size
    HTPB_I2CRequest[1] = HTPB_I2C_ADDR;               // I2C Address
    HTPB_I2CRequest[2] = HTPB_OFFSET + HTPB_DIGCTRL;  // Start digital input/output control address
    HTPB_I2CRequest[3] = mask;                        // The specified digital ports

    return writeI2C(link, HTPB_I2CRequest);
}

/**
 * Read the values of the digital inputs as specified by the mask.
 * @param link the HTPB port number
 * @param mask the specified digital ports
 */
ubyte HTPBreadIO(tSensors link, ubyte mask) {
    memset(HTPB_I2CRequest, 0, sizeof(tByteArray));

    HTPB_I2CRequest[0] = 2;                         // Message size
    HTPB_I2CRequest[1] = HTPB_I2C_ADDR;             // I2C Address
    HTPB_I2CRequest[2] = HTPB_OFFSET + HTPB_DIGIN;  // Start digital output read address

    writeI2C(link, HTPB_I2CRequest, HTPB_I2CReply, 1);

    return HTPB_I2CReply[0] & mask;
}

/**
 * Read the value of the specified analogue channel.
 * @param link the HTPB port number
 * @param channel the specified ADC channel
 * @param width the bit width of the result, can be either 8 or 10
 * @return the value of the ADC channel, or -1 if an error occurred
 */
int HTPBreadADC(tSensors link, byte channel, byte width) {
    memset(HTPB_I2CRequest, 0, sizeof(tByteArray));

    int _adcVal = 0;
    HTPB_I2CRequest[0] = 2;                                       // Message size
    HTPB_I2CRequest[1] = HTPB_I2C_ADDR;                           // I2C Address
    HTPB_I2CRequest[2] = HTPB_OFFSET + HTPB_A0_U + (channel * 2); // Start digital output read address
                                                                      // with channel offset
    if (!writeI2C(link, HTPB_I2CRequest, HTPB_I2CReply, 2))
        return -1;

    // Convert the bytes into an int
    // 1st byte contains bits 9-2 of the channel's value
    // 2nd byte contains bits 1-0 of the channel's value
    // We'll need to shift the 1st byte left by 2 and or 2nd byte onto it.
    // If 8 bits is all we want, we just return the first byte and be done with it.
    if (width == 8)
        _adcVal = HTPB_I2CReply[0];
    else
        _adcVal = (HTPB_I2CReply[0] * 4) + HTPB_I2CReply[1];

    return _adcVal;
}
