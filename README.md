# 🚀 Project: OLED UI with Nested Menu, Relay Control & EEPROM

A powerful ESP32-based UI with scroll animation, nested menus, relay control, and EEPROM-based state saving — built with Adafruit SSD1306 OLED library.

---

## ✅ Features

- 📋 Multi-level nested menus (Relay Menu & Settings)
- 🔁 Smooth scroll animation between options
- 💾 EEPROM state saving for relay (retains after reset)
- ⚡ Physical relay ON/OFF control
- 🔙 Back navigation to Main Menu
- 🧠 Optimized rendering — only visible items drawn
- 📐 Clean and scalable code structure

---

## 📂 Menu Structure

Main Menu:
  - Relay Menu
    - Relay ON
    - Relay OFF
    - Back ⬅️
  - Settings
    - Brightness (dummy)
    - Theme (dummy)
    - Back ⬅️

---

## 🧠 Concepts Practiced

- OLED graphics using `Adafruit_SSD1306`
- EEPROM usage on ESP32:
  - `EEPROM.begin(size)`
  - `EEPROM.read()`, `write()`, and `commit()`
- State-driven menu handling using `enum`
- Smooth scrollY animations
- Input debouncing with `waitForRelease()`
- Component-based menu logic (getMenuArray, getMenuCount)

---

## 🔧 Hardware Setup

| Component       | Pin    |
|----------------|--------|
| OLED (I2C)     | SDA - 21, SCL - 22 |
| Button UP      | GPIO14 |
| Button DOWN    | GPIO13 |
| Button SELECT  | GPIO26 |
| Relay IN       | GPIO25 |

---

## 🔋 EEPROM Mapping

| Address | Purpose         |
|---------|-----------------|
| `1`     | Relay State (0/1) |

EEPROM is initialized with `EEPROM.begin(4)` — enough for future settings (like brightness/theme).

---

## 🛠️ Future Add-ons

- Potentiometer to adjust **Brightness** live
- Store brightness & theme into EEPROM
- Add **long-press gesture** to go back
- **Color themes** using display inversion
- Sound feedback (buzzer click)

---

## 📜 Full Code

> You'll paste your Arduino code here.

---

## 🧑‍💻 Built & documented by [Jeeva](https://github.com/YOUR-GITHUB-ID) 🚀

