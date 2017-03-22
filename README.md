# Arduboy-TinyFont  
Tiny 4x4 Font for Arduboy which contains the small ASCII Table from 32 to 127.  
The sprites for the font are using 224 bytes.

![Preview](https://github.com/yinkou/Arduboy-TinyFont/blob/master/bitmaps/tinyfont-preview.png?raw=true "Font Preview")

## Usage:
Make an instance of `Tinyfont` and call `print()` with you c-string and position.

## Customization:
You can adjust `letterSpacing` and `lineHeight`.

## Dependencies:
Arduboy2 is required to work since it writes to sBuffer[].

## Optimization:
I'm pretty sure this can be optimized.

#### Arduboy2::print()
`Sketch uses 7896 bytes (27%) of program storage space`

#### Tinyfont::print()
`Sketch uses 8610 bytes (30%) of program storage space.`

#### Both:
`Sketch uses 8626 bytes (30%) of program storage space`
