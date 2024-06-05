# whiplash
IMU-based Virtual Drum Stick with Haptic Feedback

# Usage 
- Compile and upload the arduino source code to the Teensy board. Kick for the foot pedal and stick for both hand stick. Modify the stiffness k to get personlized experience.
- Modify the serial ports for each Teensy.
```
SERIAL_PORT_right = '/dev/tty.usbmodem56478001'
SERIAL_PORT_left = '/dev/tty.usbmodem68460701'
SERIAL_PORT_bottom = "/dev/tty.usbmodem32728401"
```
- Run python interface 
```
python drum.py
```
