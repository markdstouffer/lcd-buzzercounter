
/**
 * MARK STOUFFER
 * CS1800 J. RACHLIN
 * Final Project Code
 */

/* library code for LCD display: */
#include <LiquidCrystal.h>

/* Constants*/
#define STARTVAL 0                          // the program will begin at this number
#define MAXVAL 256                          // the program will count up to this number
#define COUNTDELAY 500                      // the delay between incrementing numbers (~1 second)
#define SWITCHDELAY 2000                    // the delay between switching between countmodes (~2 seconds)
#define BIN 0                               // assign binary countmode to 0
#define HEX 1                               // assign hex countmode to 1

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
int photoresistor = 0;
int thresholdSwitch = 400;
int thresholdDecrement = 700;
char hex0current = 0;
char hex1current = 0;
byte countmode;
byte currentVal;
long int last;
long int lastSwitch;
long int currentTime;


void setup() {
  lcd.begin(16, 2);                         // LCD with 16 rows, 2 columns
  pinMode(7, OUTPUT);                       // Set pin 7 (speaker) to output

  currentVal = STARTVAL;
  countmode = BIN;
  last = 0;
  lastSwitch = 0;

  printheader(countmode);                   // print initial countmode on the LCD's top column
  

}

void loop() {
  photoresistor = analogRead(A0);           // keep updating photoresistor with the current value read from photoresistor (wired to pin A0)

  printval(currentVal, countmode);

  currentTime = millis();                   // set current time to time since program start (in milliseconds)
  if (currentTime - last > COUNTDELAY) {
    last = currentTime;
    if (photoresistor < thresholdDecrement) {
      if (currentVal < MAXVAL) {
        currentVal--;
      } else {
        currentVal = STARTVAL;
      }
    } else if (photoresistor >= thresholdDecrement) {
      if (currentVal < MAXVAL) {
        currentVal++;
      } else {
        currentVal = STARTVAL;
      }
    }                                       // decrement number if photoresistor returns value lower than thresholdDecrement
  }

  if (currentTime - lastSwitch > SWITCHDELAY) {
    lastSwitch = currentTime;
    if (photoresistor < thresholdSwitch) {
      lcd.clear();
      countmode = !countmode;
      printheader(countmode);
    }
  }                                         // switch the countmode if photoresistor returns value lower than thresholdSwitch
}

/**
 * Write a string of either Hex or Binary numbers to the LCD
 * @param val         currentVal of the counter
 * @param countmode
 */
 void printval(int val, byte countmode) {
  char arrWritten[9];
  lcd.setCursor(0, 1);
  if (countmode == BIN)
    writeBinary(val, arrWritten);
  else 
    writeHex(val, arrWritten);

  lcd.print(arrWritten);
 }

/**
 * Convert decimal input to binary, assign to character pointer array digBin
 * - play a note when changing from a 0 to a 1
 * @param currentDec     current value in decimal
 * @param *arrBin        pointer to arrWritten array
 */
 void writeBinary(int currentDec, char *arrBin) {
  int ct = 0;
  int x = 0;
  byte bitPos = MAXVAL/2;
  int change = currentDec == 0 ? 0 : currentDec ^ (currentDec - 1);

  for (byte count = 0; count < 8; count++) {
    if (change & bitPos && ct < 1) {
      ct++;
      x = count;
    }
    if (currentDec & bitPos)
      arrBin[count] = '1';
    else
      arrBin[count] = '0';

    bitPos = bitPos >> 1;
  }
  if (change != 0)
    play(x);
  arrBin[8] = '\0';
 }

/**
 * Convert decimal input to hexadecimal, assign to character pointer array digHex
 * - play a note when changing number
 * @param currentDec     current value in decimal
 * @param *arrHex        pointer to arrWritten array
 */
void writeHex(int currentDec, char *arrHex) {
  arrHex[0] = hexCharacter(currentDec >> 4);
  arrHex[1] = hexCharacter(currentDec & 0x0f);
  arrHex[2] = 0;

  if (arrHex[0] != hex0current) 
    play(currentDec >> 4);
  else if (arrHex[1] != hex1current)
    play(currentDec & 0x0f);

  hex0current = arrHex[0];
  hex1current = arrHex[1];
}

/**
 * Convert decimal input to hexadecimal character using ASCII tables
 * @param currentASCII     ASCII decimal to be converted to char
 * return Hex Character
 */
char hexCharacter(char currentASCII) {
  if (currentASCII <= 9)
    return currentASCII + 48;
  else
    return currentASCII + 55;
}

void printheader(byte countmode) {
  if (countmode == BIN)
    lcd.print("Mode: Binary");
  else
    lcd.print("Mode: Hex");
}

void play(int noteNum) {
  int frequencies[] = {262, 277, 294, 311, 330, 350, 370, 392, 415, 440, 466, 494, 523, 554, 587, 622, 659};
  tone(7, frequencies[noteNum], 40);
  delay(25);
}
"# lcd-buzzercounter" 
