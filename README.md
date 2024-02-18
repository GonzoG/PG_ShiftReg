# Arduino-Fast-digital-I-O<br>
-------------------------
Fast digital IO for Arduino AVR boards.<br>
Using port registers.

Usage is same as with standard arduino digitalRead / digitalWrite: <br>

<b>bool digitalReadFast(uint8_t pin);</b><br>
<b>digitalWriteFast(uint8_t pin, uint8_t val);</b><br>
<br>
Speed difference:<br>
Arduino Uno/Nano:<br>
-------------------------
digitalWrite - 0.28M operations/s<br>
digitalWriteFast - theoretical up to 8M operations/s, tested at 7.8M

digitalRead - 0.26M/s<br>
digitalReadFast - 15.8M/s<br><br>

**Arduino Mega** (speed depends on pin. Some pins are slower, some faster)<br>
---------------------
digitalWrite - 0.18-0.26M/s<br>
digitalWriteFast - 3.2-7.9M/s<br>
