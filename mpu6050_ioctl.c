#include <linux/init.h>
#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DRIVER_NAME "mpu6050_driver"
#define CLASS_NAME "mpu6050"
#define DEVICE_NAME "mpu6050"

#define MPU6050_REG_ACCEL_XOUT_H 0x3B
#define MPU6050_REG_PWR_MGMT_1 0x6B

// IOCTL commands
#define MPU6050_IOCTL_MAGIC 'm'
#define MPU6050_IOCTL_READ_X _IOR(MPU6050_IOCTL_MAGIC, 1, int)
#define MPU6050_IOCTL_READ_Y _IOR(MPU6050_IOCTL_MAGIC, 2, int)
#define MPU6050_IOCTL_READ_Z _IOR(MPU6050_IOCTL_MAGIC, 3, int)

static struct i2c_client *mpu6050_client;
static struct class* mpu6050_class = NULL;
static struct device* mpu6050_device = NULL;
static int major_number;

static int mpu6050_read_axis(struct i2c_client *client, int axis)
{
    u8 reg = MPU6050_REG_ACCEL_XOUT_H;
    u8 buf[6];
    s16 accel_data[3];
    struct i2c_msg msgs[2];

    msgs[0].addr  = client->addr;
    msgs[0].flags = 0; // Write
    msgs[0].len   = 1;
    msgs[0].buf   = &reg;

    msgs[1].addr  = client->addr;
    msgs[1].flags = I2C_M_RD;
    msgs[1].len   = 6;
    msgs[1].buf   = buf;

    if (i2c_transfer(client->adapter, msgs, 2) < 0) {
        printk(KERN_ERR "Failed to read accelerometer data using i2c_transfer\n");
        return -EIO;
    }

    accel_data[0] = (buf[0] << 8) | buf[1]; // X
    accel_data[1] = (buf[2] << 8) | buf[3]; // Y
    accel_data[2] = (buf[4] << 8) | buf[5]; // Z

    return accel_data[axis];
}

static long mpu6050_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
    int data;

    switch (cmd) {
        case MPU6050_IOCTL_READ_X:
            data = mpu6050_read_axis(mpu6050_client, 0);
            break;
        case MPU6050_IOCTL_READ_Y:
            data = mpu6050_read_axis(mpu6050_client, 1);
            break;
        case MPU6050_IOCTL_READ_Z:
            data = mpu6050_read_axis(mpu6050_client, 2);
            break;
        default:
            return -EINVAL;
    }

    if (copy_to_user((int __user *)arg, &data, sizeof(data))) {
        return -EFAULT;
    }

    return 0;
}

static int mpu6050_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "MPU6050 device opened\n");
    return 0;
}

static int mpu6050_release(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "MPU6050 device closed\n");
    return 0;
}

static struct file_operations fops = {
    .open = mpu6050_open,
    .unlocked_ioctl = mpu6050_ioctl,
    .release = mpu6050_release,
};

static int mpu6050_probe(struct i2c_client *client)
{
    int mpu6050_wakeup;
    u8 config_data[2] = { MPU6050_REG_PWR_MGMT_1, 0x00 };

    mpu6050_client = client;

    // Đánh thức MPU6050 khỏi chế độ ngủ
    mpu6050_wakeup = i2c_master_send(client, config_data, 2);
    if (mpu6050_wakeup < 0) {
        printk(KERN_ERR "Failed to wake up MPU6050\n");
        return mpu6050_wakeup;
    }

    major_number = register_chrdev(0, DEVICE_NAME, &fops);
    if (major_number < 0) {
        printk(KERN_ERR "Failed to register a major number\n");
        return major_number;
    }

    mpu6050_class = class_create(CLASS_NAME);
    if (IS_ERR(mpu6050_class)) {
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ERR "Failed to register device class\n");
        return PTR_ERR(mpu6050_class);
    }

    mpu6050_device = device_create(mpu6050_class, NULL, MKDEV(major_number, 0), NULL, DEVICE_NAME);
    if (IS_ERR(mpu6050_device)) {
        class_destroy(mpu6050_class);
        unregister_chrdev(major_number, DEVICE_NAME);
        printk(KERN_ERR "Failed to create the device\n");
        return PTR_ERR(mpu6050_device);
    }

    printk(KERN_INFO "MPU6050 driver installed\n");
    return 0;
}

static void mpu6050_remove(struct i2c_client *client)
{
    device_destroy(mpu6050_class, MKDEV(major_number, 0));
    class_unregister(mpu6050_class);
    class_destroy(mpu6050_class);
    unregister_chrdev(major_number, DEVICE_NAME);

    printk(KERN_INFO "MPU6050 driver removed\n");
}

static const struct of_device_id mpu6050_of_match[] = {
    { .compatible = "invensense,mpu6050", },
    { },
};
MODULE_DEVICE_TABLE(of, mpu6050_of_match);

static const struct i2c_device_id mpu6050_id[] = {
    { "mpu6050", 0 },
    { },
};
MODULE_DEVICE_TABLE(i2c, mpu6050_id);

static struct i2c_driver mpu6050_driver = {
    .driver = {
        .name   = DRIVER_NAME,
        .owner  = THIS_MODULE,
        .of_match_table = of_match_ptr(mpu6050_of_match),
    },
    .probe      = mpu6050_probe,  // Dùng probe
    .remove     = mpu6050_remove,
    .id_table   = mpu6050_id,     // Đảm bảo khai báo id_table
};

// Cần đăng ký driver
module_i2c_driver(mpu6050_driver);

MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("MPU6050 I2C Client Driver with IOCTL Interface");
MODULE_LICENSE("GPL");
