#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/types.h>
#include <linux/delay.h>

#define DRIVER_NAME "ads1115_driver"
#define CLASS_NAME "ads1115"
#define DEVICE_NAME "ads1115"

// IOCTL commands
#define ADS1115_IOCTL_MAGIC 'a'

#define ADS1115_IOCTL_CONFIG _IOW(ADS1115_IOCTL_MAGIC, 1, int)
#define ADS1115_IOCTL_SET_LOTHRESH _IOW(ADS1115_IOCTL_MAGIC, 2, int)
#define ADS1115_IOCTL_SET_HITHRESH _IOW(ADS1115_IOCTL_MAGIC, 3, int)
#define ADS1115_IOCTL_READ_ADC _IOR(ADS1115_IOCTL_MAGIC, 4, int)
#define ADS1115_IOCTL_SET_CHANNEL _IOW(ADS1115_IOCTL_MAGIC, 5, int)

#define ADS1115_ADR 0x48    //ADDR nối với Ground
#define ADS_REG_CONV 0x00
#define ADS_REG_CONFIG 0x01
#define ADS_REG_LO_THRESH 0x02
#define ADS_REG_HI_THRESH 0x03

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

static struct i2c_client *ads1115_client;
static struct class* ads1115_class = NULL;
static struct device* ads1115_device = NULL;
static int major_number;

static int ads1115_set_config(struct i2c_client *client,u16 config)
{
    int ret;
    ret = i2c_smbus_write_word_data(client, ADS_REG_CONFIG, cpu_to_be16(config));
    if (ret < 0) {
        printk("ADS1115: Failed to set config: %d\n", ret);
        return ret;
    }
    return 0;
}
static int ads1115_set_loThresh(struct i2c_client *client,u16 loThresh){
    int ret;
    ret = i2c_smbus_write_word_data(client,ADS_REG_LO_THRESH,cpu_to_be16(loThresh));
    if (ret < 0) {
        printk("ADS1115: Failed to set low thresh: %d\n", ret);
        return ret;
    }
    ndelay(30000);
    ret = i2c_smbus_read_word_data(client, ADS_REG_LO_THRESH);
    if (ret < 0) {
        printk("ADS1115: Failed to read ADC value: %d\n", ret);
        return ret;
    }
    printk("ret: %d\n", ret);
    return 0;
}
static int ads1115_set_hiThresh(struct i2c_client *client,u16 hiThresh){
    int ret;
    ret = i2c_smbus_write_word_data(client,ADS_REG_HI_THRESH,cpu_to_be16(hiThresh));
    if (ret < 0) {
        printk("ADS1115: Failed to set hi thresh: %d\n", ret);
        return ret;
    }

    return 0;
}
static int ads1115_read_adc(struct i2c_client *client)
{
    int ret;

    ret = i2c_smbus_read_word_data(client, ADS_REG_CONV);
    if (ret < 0) {
        printk("ADS1115: Failed to read ADC value: %d\n", ret);
        return ret;
    }
    return be16_to_cpu(ret);
}
int ads1115_set_channel(struct i2c_client *client, u16 channel)
{
    int ret;
    u16 config;
    // đọc lại config
    printk("doc config truoc\n");
    ret = i2c_smbus_read_word_data(client, ADS_REG_CONFIG);
    if (ret < 0)
        return ret;

    config = be16_to_cpu((u16)ret);

    // Điều chỉnh MUX
    config &= ~(0x07 << 12);
    config |= (channel & 0x07) << 12;

    // Bắt đầu chuyển đổi
    config |= (1 << 15); 
    printk("set channel: %d\n", config);
    return i2c_smbus_write_word_data(client, ADS_REG_CONFIG, cpu_to_be16(config));
}
static long ads1115_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int data;
    switch (cmd) {
        case ADS1115_IOCTL_CONFIG:
        if (copy_from_user(&data, (int __user *)arg, sizeof(data))) {
            return -EFAULT;
        }
        return ads1115_set_config(ads1115_client,(u16)data);
        break;

        case ADS1115_IOCTL_SET_LOTHRESH:
        if (copy_from_user(&data, (int __user *)arg, sizeof(data))) {
            return -EFAULT;
        }
        return ads1115_set_loThresh(ads1115_client,(u16)data);
        break;

        case ADS1115_IOCTL_SET_HITHRESH:
        if (copy_from_user(&data, (int __user *)arg, sizeof(data))) {
            return -EFAULT;
        }
        return ads1115_set_hiThresh(ads1115_client,(u16)data);
        break;

        case ADS1115_IOCTL_SET_CHANNEL:
        printk("dang set\n");
        if (copy_from_user(&data, (int __user *)arg, sizeof(data))) {
            return -EFAULT;
        }
        return ads1115_set_channel(ads1115_client,(u16)data);
        break;

        case ADS1115_IOCTL_READ_ADC:
            data = ads1115_read_adc(ads1115_client);
            if (copy_to_user((int __user *)arg, &data, sizeof(data)))
                return -EFAULT;
        break;
        default:
            return -EINVAL;
    }
    return 0;
}

static int ads1115_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "ads1115 device opened\n");
    return 0;
}

static int ads1115_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "ads1115 device closed\n");
    return 0;
}

static struct file_operations fops = {
    .open = ads1115_open,
    .unlocked_ioctl = ads1115_ioctl,
    .release = ads1115_release,
};

static int ads1115_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
    ads1115_client = client;

    // Create a char device
    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ERR "Failed to register a major number\n");
        return major_number;
    }

    ads1115_class = class_create(THIS_MODULE, CLASS_NAME);
    if (IS_ERR(ads1115_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ERR "Failed to register device class\n");
        return PTR_ERR(ads1115_class);
    }

    ads1115_device = device_create(ads1115_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(ads1115_device)) {
        class_destroy(ads1115_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ERR "Failed to create the device\n");
        return PTR_ERR(ads1115_device);
    }

    printk(KERN_INFO "ads1115 driver installed\n");
    return 0;
}

static int ads1115_remove(struct i2c_client *client)
{
    device_destroy(ads1115_class, MKDEV(major_number, 0));
    class_unregister(ads1115_class);
    class_destroy(ads1115_class);
    unregister_chrdev(major_number, DEVICE_NAME);

    printk(KERN_INFO "ads1115 driver removed\n");
    return 0;
}

static const struct of_device_id ads1115_of_match[] = {
    { .compatible = "TI,ads1115", },
    { },
};
MODULE_DEVICE_TABLE(of, ads1115_of_match);

static struct i2c_driver ads1115_driver = {
    .driver = {
        .name   = DRIVER_NAME,
        .owner  = THIS_MODULE,
        .of_match_table = of_match_ptr(ads1115_of_match),
    },
    .probe      = ads1115_probe,
    .remove     = ads1115_remove,
};

static int __init ads1115_init(void)
{
    printk(KERN_INFO "Initializing ads1115 driver\n");
    return i2c_add_driver(&ads1115_driver);
}

static void __exit ads1115_exit(void)
{
    printk(KERN_INFO "Exiting ads1115 driver\n");
    i2c_del_driver(&ads1115_driver);
}

module_init(ads1115_init);
module_exit(ads1115_exit);

MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("ads1115 I2C Client Driver with IOCTL Interface");
MODULE_LICENSE("GPL");
