# Driver cho ADS1115 trên Raspberry Pi

Dự án này cung cấp một Linux Kernel Driver cho IC ADC ADS1115 sử dụng giao tiếp I2C. Driver này cho phép người dùng đọc tín hiệu analog từ ADS1115 thông qua file device trong user space trên Raspberry Pi.

## Tính năng

- Hỗ trợ chế độ đọc single-shot từ 4 kênh analog
- Giao tiếp I2C chuẩn, tự động probe thiết bị
- Tạo file device `/dev/ads1115` để user-space truy cập
- Có thể mở rộng hỗ trợ nhiều địa chỉ I2C (ADDR pin)

## Thông tin hệ thống

- **IC**: ADS1115 (16-bit ADC, 4 channel, I2C)
- **Platform**: Raspberry Pi 3,4,5
- **Kernel**: 6.x trở lên
- **Ngôn ngữ**: C (Linux Kernel Module)
