#include <Arduino.h>
#include "FastOut.h"
// Copyright 2011 David Mitchell
struct strand {
  int Start;
  int End;
  int Direction;
  byte clock;
  byte data;
};

#define MAX_STATE 0
#define NUM_LEDS 350
#define NUM_STRANDS 4
const strand Strands[NUM_STRANDS] = {
// unintuitively the end number is one past the last pixel in the strand
//  {0,100,1,10,11},
    {99,0,-1,4,5},  // left side of house
    {200,299,1,6,7}, // porch up left down right
    {100,199,1,8,9}, // right side of house
    {300,349,1,10,11} // spare
};

volatile int state = 0;

// When cast as a long, the bits are as follows:
// 0xFF blue
// 0xFF00 green
// 0xFF0000 red
// 0xRRGGBB

union color {
  struct {
    byte blue;
    byte green;
    byte red;
  } c;
  long rgb;
} ;
#define BLACK        0x000000
#define WHITE        0x404040
#define RED          0x400000
#define GREEN        0x004000
#define BLUE         0x000040
#define GOLD         0x403600
#define SILVER       0x2f4f4f
#define DIM_BLUE     0x00000c
#define DIM_GREEN    0x000800
#define DIM_MAGENTA  0x0c0004
#define DIM_ORANGE   0x0f0400
#define DIM_VIOLET   0x0f000f
#define DIM_MAGENTA2 0x0f0008
color Display[NUM_LEDS];

void show();
void Owen(int left, int right);
void Fill(int start, int end, int delta, long c1, long c2);
void FillB(int start, int end, int delta, long c1, long c2);
void Twinkle(int left, int right);
void TrigRainbow(int left, int right);
void ValRainbow(int left, int right);
void CandyCane(int left, int right);
void Scott(int left, int right);
void MidScroll(int left, int right, int mid);

void clear(int left, int right, long color) {
  // Turn all LEDs off.
  for(int Counter=left;Counter <= right; Counter++) {
    Display[Counter].rgb=color;
  }
}

void setup() {
  clear(0,NUM_LEDS+1, BLACK);
  for (int x =0; x < NUM_STRANDS; x++) {
    pinMode(Strands[x].data,OUTPUT);
    pinMode(Strands[x].clock,OUTPUT);
  }
  show();
}


void show()
{
  int i;
  for(int s=0; s < NUM_STRANDS; s++) {
    if (Strands[s].Direction > 0) {
      for(i=Strands[s].Start; i <= Strands[s].End; i += Strands[s].Direction) {
        fastOut(Strands[s].data,Strands[s].clock,MSBFIRST,Display[i].c.red);
        fastOut(Strands[s].data,Strands[s].clock,MSBFIRST,Display[i].c.green);
        fastOut(Strands[s].data,Strands[s].clock,MSBFIRST,Display[i].c.blue);      
      }
    } else {
      for(i=Strands[s].Start; i >= Strands[s].End; i += Strands[s].Direction) {
        fastOut(Strands[s].data,Strands[s].clock,MSBFIRST,Display[i].c.red);
        fastOut(Strands[s].data,Strands[s].clock,MSBFIRST,Display[i].c.green);
        fastOut(Strands[s].data,Strands[s].clock,MSBFIRST,Display[i].c.blue);      
      }
    }
  }
}

void show_one(int s)
{
  int i;
  delay(1);
  for(i=Strands[s].Start; i != Strands[s].End; i += Strands[s].Direction) {
//    fastOut(Strands[s].data,Strands[s].clock,LSBFIRST,Display[i].rgb);
    fastOut(Strands[s].data,Strands[s].clock,MSBFIRST,Display[i].c.red);
    fastOut(Strands[s].data,Strands[s].clock,MSBFIRST,Display[i].c.green);
    fastOut(Strands[s].data,Strands[s].clock,MSBFIRST,Display[i].c.blue);      
  }
}

int Timer(int duration) {
  // keep track of a timer. If duration is zero,
  // return true until the timer has expired. If called with a non-zero
  // value reset the timer.
  static unsigned long end_time = 0;
  if (duration != 0) {
    end_time = millis() + duration;
    return 0;
  } else {
    return (end_time > millis());
  }
}
void loop() {
  
  clear(0,349, BLACK);
//  Scott();
//  clear(300,349,0x10);
//  show();
//  for (int x=0; x < 25; x++) {
//    clear(300,349,0x080000);
//    show_one(3);
//    clear(300,349,0x000800);
//    show_one(3);
//  };
  Timer(15000);
  while(Timer(0)) {
    Owen(0,199);
    Fill(200,249,-8,GOLD,SILVER);
    FillB(250,299,-8,GOLD,SILVER);
    Twinkle(300,349);
    show();
  } 
  Timer(17000);
  while(Timer(0)) {
    Fill(200,249,-8,RED,GREEN);
    FillB(250,299,-8,RED,GREEN);
    Twinkle(0,199);
    TrigRainbow(300,349);
    show();
  };
  Timer(17000);
  while(Timer(0)) {
    Fill(200,249,8,RED,GREEN);
    FillB(250,299,-8,RED,GREEN);
    ValRainbow(0,199);
    TrigRainbow(300,349);
    show();
  }
  Timer(17000);
  while(Timer(0)) {
    ValRainbow(0,199);
    CandyCane(200,299);
    show();
  };
  Timer(17000);
//  clear(0,199,BLACK);
  while (Timer(0)) {
    TrigRainbow(0,199);
    CandyCane(200,299);
    show();
  };
  clear(0,299,BLACK);
  Timer(17000);
  Scott(0,199);
  clear(200,299,DIM_BLUE);
  while (Timer(0)) {
    show();
  };

}
void Owen(int left, int right) {
  int x=random(left,right+1);
  int y=random(x,right+1);
  clear (x,y,GOLD);
  x = random(left,right+1);
  y = random(x,right+1);
  clear (x,y,WHITE);
  x = random(left,right+1);
  y = random(x,right+1);
  clear (x,y,SILVER);
 
}
void Scott(int left, int right) {
  //Try to imitate Scott's lights
  const color palette[6] =
    {
     {{4,0,12}},
     {{12,0,0}},
     {{0,8,0}},
     {{0,8,15}},
     {{15,0,15}},
    };
  int i;
  int c;
  for (i=left; i <= right; i++) {
    c = random(0,4);
    Display[i].c.red=palette[c].c.red;
    Display[i].c.green=palette[c].c.green;
    Display[i].c.blue=palette[c].c.blue;
  };
};


void Fill(int start, int end, int delta, long c1, long c2) {
  long c,d;
  //Fill then empty a color based on delta. z is the
  //current position of the transition from one color
  //to another.
  static int z = -1;
  static int step = 0;
  if ((z < start) || (z> end)) {
    step = step ^ 1;
    if (delta > 0) {
      z = start + delta;
    } else {
      z = end + delta;
    }
  };
  if (step) {
    c = c1;
    d = c2;
  } else {
    c = c2;
    d = c1;
  }
  for (int i = start; i <=  end; i++) {
    if (i < z) {
        Display[i].rgb=d;
    } else {
        Display[i].rgb=c;
    }
  }
  z += delta;
}
void FillB(int start, int end, int delta, long c1, long c2) {
  long c,d;
  //Fill then empty a color based on delta. z is the
  //current position of the transition from one color
  //to another.
  static int z = -1;
  static int step = 1;
  if ((z < start) || (z> end)) {
    step = step ^ 1;
    if (delta > 0) {
      z = start + delta;
    } else {
      z = end + delta;
    }
  };
  if (step) {
    c=c1;
    d=c2;
  } else {
    c=c2;
    d=c1;
  }
  for (int i = start; i <=  end; i++) {
    if (i < z) {
      Display[i].rgb=d;
    } else {
      Display[i].rgb=c;
    }
  }
  z += delta;
}

void Twinkle(int left, int right) {
// Try and do a little twinkling star effect.
// Each time through cycle the stars. Then add one more.
// The brightness value MUST be even. We use even/odd to track if
// we are getting brighter or dimmer.

  int b = 32;
  int step = 4;
  int i;
  int bright;
  
  for (i = left; i <= right; i++) {
    if (Display[i].c.red != 0) {
      if (Display[i].c.red > b) {
        Display[i].c.red = b-1;
        Display[i].c.green = b-1;
        Display[i].c.blue = b-1;
      } else if (Display[i].c.red%2) {
        bright = max(Display[i].c.red-step, 0);
        Display[i].c.red = bright;
        Display[i].c.green = bright;
        Display[i].c.blue = bright;
      } else {
        bright = Display[i].c.red + step;
        Display[i].c.red = bright;
        Display[i].c.green = bright;
        Display[i].c.blue = bright;
      };
    };
  };
  i = random(left, right+1);
  Display[i].c.red = step;
  Display[i].c.green = step;
  Display[i].c.blue = step;
}
      
    
// Candy cane tries to do a barber pole effect.
void CandyCane(int left, int right) {
  int b = 32;
  int spacing =2;
  int delay = 3;
  int static cycle;
  int midpoint = (left+right)/2;
  if ((cycle>>delay) < spacing) {
    Display[midpoint].c.red = b;
    Display[midpoint].c.green = b;
    Display[midpoint].c.blue = b;
  } else {
    Display[midpoint].c.red = b;
    Display[midpoint].c.green = 0;
    Display[midpoint].c.blue = 0;
  }
  MidScroll(left, right, midpoint);
  cycle++;
  if ((cycle>>delay) > spacing * 2) {
    cycle = 0;
  };
//  delay(50);
}
    
// Siren is flashing blue and red. Not very interesting.
void Siren(int left, int right) {
  int spacing = 3;
  int d;
  int b = 32;
  int static cycle = 0;
  for (int i = left; i <= right; i++) {
    d = (i / spacing) + cycle;
    if (d%2) {
      Display[i].c.blue = 0;
      Display[i].c.red = b;
      Display[i].c.green = 0;
    } else {
      Display[i].c.blue = b;
      Display[i].c.red = b;
      Display[i].c.green = b;
    };
  };
  cycle = cycle ^ 1;

}

// This could be OK with some work. It should fill from the 'top'
void Fill() {
  int b = 32;
  int d = 10;
  int d2 = 10;
  int static i = 0;
  color rc;
    rc.c.red=random(b);
    rc.c.green=random(b);
    rc.c.blue=random(b);
    for (int j = 1; j < NUM_LEDS-i; j++) {
      Display[j].c.green=rc.c.green;
      Display[j].c.red=rc.c.red;
      Display[j].c.blue=rc.c.blue;
      delay(d2);
      if (j > 0) {
        Display[j-1].c.green = 0;
        Display[j-1].c.red = 0;
        Display[j-1].c.blue = 0;
      };
      show();
      delay(d);
    };

  i++;
  if (i >= NUM_LEDS) {
    i = 0;
  };
}

//Boring for this application. 
void Scan() {
  int b = 16;
  static int c = 0;
  clear(0, NUM_LEDS-1,BLACK);
  for (int i = 0; i < NUM_LEDS; i++) {
    if (c % 2) {
      Display[i].c.red = b;
      show();
      Display[i].c.red = 0;
    } else {
      Display[i].c.green = b;
      show();
      Display[i].c.green = 0;
    }
  }
  c++;
//  Display[NUM_LEDS-(2*i+1)].red = b;
//  show();
//  delay(d);
//  i++;
//  if (i >= NUM_LEDS) {
//    i = 0;
//  };
}

// This is a favorite if it can be tweaked for pinks and reds.
void ValRainbow(int left, int right) {
  int midpoint = (left+right)/2;
  int b = 64;
  const float STEP = 0.5;
  static float s = STEP;
  static float i = 0;
  int z = 12;
  MidScroll(left,right,midpoint);
  Display[midpoint].c.red = (sin(i+3.14)+1)*b;
  Display[midpoint].c.blue=(sin(i)+1)*0;
  Display[midpoint].c.green=(sin(i)+1)*b;
  i += s;
  if (i > 255) {
    s = -STEP;
  } else if (i < 1) {
    s = STEP;
  }
  if (((int)i%z) == 0) {
    clear(midpoint-4,midpoint+4,BLACK);
  }
}

void TrigRainbow(int left, int right) {
  int midpoint = (left+right)/2;
  int b = 32;
  const float STEP = 0.5;
  static float s = STEP;
  static float i = 0;
  MidScroll(left,right,midpoint);
  Display[midpoint].c.red=(sin(0.3*i)+1)*b;
  Display[midpoint].c.blue=(sin(i)+1)*b;
  Display[midpoint].c.green=(cos(i)+1)*b;
  i += s;
  if (i > 255) {
    s = -STEP;
  } else if (i < 1) {
    s = STEP;
  }
//  if (((int)i%z) == 0) {
//    clear(midpoint-4,midpoint+4,BLACK);
//  }
}

void Scroll() {
  for (int ik = NUM_LEDS-1; ik > 0; ik--) {
    Display[ik].c.red = Display[ik-1].c.red;
    Display[ik].c.blue = Display[ik-1].c.blue;
    Display[ik].c.green = Display[ik-1].c.green;
  }
}
  
// This is like Scroll(), but scrolls from the middle outward.
void MidScroll(int left, int right, int mid) {
  for (int i = left; i < mid; i++) {
    Display[i].c.red = Display[i+1].c.red;
    Display[i].c.green = Display[i+1].c.green;
    Display[i].c.blue = Display[i+1].c.blue;
  };
  for (int i = right; i > mid; i--) {
    Display[i].c.red = Display[i-1].c.red;
    Display[i].c.green = Display[i-1].c.green;
    Display[i].c.blue = Display[i-1].c.blue;
  }
}

void Blocks() {
  int x1,x2;
  int bright = 8;
  x1 = random(0,NUM_LEDS);
  x2 = random(x1,NUM_LEDS);
  byte r = random(0,bright);
  byte g = random(0,bright);
  byte b = random(0,bright);
  for (x1; x1<= x2; x1++) {
    Display[x1].c.red = r;
    Display[x1].c.green = g;
    Display[x1].c.blue = b;
  }
  show();
  delay(250);
}

void Static() {
  int bright = 32;
  int d = 3;  
  int x = random(0,NUM_LEDS);
  int v = random(0,bright);
  Display[x].c.red = v;
  Display[x].c.green=  v;
  Display[x].c.blue = v;
  show();
  delay(d);
}


void LimitedBlocks() {
  const color palette[6] =
    {
     {{15,0,0}},
     {{15,4,4}},
     {{15,15,15}},
     {{15,0,15}},
     {{15,0,15}},
     {{0,15,15}},
    };
  int x1,x2;
  x1 = random(0,NUM_LEDS);
  x2 = random(x1,NUM_LEDS);
  unsigned int rc = random(4);

  for (x1; x1<= x2; x1++) {
    Display[x1].c.red = palette[rc].c.red;
    Display[x1].c.green = palette[rc].c.green;    
    Display[x1].c.blue = palette[rc].c.blue;
  }
  show();
  delay(250); 
}

void SlidingBlocks() {
  const color palette[7] =
    {
     {{15,0,0}},
     {{15,4,4}},
     {{15,0,15}},
     {{0,0,0}},
     {{0,0,0}},
     {{0,15,15}},
     {{0,0,0}}
    };
  int d = 100;
  int prob = 10;
  static int rc = 0;
  
    if (random(100) < prob) {
      rc = random(3);  
    }
    Scroll();
    Display[0].c.red = palette[rc].c.red;
    Display[0].c.green = palette[rc].c.green;
    Display[0].c.blue = palette[rc].c.blue;
    show();
    delay(d); 
  
}

