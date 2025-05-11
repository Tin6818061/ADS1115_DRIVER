# Kết nối phần cứng

## Sơ đồ nối dây

| ADS1115 | Raspberry Pi 5 |
|---------|----------------|
| VDD     | 3.3V           |
| GND     | GND            |
| SDA     | GPIO 2 (SDA)   |
| SCL     | GPIO 3 (SCL)   |
| ADDR    | GND (địa chỉ 0x48) |

> Lưu ý: Raspberry Pi cần bật I2C trong `raspi-config`.

## Kiểm tra kết nối

```bash
sudo apt install -y i2c-tools
i2cdetect -y 1
```

Kết quả mong đợi: xuất hiện địa chỉ `0x48` trong bảng.

## Sơ đồ mạch minh hoạ

*(Chèn ảnh sơ đồ hoặc dùng phần mềm Fritzing vẽ)*
