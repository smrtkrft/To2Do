# Hardware Setup Guide

## Basic Setup (Minimal Configuration)

### Required Components

1. **ESP32 Development Board**
   - Any ESP32 board with USB port
   - Recommended: ESP32 DevKit V1, NodeMCU-32S, or WROOM-32
   - Minimum: 4MB Flash

2. **USB Cable**
   - Micro-USB or USB-C (depending on your ESP32 board)
   - Must support data transfer (not just power)

3. **Power Source**
   - USB power adapter (5V, 500mA minimum)
   - OR computer USB port
   - OR USB power bank

### Basic Connection

```
Computer/Power Source
        |
      [USB Cable]
        |
    [ESP32 Board]
```

**That's it for basic operation!** The ESP32 has built-in WiFi, so no additional components needed.

## Optional: OLED Display Setup (Future Enhancement)

### Additional Components Needed

1. **OLED Display**
   - 0.96" or 1.3" OLED (SSD1306 or SH1106)
   - I2C interface (4 pins: VCC, GND, SCL, SDA)
   - Resolution: 128x64 or 128x32

2. **Jumper Wires**
   - 4 female-to-female wires (if using separate display)

### OLED Wiring Diagram

```
ESP32 Board          OLED Display
-----------          ------------
    3.3V  ---------> VCC (or VDD)
     GND  ---------> GND
  GPIO22  ---------> SCL (Clock)
  GPIO21  ---------> SDA (Data)
```

**Pin Details:**
- VCC: Power (3.3V or 5V depending on display)
- GND: Ground
- SCL: I2C Clock line (GPIO22 default on ESP32)
- SDA: I2C Data line (GPIO21 default on ESP32)

**Alternative I2C Pins:**
You can use any GPIO pins for I2C, but these are standard:
- SCL: GPIO22 (default)
- SDA: GPIO21 (default)

## Optional: Button Input (Future Enhancement)

### Button Wiring

For menu navigation and task interaction:

```
ESP32 Board              Button          ESP32 Board
-----------              ------          -----------
  GPIO34  ----------> [Button 1] <----- GND
  GPIO35  ----------> [Button 2] <----- GND
  GPIO32  ----------> [Button 3] <----- GND
```

**Button Configuration:**
- Button 1: Menu/Select
- Button 2: Up/Previous
- Button 3: Down/Next

**Pull-up Configuration:**
ESP32 has internal pull-up resistors, so external resistors not required.
Configure in code: `pinMode(34, INPUT_PULLUP);`

## Power Requirements

### Current Draw

| Component | Current Draw |
|-----------|--------------|
| ESP32 (idle) | ~80mA |
| ESP32 (WiFi active) | ~160-260mA |
| OLED Display | ~20mA |
| **Total Maximum** | **~280mA** |

**Power Sources:**
- ✅ USB port: Typically 500mA - sufficient
- ✅ Wall adapter: 5V, 1A recommended
- ✅ Power bank: Any USB power bank works
- ❌ 3.3V battery: Not enough voltage
- ⚠️ 5V battery: Needs voltage regulator

### Power Supply Options

1. **USB Power (Recommended)**
   - Simplest option
   - Always available during development
   - Stable 5V supply

2. **Battery Power**
   - For portable use
   - Options:
     - 18650 Li-ion battery (3.7V) + buck/boost converter to 5V
     - USB power bank (easiest portable solution)
     - 9V battery + 5V regulator (inefficient)

3. **Wall Adapter**
   - For permanent installation
   - 5V, 1A USB adapter
   - Most phone chargers work

## Complete Setup Options

### Option 1: Development Setup (No Display)

**Components:**
- ESP32 board
- USB cable
- Computer

**Use Case:**
- Development and testing
- Web interface only
- Lowest cost
- Easiest setup

**Cost:** ~$5-10

### Option 2: Basic Production (With OLED)

**Components:**
- ESP32 board
- 0.96" OLED display (I2C)
- 4 jumper wires
- USB power adapter

**Use Case:**
- Standalone operation
- Visual feedback on device
- No computer needed after programming

**Cost:** ~$10-15

### Option 3: Full Featured (OLED + Buttons)

**Components:**
- ESP32 board
- 1.3" OLED display (I2C)
- 3 push buttons
- Jumper wires
- Small project box (optional)

**Use Case:**
- Complete standalone device
- Full menu navigation
- Professional appearance

**Cost:** ~$15-20

### Option 4: Portable (Battery Powered)

**Components:**
- ESP32 board
- OLED display
- 3 buttons
- USB power bank OR
- 18650 battery + TP4056 charging module + boost converter
- Project box

**Use Case:**
- On-the-go task management
- No wall power needed
- Take it anywhere

**Cost:** ~$20-30

## Enclosure Options

### 3D Printed Enclosure

Design considerations:
- Cutout for USB port (programming/power)
- Cutout for OLED display
- Mounting holes for ESP32
- Button openings (if used)
- Ventilation holes

**Dimensions (typical):**
- ESP32 DevKit: 55mm x 28mm x 15mm
- With OLED: 60mm x 40mm x 20mm
- With buttons: 80mm x 60mm x 30mm

### Commercial Enclosure

Standard project boxes that fit:
- 85mm x 58mm x 33mm (Hammond 1591ASBK equivalent)
- 100mm x 68mm x 50mm (for more room)

### Desktop Stand

Simple 3D printed stand:
- Angled for easy viewing
- Cable management
- Non-slip feet

## Assembly Tips

### General Guidelines

1. **First Upload Code**
   - Program ESP32 before final assembly
   - Test all functionality
   - Easier to access USB port

2. **Test Components Separately**
   - Verify OLED works before mounting
   - Test buttons individually
   - Check power consumption

3. **Secure Connections**
   - Solder wires for permanent installation
   - Use heat shrink tubing
   - Label wires clearly

4. **Cable Management**
   - Keep I2C wires short (< 30cm)
   - Avoid parallel runs with power wires
   - Use ribbon cable for neat appearance

5. **Mounting**
   - Use standoffs for ESP32
   - Hot glue for temporary setups
   - Screws for permanent installation

### OLED Display Mounting

**Method 1: Direct Mount**
- Use M2 screws through display mounting holes
- Add small spacers if needed

**Method 2: Hot Glue**
- Quick and easy
- Difficult to remove later
- Good for prototypes

**Method 3: 3D Printed Holder**
- Professional appearance
- Easy to replace display
- Requires 3D printer

## Testing Checklist

Before final assembly:

- [ ] ESP32 powers on
- [ ] Code uploads successfully
- [ ] WiFi connects
- [ ] Web interface accessible
- [ ] OLED displays text (if used)
- [ ] Buttons respond (if used)
- [ ] Serial monitor shows no errors
- [ ] All connections secure
- [ ] No short circuits
- [ ] Power consumption acceptable

## Safety Notes

⚠️ **Important Safety Information:**

1. **Polarity**
   - Check VCC/GND before connecting
   - Reversed polarity can damage components
   - Use multimeter to verify

2. **Voltage Levels**
   - ESP32 GPIO is 3.3V tolerant
   - OLED can be 3.3V or 5V (check specs)
   - Never apply > 3.6V to ESP32 GPIO

3. **Current Limits**
   - ESP32 GPIO max: 40mA per pin
   - Total for all GPIO: 200mA
   - USB port max: Usually 500mA

4. **Heat**
   - ESP32 can get warm during WiFi use (normal)
   - Ensure adequate ventilation
   - Don't cover with insulating materials

5. **Static Discharge**
   - Touch grounded object before handling
   - ESP32 is static-sensitive
   - Work on anti-static mat if possible

## Troubleshooting Hardware

### ESP32 Won't Power On
- Check USB cable (try another)
- Check power source
- Look for onboard LED
- Measure voltage at 3.3V pin

### OLED Not Displaying
- Check I2C address (usually 0x3C or 0x3D)
- Verify wiring (SCL/SDA not swapped)
- Check power to display
- Run I2C scanner sketch

### Buttons Not Working
- Verify GPIO pins
- Check for shorts
- Test with multimeter
- Verify pull-up configuration

### Random Resets
- Insufficient power
- Bad USB cable
- Short circuit
- Faulty ESP32 board

## Next Steps

Once hardware is assembled:
1. Follow GETTING_STARTED.md for software setup
2. Upload the sketch
3. Configure WiFi
4. Start using To2Do!

## Resources

- ESP32 Pinout: https://randomnerdtutorials.com/esp32-pinout-reference-gpios/
- I2C Scanner: Available in Arduino IDE examples
- OLED Libraries: Adafruit_SSD1306, Adafruit_GFX
- Community: ESP32 forums, Arduino forums
