#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h> // Include errno header

// IOCTL commands for ADS1115
#define ADS1115_IOCTL_MAGIC 'a'
#define ADS1115_IOCTL_CONFIG _IOW(ADS1115_IOCTL_MAGIC, 1, int)
#define ADS1115_IOCTL_SET_LOTHRESH _IOW(ADS1115_IOCTL_MAGIC, 2, int)
#define ADS1115_IOCTL_SET_HITHRESH _IOW(ADS1115_IOCTL_MAGIC, 3, int)
#define ADS1115_IOCTL_READ_ADC _IOR(ADS1115_IOCTL_MAGIC, 4, int)
#define ADS1115_IOCTL_SET_CHANNEL _IOW(ADS1115_IOCTL_MAGIC, 5, int)

// IOCTL commands for GPIO
#define GPIO_MAGIC 'G'
#define GPIO_SET_PIN _IOW(GPIO_MAGIC, 0, int)
#define GPIO_GET_PIN _IOR(GPIO_MAGIC, 1, int)
#define GPIO_RESET_PIN _IOW(GPIO_MAGIC, 2, int)

// Differential input modes
#define ADS1115_MUX_0_1    0  // AIN0 - AIN1
#define ADS1115_MUX_0_3    1  // AIN0 - AIN3
#define ADS1115_MUX_1_3    2  // AIN1 - AIN3
#define ADS1115_MUX_2_3    3  // AIN2 - AIN3

// Single-ended input modes (AINx vs GND)
#define ADS1115_MUX_0_GND  4  // AIN0 - GND
#define ADS1115_MUX_1_GND  5  // AIN1 - GND
#define ADS1115_MUX_2_GND  6  // AIN2 - GND
#define ADS1115_MUX_3_GND  7  // AIN3 - GND

static inline uint16_t adsConfig(uint8_t mux, uint8_t pga, uint8_t mode,uint8_t dr,
                                 uint8_t c_mode,uint8_t pol, uint8_t lat, uint8_t que)
{
return (1 << 15) |                  //   [15] OS
        ((mux & 0x07) << 12)   |    //[14:12] MUX
        ((pga & 0x07) << 9)    |    //  11:9] PGA
        ((mode & 0x01) << 8)   |    //    [8] MODE
        ((dr & 0x07) << 5)     |    //  [7:5] DR
        ((c_mode & 0x01) << 4) |    //    [4] COMP_MODE
        ((pol & 0x01) << 3)    |    //    [3] COMP_POL
        ((lat & 0x01) << 2)    |    //    [2] COMP_LAT
        (que & 0x03);               //  [1:0] COMP_QUE
}
int main(void) {
    int ads_fd;
    int fd;
    int adc_val;
    int alert_pin = 17; // Example GPIO pin number
    int value;
    float volt;
    int16_t channel = ADS1115_MUX_0_GND;
    // Open ADS1115 
    ads_fd = open("/dev/ads1115", O_RDWR);
    if (ads_fd < 0) {
        perror("Failed to open the device");
        return errno;
    }

    // Open the device file
    fd = open("/dev/gpio_ioctl_device", O_RDWR);
    if (fd < 0) {
        perror("Failed to open the device...");
        return errno;
    }

    // Config ADS1115
    uint16_t config = adsConfig(
        0x04, // MUX: AIN0 vs GND
        0x01, // PGA: ±4.096V
        0x01, // MODE: Single-shot
        0x01, // DR: 16 SPS
        0x01, // COMP_MODE: window
        0x01, // COMP_POL: active high
        0x00, // COMP_LAT: non-latching
        0x00  // COMP_QUE: enable comparator
    );
    if (ioctl(ads_fd, ADS1115_IOCTL_CONFIG, &config) < 0) {
        perror("Failed to config ADS1115");
        close(ads_fd);
        return errno;
    }
    usleep(62500);
    while(1){
        // Set channel
        if (ioctl(ads_fd, ADS1115_IOCTL_SET_CHANNEL, &channel) < 0) {
            perror("Failed to read ADC data");
            close(ads_fd);
            return errno;
        }
        usleep(62500);  // Data rate: 16sps

        // Read ADC data
        if (ioctl(ads_fd, ADS1115_IOCTL_READ_ADC, &adc_val) < 0) {
            perror("Failed to read ADC data");
            close(ads_fd);
            return errno;
        }
        volt = adc_val*4.096f/32767;
        printf("ADC value = %f\n", volt);
    
    }
    close(ads_fd);
    close(fd);
    return 0;
}
//sudo dtc -@ -I dts -O dtb -o ads1115-overlay.dtbo ads1115-overlay.dts
