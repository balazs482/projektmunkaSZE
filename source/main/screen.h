#ifndef __SCREEN_H__
#define __SCREEN_H__

#include <Wire.h>
#include "ACROBOTIC_SSD1306.h"

constexpr short HEIGHT = 8, WIDTH = 16;

/**
 * Contains functions to use the screen easily.
 */
class Screen
{  
public:
  /**
   * Replaces the text in a specific line.
   * @param lineNum Number of the line.
   * @param input Text to write.
   */
	static void writeLine(short lineNum, std::string input)
	{
    input += std::string(WIDTH - input.length(), ' '); //Complements the line with spaces to fill the width of the screen.
		oled.setTextXY(lineNum, 0); //Sets the starting position of the line.
    oled.putString(input.c_str()); //Writes the line.
	}
  /**
   * Clears the screen. All characters will disappear.
   */
  static void clear() { oled.clearDisplay(); }
};

#endif
