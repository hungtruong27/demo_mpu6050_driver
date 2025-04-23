#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <errno.h> // Include errno header

#define DEVICE_PATH "/dev/mpu6050"
#define MPU6050_IOCTL_MAGIC 'm'
#define MPU6050_IOCTL_READ_X _IOR(MPU6050_IOCTL_MAGIC, 1, int)
#define MPU6050_IOCTL_READ_Y _IOR(MPU6050_IOCTL_MAGIC, 2, int)
#define MPU6050_IOCTL_READ_Z _IOR(MPU6050_IOCTL_MAGIC, 3, int)

int main() {
    int fd;
    int data;

    // Open the device
    fd = open(DEVICE_PATH, O_RDONLY);
    if (fd < 0) {
        perror("Failed to open the device");
        return errno;
    }

    // Read X-axis data
    if (ioctl(fd, MPU6050_IOCTL_READ_X, &data) < 0) {
        perror("Failed to read X-axis data");
        close(fd);
        return errno;
    }
    printf("X-axis: %d\n", data);

    // Read Y-axis data
    if (ioctl(fd, MPU6050_IOCTL_READ_Y, &data) < 0) {
        perror("Failed to read Y-axis data");
        close(fd);
        return errno;
    }
    printf("Y-axis: %d\n", data);

    // Read Z-axis data
    if (ioctl(fd, MPU6050_IOCTL_READ_Z, &data) < 0) {
        perror("Failed to read Z-axis data");
        close(fd);
        return errno;
    }
    printf("Z-axis: %d\n", data);

    // Close the device
    close(fd);
    return 0;
}
