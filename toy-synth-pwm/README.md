# Compile and run

./acli core search nano
ID                Version Name                                                                                 
arduino:avr       1.8.5   Arduino AVR Boards                                                                   
arduino:mbed_nano 2.7.2   Arduino Mbed OS Nano Boards                                                          
arduino:megaavr   1.8.7   Arduino megaAVR Boards                                                               
arduino:samd      1.8.13  Arduino SAMD Boards (32-bits ARM Cortex-M0+)                                         
arduino:mbed      2.7.2   [DEPRECATED] [DEPRECATED - Please install standalone packages] Arduino Mbed OS Boards

# Status before and after setup
./acli board list
Port                            Protocol Type              Board Name FQBN Core
/dev/cu.Bluetooth-Incoming-Port serial   Serial Port       Unknown             
/dev/cu.usbserial-14120         serial   Serial Port (USB) Unknown

CH340 works as arduino:avr
# One time setup
./acli core install arduino:avr 

# Compile
./acli compile --fqbn arduino:avr:nano toy-synth-pwm

# Upload
./acli upload -p /dev/cu.usbserial-14120 --fqbn arduino:avr:nano toy-synth-pwm