#include "Tinyfont.h"
#include "TinyfontSprite.c"

#define TINYFONT_WIDTH 4
#define TINYFONT_HEIGHT 4

Tinyfont::Tinyfont(){
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
  if (x + TINYFONT_WIDTH <= 0 || x > WIDTH - 1 || y + TINYFONT_HEIGHT <= 0 || y > HEIGHT - 1)
    return;

  // check if char is available
  if (((uint8_t) c) < 32 || ((uint8_t) c) > 127) c = (char)127;

  uint8_t cval = ((uint8_t) c) - 32;

  // get sprite frames
  uint8_t spriteFrame = cval/4;
  uint8_t maskFrame = cval%4;

  // calc correct position
  switch (maskFrame) {
    case 1:
      x-=4;
      break;
    case 2:
      y-=4;
      break;
    case 3:
      x-=4;
      y-=4;
      break;
    case 0:
    default:
      break;
  }

  // layout lowercase letters
  if (cval >= 65 && cval <= 90) y++;

  const uint8_t *bitmap = TINYFONT_SPRITE;
  const uint8_t *mask = TINYFONT_MASK;
  unsigned int frame_offset = 8;

  // from here it's draw sprite code from the Sprites Class

  if (spriteFrame > 0 || maskFrame > 0) {
    mask += maskFrame * frame_offset;
    bitmap += spriteFrame * frame_offset;
  }

  // xOffset technically doesn't need to be 16 bit but the math operations
  // are measurably faster if it is
  uint16_t xOffset, ofs;
  int8_t yOffset = abs(y) % 8;
  int8_t sRow = y / 8;
  uint8_t loop_h, start_h, rendered_width;

  if (y < 0 && yOffset > 0) {
    sRow--;
    yOffset = 8 - yOffset;
  }

  // if the left side of the render is offscreen skip those loops
  if (x < 0) {
    xOffset = abs(x);
  } else {
    xOffset = 0;
  }

  // if the right side of the render is offscreen skip those loops
  if (x + 8 > WIDTH - 1) {
    rendered_width = ((WIDTH - x) - xOffset);
  } else {
    rendered_width = (8 - xOffset);
  }

  // if the top side of the render is offscreen skip those loops
  if (sRow < -1) {
    start_h = abs(sRow) - 1;
  } else {
    start_h = 0;
  }

  loop_h = 1; // divide, then round up

  // if (sRow + loop_h - 1 > (HEIGHT/8)-1)
  if (sRow + loop_h > (HEIGHT / 8)) {
    loop_h = (HEIGHT / 8) - sRow;
  }

  // prepare variables for loops later so we can compare with 0
  // instead of comparing two variables
  loop_h -= start_h;

  sRow += start_h;
  ofs = (sRow * WIDTH) + x + xOffset;
  uint8_t *bofs = (uint8_t *)bitmap + (start_h * 8) + xOffset;
  uint8_t *mask_ofs;
  if (mask != 0)
    mask_ofs = (uint8_t *)mask + (start_h * 8) + xOffset;
  uint8_t data;

  uint8_t mul_amt = 1 << yOffset;
  uint16_t mask_data;
  uint16_t bitmap_data;

  for (uint8_t a = 0; a < loop_h; a++) {
    for (uint8_t iCol = 0; iCol < rendered_width; iCol++) {
      // load data and bit shift
      mask_data = pgm_read_byte(mask_ofs) * mul_amt;
      bitmap_data = pgm_read_byte(bofs) * mul_amt;

      if (sRow >= 0) {
        data = Arduboy2Base::sBuffer[ofs];
        data |= (uint8_t)(bitmap_data & mask_data);
        Arduboy2Base::sBuffer[ofs] = data;
      }
      if (yOffset != 0 && sRow < 7) {
        data = Arduboy2Base::sBuffer[ofs + WIDTH];
        data |= (*((unsigned char *) (&bitmap_data) + 1)) & (*((unsigned char *) (&mask_data) + 1));
        Arduboy2Base::sBuffer[ofs + WIDTH] = data;
      }
      ofs++;
      mask_ofs++;
      bofs++;
    }
    sRow++;
    bofs += 8 - rendered_width;
    mask_ofs += 8 - rendered_width;
    ofs += WIDTH - rendered_width;
  }
}
