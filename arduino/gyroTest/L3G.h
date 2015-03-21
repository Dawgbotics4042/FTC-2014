#ifndef L3G_h
#define L3G_h
/**
 * @addtogroup arduino
 * @{
 * @addtogroup L3G
 * \brief The interface to the gyroscope.
 *
 * Provides methods to work with pololu v3 gyroscope.
 *
 * The Arduino two-wire interface uses a 7-bit number for the address,
 * and sets the last bit correctly based on reads and writes.
 * @{
 */

#include <Arduino.h> // for byte data type
#include <Wire.h>

// Defines ////////////////////////////////////////////////////////////////

// device types

#define L3G_DEVICE_AUTO 0
#define L3G4200D_DEVICE 1
#define L3GD20_DEVICE   2


// SA0 states

#define L3G_SA0_LOW  0
#define L3G_SA0_HIGH 1
#define L3G_SA0_AUTO 2

// register addresses

#define L3G_WHO_AM_I      0x0F

#define L3G_CTRL_REG1     0x20
#define L3G_CTRL_REG2     0x21
#define L3G_CTRL_REG3     0x22
#define L3G_CTRL_REG4     0x23
#define L3G_CTRL_REG5     0x24
#define L3G_REFERENCE     0x25
#define L3G_OUT_TEMP      0x26
#define L3G_STATUS_REG    0x27

#define L3G_OUT_X_L       0x28
#define L3G_OUT_X_H       0x29
#define L3G_OUT_Y_L       0x2A
#define L3G_OUT_Y_H       0x2B
#define L3G_OUT_Z_L       0x2C
#define L3G_OUT_Z_H       0x2D

#define L3G_FIFO_CTRL_REG 0x2E
#define L3G_FIFO_SRC_REG  0x2F

#define L3G_INT1_CFG      0x30
#define L3G_INT1_SRC      0x31
#define L3G_INT1_THS_XH   0x32
#define L3G_INT1_THS_XL   0x33
#define L3G_INT1_THS_YH   0x34
#define L3G_INT1_THS_YL   0x35
#define L3G_INT1_THS_ZH   0x36
#define L3G_INT1_THS_ZL   0x37
#define L3G_INT1_DURATION 0x38
#define L3G_LOW_ODR       0x39


// The Arduino two-wire interface uses a 7-bit number for the address,
// and sets the last bit correctly based on reads and writes
#define L3G4200D_ADDRESS_SA0_LOW  (0xD0 >> 1) /*!< Address for a device on low power.
                                                Not sure what device this is for. */
#define L3G4200D_ADDRESS_SA0_HIGH (0xD2 >> 1) /*!< Address for a device on high power.
                                                Not sure what device this is for. */
#define L3GD20_ADDRESS_SA0_LOW    (0xD4 >> 1) /*!< Address for our gyro on low power.
                                                Use this if SA0 pin on gyro is driven low.
                                                This is not the default. */
#define L3GD20_ADDRESS_SA0_HIGH   (0xD6 >> 1) /*!< Address for our gyro with default setup.
                                                Use this if SA0 pin is left untouched. */
class L3G
{
    public:
        typedef struct vector
        {
            float x, y, z;
        } vector;

        vector g; // gyro angular velocity readings

        /**
         * Finds the correct slave address for the gryo.
         *
         * @param device The type of device. Ours is L3GD20_DEVICE.
         * Not sure what the other device is or what it is for.
         * @param sa0 Says if the SA0 pin is being driven low or left
         * at its default. Default is L3G_SA0_HIGH. Use L3G_SA0_LOW if SA0 pin
         * is driven low.
         */
        bool init(byte device = L3GD20_DEVICE, byte sa0 = L3G_SA0_HIGH);

        /**
         * Turns on the gyro and set some basic settings.
         *
         * This includes which axes to enable.
         *
         * TODO: complete documentation on addresses and settings
         */
        void enableDefault(void);

        /**
         * Writes to a register on the gyro.
         *
         * @param reg The register to write to.
         * @param value The value to write.
         */
        void writeReg(byte reg, byte value);

        /**
         * Reads a register on the gryo.
         *
         * @param reg The register to read.
         */
        byte readReg(byte reg);

        /**
         * Reads all 3 gryo outputs and stores them in a vector g.
         */
        void read(void);

        // vector functions
        /**
         * Computes a cross product.
         */
        static void vector_cross(const vector *a, const vector *b, vector *out);

        /**
         * Computes a dot product.
         */
        static float vector_dot(const vector *a,const vector *b);

        /**
         * Normalizes a vector using the dot product.
         * Not exactly sure what purpose this has.
         */
        static void vector_normalize(vector *a);

    private:
        byte _device; // chip type (4200D or D20)
        byte address;

        /**
         * Try to automatically detect the address of the gyro.
         * Connects to a bunch of addresses and sees what works.
         */
        bool autoDetectAddress(void);
};

/**
 * @}
 * @}
 */
#endif
