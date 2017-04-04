#include "Tinyfont.h"
#include "TinyfontSprite.c"
#include <string.h>

#define TINYFONT_WIDTH 4
#define TINYFONT_HEIGHT 4

Tinyfont::Tinyfont(uint8_t *screenBuffer, int16_t width, int16_t height){
  sBuffer = screenBuffer;
  sWidth = width;
  sHeight = height;

  // default values
  lineHeight = TINYFONT_HEIGHT + 1;
  letterSpacing = 1;
}

void Tinyfont::print(const char *text, int16_t x, int16_t y){
  size_t l = strlen(text);
  int cursorX = x;
  int cursorY = y;

  for (size_t i = 0; i < l; i++) {
    // check for new line
    if(text[i] == '\n'){
      cursorX = x; // cr
      cursorY += lineHeight; // lf
    }
    // check for tab
    else if(text[i] == '\t'){
      cursorX += TINYFONT_WIDTH + 5;
    }
    else{
      // draw char
      Tinyfont::printChar(text[i], cursorX, cursorY);
      cursorX += TINYFONT_WIDTH + letterSpacing;
    }
  }
}

//common functions
void Tinyfont::printChar(char c, int16_t x, int16_t y)
{

  // no need to draw at all of we're offscreen
  if (x + TINYFONT_WIDTH <= 0 || x > sWidth - 1 || y + TINYFONT_HEIGHT <= 0 || y > sHeight - 1)
    return;

  // check if char is available
  if (((uint8_t) c) < 32 || ((uint8_t) c) > 255) c = (char)127;

  uint8_t cval = ((uint8_t) c) - 32;

  // layout lowercase letters
  if (cval >= 65 && cval <= 90) y++;

  // layout comma letters
  if (cval == 12 || cval == 27) y++;

  // get sprite frames
  uint8_t spriteFrame = cval/4;

  const uint8_t *letter = TINYFONT_SPRITE + (spriteFrame * 8);

  // check if character is in the right part of the sprite
  if (cval % 2 == 1) letter += 4;

  for (uint8_t i = 0; i < 4; i++ ) {

    uint8_t colData = pgm_read_byte(letter++);
    if (c % 4 < 2) {
      // mask upper sprite
      colData &= 0x0f;
    }
    else{
      // for every odd character, shift pixels to get the correct character
      colData >>= 4;
    }
    for ( uint8_t j = 0; j < 4; j++ ) {
      // shift our pixel over
      uint8_t pixel = (colData >> j) & 0x01;
      drawPixel(x+i, y+j, pixel);
    }
  }
}

void Tinyfont::drawPixel(int16_t x, int16_t y, uint8_t color)
{
  uint8_t row = (uint8_t)y / 8;
  if (color)
  {
    sBuffer[(row*sWidth) + (uint8_t)x] |=   _BV((uint8_t)y % 8);
  }
  else
  {
    sBuffer[(row*sWidth) + (uint8_t)x] &= ~ _BV((uint8_t)y % 8);
  }
}

void Tinyfont::drawByte(int16_t x, int16_t y, uint8_t pixels){

}
