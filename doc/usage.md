# Hướng dẫn sử dụng Driver ADS1115

## 1. Build driver

```bash
make
```

## 2. Nạp driver

```bash
sudo insmod ads1115_driver.ko
dmesg | tail
```

## 3. Kiểm tra device node

```bash
ls -l /dev/ads1115
```

## 4. Đọc dữ liệu ADC

```bash
cat /dev/ads1115
```

## 5. Gỡ driver

```bash
sudo rmmod ads1115_driver
```
