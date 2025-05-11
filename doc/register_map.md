# Bản đồ thanh ghi ADS1115

## 1. Conversion Register (0x00)
- 16-bit signed, chứa kết quả ADC sau khi chuyển đổi.
- Đọc khi bit OS (bit 15 của Config Register) đã = 1.

## 2. Config Register (0x01)

### Định dạng 16-bit:

```
Bit: 15      14-12    11-9   8    7-5    4      3      2     1-0
     OS      MUX      PGA    MODE DR     COMP_MODE POL   LAT   QUE
```

| Field     | Bits   | Mô tả |
|-----------|--------|-------|
| OS        | 15     | Start single conversion (1) |
| MUX       | 14-12  | Kênh đầu vào: `100` = AIN0-GND |
| PGA       | 11-9   | Gain: `010` = ±2.048V |
| MODE      | 8      | `1` = Single-shot |
| DR        | 7-5    | Tốc độ mẫu: `100` = 128 SPS |
| COMP_MODE | 4      | Comparator truyền thống |
| COMP_POL  | 3      | `0` = Active low |
| COMP_LAT  | 2      | `0` = Non-latching |
| COMP_QUE  | 1-0    | `11` = Disable comparator |

## 3. Lo_thresh (0x02), Hi_thresh (0x03)
- Dùng nếu bật comparator (ít khi dùng trong driver cơ bản)
