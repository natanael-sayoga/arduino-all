# For my particular LCD:
### BLA (Backlight Anode)
- **Function**: This pin is the positive connection for the backlight.
- **Connection**: It should be connected to a positive voltage supply (commonly **5V**). This powers the backlight of the LCD, allowing it to illuminate the display, making it easier to read in low-light conditions.

### BLK (Backlight Cathode)
- **Function**: This pin is the negative connection for the backlight.
- **Connection**: It should be connected to **GND** (ground). This completes the circuit for the backlight, allowing current to flow through the backlight LEDs.

### RS (Register Select)
- **Function**: Determines whether the data being sent to the LCD is command or character data.
  - **RS = 0**: Command mode (for commands like clearing the display, setting cursor position, etc.).
  - **RS = 1**: Data mode (for sending character data to be displayed).

### RW (Read/Write)
- **Function**: Sets the direction of data flow.
  - **RW = 0**: Write mode (default mode for sending data to the LCD).
  - **RW = 1**: Read mode (used to read data from the LCD, though often not used in typical applications).

### E (Enable)
- **Function**: This pin is used to latch the information present on the data pins.
- **Operation**: 
  - To send data or commands, you toggle this pin:
    1. Set **E** high (1) to enable the LCD to read the data on the data pins.
    2. Set **E** low (0) to complete the operation.

### Summary
- **RS**: Selects command or data mode.
- **RW**: Determines if the operation is read or write.
- **E**: Enables the execution of the command or data.
- **BLA**: Provides power to the backlight (positive terminal).
- **BLK**: Serves as the ground connection for the backlight (negative terminal).

These pins allow the microcontroller (like the ESP8266) to effectively communicate with the LCD and control its behavior.
The **RS**, **RW**, and **E** pins on an LCD are crucial for controlling the display and communicating with it. <br>
The BLA and BLK pins on an LCD are related to the backlighting functionality. <br>

# FILE:
1. **mqtt.py**  
   running and testing mqtt server connection, you might need to run **pip install paho-mqtt** first to run this file!
2. **scan_i2c.ino**  
   finding out device address for LCD practices
