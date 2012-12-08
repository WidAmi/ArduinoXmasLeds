#include "FastOut.h"
#include "wiring_private.h"
#include "pins_arduino.h"

// This is pretty much a copy of the Arduino shiftOut. That function calls
// digitalWrite() twice per bit, resulting in 16 calls per byte shifted out.
// The digitalWrite() function ends up doing a bunch of redundant work such as
// looking up the registers and such in addition to the overhead of the 
// function calls. To speed it up the digitalWrite() and shiftOut() are
// merged together.
void fastOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
  uint8_t i;
  uint8_t oldSREG;
  uint8_t dtimer = digitalPinToTimer(dataPin);
  uint8_t dbit = digitalPinToBitMask(dataPin);
  uint8_t dport = digitalPinToPort(dataPin);
  uint8_t ctimer = digitalPinToTimer(clockPin);
  uint8_t cbit = digitalPinToBitMask(clockPin);
  uint8_t cport = digitalPinToPort(clockPin);
  volatile uint8_t *dout;
  volatile uint8_t *cout;
  
  if (dport == NOT_A_PIN) return;
  if (cport == NOT_A_PIN) return;

  // If the pin that support PWM output, we need to turn it off
  // before doing a digital write.
 // if (dtimer != NOT_ON_TIMER) turnOffPWM(dtimer);
 // if (ctimer != NOT_ON_TIMER) turnOffPWM(ctimer);
  dout = portOutputRegister(dport);
  cout = portOutputRegister(cport);
  oldSREG = SREG;
  cli();
  for (i = 0; i < 8; i++)  {
    if (bitOrder == LSBFIRST) {
      if (!!(val & (1 << i)) == LOW){
 //       oldSREG = SREG;
//        cli();
        *dout &= ~dbit;
//        SREG = oldSREG;
      } else {
//        oldSREG = SREG;
//        cli();
        *dout |= dbit;
//        SREG = oldSREG;
      }
    } else {
      if ((!!(val & (1 << (7 - i)))) == LOW) {
//        oldSREG = SREG;
//        cli();
        *dout &= ~dbit;
//        SREG = oldSREG;
      } else {
//        oldSREG = SREG;
 //       cli();
        *dout |= dbit;
//        SREG = oldSREG;
      }
    }
 // Write HIGH
//    oldSREG = SREG;
//    cli();
    *cout |= cbit;
//    SREG = oldSREG;
// Write LOW
//    oldSREG = SREG;
//    cli();
    *cout &= ~cbit;
    SREG = oldSREG;
//    digitalWrite(clockPin, HIGH);
//    digitalWrite(clockPin, LOW);
  }
}

