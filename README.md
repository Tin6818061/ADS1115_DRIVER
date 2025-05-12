# Linux Kernel Driver cho ADS1115 trên Raspberry Pi

## Giới thiệu

Dự án này là một Linux Kernel Driver tùy chỉnh cho IC chuyển đổi tương tự–số (ADC) ADS1115, giao tiếp qua I2C. Driver giúp truy cập dữ liệu ADC từ user space thông qua thiết bị `/dev/ads1115`.

Phù hợp cho các dự án nhúng, học tập hoặc ứng dụng trong hệ thống đo lường sử dụng Raspberry Pi.

---

## Thư mục dự án

```
ads1115_driver/
│
├── SRC                    # File Source code driver
├── README.md   
├── Makefile               # Tập tin biên dịch kernel module
│
└── doc/                   # Tài liệu chi tiết
│    ├── overview.md
│    ├── hardware.md
│    ├── register_map.md
│    ├── driver_architecture.md
│    ├── usage.md
│    ├── troubleshooting.md
│    └── references.md
└── test/ 
     ├── test.c            # File test trên user space 

```

---

## Yêu cầu hệ thống

- Raspberry Pi 3/4/5 với Raspbian OS
- Kernel 6.x hoặc tương thích
- `i2c-dev`, `i2c-tools` đã cài đặt
- Giao tiếp I2C đã bật (`sudo raspi-config`)
- Toolchain để build kernel module (`make`, `gcc`, `linux-headers`)

---

## Kết nối phần cứng

| ADS1115 Pin | Raspberry Pi GPIO |
|-------------|-------------------|
| VDD         | 3.3V              |
| GND         | GND               |
| SDA         | GPIO 2 (SDA)      |
| SCL         | GPIO 3 (SCL)      |
| ADDR        | GND (→ I2C addr 0x48) |

---

##  Cách sử dụng

### 1. Build module

```bash
make
```

### 2. Nạp module vào kernel

```bash
sudo insmod ads1115_driver.ko
```

### 3. Kiểm tra thiết bị đã tạo

```bash
ls -l /dev/ads1115
```

### 4. Đọc dữ liệu từ user space

```bash
cat /dev/ads1115
# Kết quả: số nguyên đại diện giá trị ADC (signed 16-bit)
```

### 5. Gỡ module

```bash
sudo rmmod ads1115_driver
```

---

## Hoạt động bên trong driver

- Gọi `i2c_smbus_write_word_data()` để cấu hình và bắt đầu chuyển đổi
- Đợi một thời gian ngắn theo tốc độ lấy mẫu (DR)
- Đọc kết quả bằng `i2c_smbus_read_word_data()`
- Trả về kết quả cho user thông qua file `/dev/ads1115`

Chi tiết xem thêm trong `doc/driver_architecture.md`

---

## Gỡ lỗi và xử lý sự cố

- `i2cdetect -y 1` không thấy `0x48` → kiểm tra dây hoặc bật I2C
- Không tạo `/dev/ads1115` → xem `dmesg`, kiểm tra probe
- `cat /dev/ads1115` báo lỗi → kiểm tra `dmesg` xem log kernel

Xem thêm `doc/troubleshooting.md`

---

## Tài liệu tham khảo

- [ADS1115 Datasheet](https://www.ti.com/lit/ds/symlink/ads1115.pdf)
- [Linux I2C Subsystem](https://www.kernel.org/doc/html/latest/i2c/)
- [Bootlin Elixir I2C Drivers](https://elixir.bootlin.com/)

