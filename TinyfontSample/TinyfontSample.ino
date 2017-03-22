#include <Arduboy2.h>
#include "Tinyfont.h"

Arduboy2 arduboy;
Tinyfont tinyfont;
char allLetters[101];

void setup() {
  // put your setup code here, to run once:

    arduboy.begin();

    // create all ascii letters from 32-126
    size_t newLineCounter = 0;
    for (size_t i = 0; i < 100; i++) {
      if ((i % 26) == 0) {
        allLetters[i] = '\n';
        newLineCounter++;
      }
      else{
        allLetters[i] = (char) (i+32) - newLineCounter;
      }
    }
    allLetters[100] = '\0';
}

void loop() {
  // put your main code here, to run repeatedly:

  arduboy.clear();
  // for comparison
  arduboy.print("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
  tinyfont.print("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0, 8);

  // all letters
  tinyfont.print(allLetters, 0, 12);

  arduboy.display();
}
