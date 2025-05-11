# Kiến trúc Driver ADS1115

## Thành phần

- `ads1115_driver.c`: driver chính
- `ads1115.h`: định nghĩa thanh ghi và địa chỉ
- `Makefile`: biên dịch module

## Luồng hoạt động

```
+-------------+         +----------------+        +-----------------+
| User space  | ----->  | File Operations| --->   | I2C Transactions|
+-------------+         +----------------+        +-----------------+
       cat /dev/ads1115          read()                  i2c_smbus_read_word
```

### probe()

- Được gọi khi device I2C được detect
- Gán i2c_client
- Đăng ký `miscdevice` hoặc `cdev`

### read()

- Gửi lệnh start conversion
- Delay ngắn (tuỳ tốc độ mẫu)
- Đọc từ register conversion và trả về

### release()

- Giải phóng resource nếu cần

## Các API kernel sử dụng

- `i2c_smbus_read_word_data()`
- `i2c_smbus_write_word_data()`
- `device_create()`, `class_create()`
