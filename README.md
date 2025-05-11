# Driver ADS1115 cho Raspberry Pi 3

Đây là driver ADS1115 được viết bằng ngôn ngữ C, dùng để giao tiếp với module **ADS1115** thông qua giao thức **I2C**, hoạt động trên nền **Raspberry Pi 3**. Driver hỗ trợ đọc ADC với độ phân giải 16-bit, có thể cấu hình hệ số khuếch đại (gain), tốc độ lấy mẫu, và kênh đọc.

**Repo GitHub:** https://github.com/Tin6818061/ADS1115_DRIVER.git

---

## Tính năng

- Đọc giá trị ADC từ 4 kênh đơn (AIN0-AIN3)
- Hỗ trợ chế độ đọc đơn (single-shot)
- Cấu hình được gain, tốc độ lấy mẫu
- Giao tiếp I2C, tương thích với Raspberry Pi 3
- Đã kiểm tra thực tế với **biến trở 10k** gắn vào kênh AIN0

---

## Kết nối phần cứng

| Raspberry Pi 3 | ADS1115    |
|----------------|------------|
| GPIO2 (SDA)    | SDA        |
| GPIO3 (SCL)    | SCL        |
| 3.3V           | VDD        |
| GND            | GND        |
| Biến trở 10k   | Giữa nối GND và VDD, chân giữa nối AIN0 |

> Lưu ý: cần thêm điện trở pull-up 4.7kΩ nếu cần ổn định đường SDA/SCL

---

## Cấu trúc thư mục

---

## Bắt đầu sử dụng

### 1. Cài đặt I2C trên Raspberry Pi
```bash
sudo raspi-config
# → Interface Options → I2C → Enable
sudo apt-get update
sudo apt-get install i2c-tools libi2c-dev
### Kiểm tra kết nối
i2cdetect -y 1
# ADS1115 thường có địa chỉ là 0x48
### Biên dịch chương trình
cd ADS1115_DRIVER
gcc examples/main.c src/ads1115.c -Iinclude -o adc_test
### Chạy chương trình
sudo ./adc_test


