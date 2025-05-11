# Khắc phục lỗi thường gặp

## Không có `/dev/ads1115`
- Kiểm tra `dmesg`: xem probe có lỗi không
- Dây nối I2C đúng chưa?
- Đã bật I2C bằng `sudo raspi-config` chưa?
- Kiểm tra địa chỉ với `i2cdetect -y 1`

## Đọc dữ liệu trả về -1 hoặc lỗi
- Kiểm tra log kernel (`dmesg`)
- Kiểm tra dòng I2C đang sử dụng có đúng `/dev/i2c-1`
- Đảm bảo IC được cấp nguồn đúng

## `insmod` báo lỗi
- Có thiếu `MODULE_LICENSE("GPL");`?
- Phiên bản kernel không tương thích?
- Đã build với `make` chưa?
