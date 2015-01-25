ArduinoXmasLeds
===============

Arduino code for controlling WS2801 based LED pixel strings as christmas lights.

The code produces a number of hopefully interesting patterns and displays them
using the WS2801 LED strings. 

Physical connections
- I normally deploy the strings in lengths of 100. Any longer and the voltage
  drop starts to mess with their color. At high intensities this can be seen
  with even 100 LEDs, but high intensities are not required.
- Some of the strings are laid out end to end and so the code should be able to
  treat them as one long string.
- Some strings are installed on other parts of the house and should display a
  different pattern.

Wiring Reminder
- I have used four conductor trailer wiring for the connections from the
  controller and power supply out to the individual strands. It is overkill
  and in retrospect kind of clunky to deal with. That said I have it wired
  up as such:
  * White: Ground
  * Brown: 5V
  * Yellow: Clock
  * Green: Data
