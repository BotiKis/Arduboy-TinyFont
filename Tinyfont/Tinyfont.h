#ifndef TINYFONT_H
#define TINYFONT_H

#include <Arduboy2.h>

/**
 * Tinyfont uses a 4x4 font to print text.
 * It conforms do standard ASCII letters in range from 32 to 126.
 * Even tough the lowercase letters are available, the best readability
 * is given with the Uppercase letters.
 *
 * Usage:
 * Call the print() method of a Tinyfont instance.
 *
 * Special letters:
 * \n makes a linebreak in print().
 * \t makes a 9 px long jump in print().
 */
class Tinyfont{
  public:
    Tinyfont();
    void printChar(char c, int16_t x, int16_t y);       //!< Prints a single letter in ASCII range from 32 to 126. Everything else will be 127, wich is a square fallback sprite.
    void print(const char *text, int16_t x, int16_t y); //!<  Prints the given c-string.

    uint8_t letterSpacing;  //!< LetterspletterSpacingacing controls the distance between letters. Default is 1.
    uint8_t lineHeight;     //!< lineHeight controls the height between lines breakend by \n. Default is 5.

  private:
    void drawPixel(int16_t x, int16_t y, uint8_t color);  
};

#endif

