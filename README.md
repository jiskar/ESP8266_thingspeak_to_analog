# ESP8266_thingspeak_to_analog

reads the last value from a [thingspeak](https://thingspeak.com/) channel using an ESP8266 wifi chip and set's a pwm channel to it's value. 

I use this to display the channel's value on an analog panel meter.

Tested on an [olimex MOD-WIFI-ESP8266-DEV](https://www.olimex.com/Products/IoT/MOD-WIFI-ESP8266-DEV/open-source-hardware) board from arduino IDE 1.6.5

## Hardware connections

On the MOD-WIFI-ESP8266-DEV board I have the following connections:

* pin 1: 3.3V
* pin 2: gnd
* pin 3: rx from usb-serial cable (green)
* pin 4: tx from usb-serial cable (red)
* pin 20: analog panel meter via voltage divider
* pin 21: gnd (for programming), open (for running)

Furthermore I've removed the left most print-switch (IO0JP aka GPIO0) in order to be able to change the mode between UART and FLASH using pin 21.

The analog panel meter is connected at the center of a 22k/1k resistor divider which is between pin20 (the pwm output) and gnd. Over the panel meter is a 22uF cap for buffering. The resistor values depend on your specific panel meter and it would be nice to add a potentiometer in series between the lower resistor and ground so you can adjust the meter's maximum value.

## Usage

Install the [arduino core for ESP8266](https://github.com/esp8266/Arduino). 

In the arduino IDE go to Tools > Boards and select "Olimex MOD-WIFI-ESP8266(-DEV)".

Power up the ESP, plug in the usb programming cable, note the COM port and set it correctly in the arduino IDE. (Tools > Port)

Fill in your wifi credentials and the channel, key and field values of the thingspeak channel you'd like to read from

On the ESP, connect pin 21 to ground. Reset the board by disconnecting and reconnecting the power.

Hit the upload button in the arduino IDE and wait till the code is uploaded.

Open a terminal to get the script's output (set it to 11520 baud)
