
# STM32F4 + DS1302 Real Time Clock Project

This project demonstrates interfacing the **DS1302 RTC (Real Time Clock)** module with an **STM32F407VG** microcontroller. The current time and date are read from the RTC module and displayed via UART (USART2), using a USB-to-TTL converter.

---

## 📦 Features

- Custom DS1302 driver (written in C)
- Read real-time **seconds, minutes, hours, date, month, day of the week, and year**
- Display formatted time and date via UART (serial terminal)
- Optionally set the initial date and time via code
- Compatible with STM32 HAL library

---

## 🔧 Hardware Used

| Component        | Model                   |
|------------------|-------------------------|
| Microcontroller  | STM32F407VG (Discovery) |
| RTC Module       | DS1302                  |
| UART Converter   | USB to TTL (3.3V logic) |
| Power Supply     | 3.3V                    |           

---

## 🧩 Pin Configuration

| DS1302 Pin | STM32F407VG Pin | Description              |
|------------|-----------------|--------------------------|
| VCC        | 3.3V            | Power                    |
| GND        | GND             | Ground                   |
| CLK        | PA5             | Serial Clock (SCLK)      |
| I/O        | PA6             | Bidirectional Data (SDA) |
| CE         | PA7             | Chip Enable (RST)        |

> You can modify the pin mappings in `main.c` by `DS1302_HandleTypeDef` if needed.

---

## 🚀 Getting Started

1. Clone the repository to your local machine.
2. Open the project in STM32CubeIDE or your preferred IDE.
3. Compile and flash the firmware to your STM32 board.
4. Connect a USB-to-TTL converter to USART2 (PA2 = TX, PA3 = RX).
5. Open a serial terminal (e.g., PuTTY, Tera Term) at **115200 baud**.

---

## 🧪 Sample Output (UART)

```
2025-06-10 02:05:27
```

> Format: `YYYY-MM-DD HH:MM:DD` (This is ISO format)

---

## 🛠️ Project Structure

```
DS1302/
  ├── Inc/
  │   └──ds1302.h
  └── Src/
      └── ds1302.c
```

---

## 🔍 Function Descriptions

### `void DS1302_Init(void)`
Initializes GPIOs for the DS1302 interface. Sets CLK, IO, and CE pins as needed.

---

### `void DS1302_WriteByte(uint8_t byte)`
Sends a byte to the DS1302 one bit at a time via bit-banging protocol.

---

### `uint8_t DS1302_ReadByte(void)`
Reads a byte from the DS1302, bit by bit, using manual GPIO control.

---

### `void DS1302_WriteRegister(uint8_t reg, uint8_t data)`
Writes `data` to the register specified by `reg`. Sends the write command followed by the byte.

---

### `uint8_t DS1302_ReadRegister(uint8_t reg)`
Reads and returns the value of a register from the DS1302.

---

### `void DS1302_SetDateTime(...)`
Sets the RTC with given values by The `DS1302_TimeRecord` struct:
- `sec`: Seconds
- `min`: Minutes
- `hour`: Hour
- `dayOfWeek`: Day of the week (1=Sunday, 7=Saturday)
- `date`: Day of the month
- `month`: Month
- `year`: Last two digits of year (e.g., 25 for 2025)

---

### `void DS1302_GetDateTime(...)`
Fills the provided pointer variables with the current time and date values from the RTC.

---

### `void DS1302_GetFormattedString(char *buffer)`
Formats the current time and date as a human-readable string:
`YYYY-MM-DD HH:MM:DD`

---

## 📚 Notes

- The RTC is battery-backed and retains time even when powered off.
- To set the time manually, uncomment and modify this line in `main.c`:
  ```c
  //SETTING THE TIME DATE
  DS1302_TimeRecord datetime={
    .sec=0,
    .min=29,
    .hour={.hour=1,.meridiem=NONE},
    .date=10,
    .month=6,
    .year=25,
    .day=TUE
  };
  
  ds1302_setTimeDate(&rtc, datetime);
  ```
- After setting the time once, you should comment it out again to prevent overwriting.

---

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
<hr>

## ✍️ Developer

**Semi Eren Gökyıldız**