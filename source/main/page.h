#ifndef __PAGE_H__
#define __PAGE_H__

#include <vector>
#include <string>

/**
 * Virtual class to display things on the screen.
 */
class Page
{
protected:
  std::string title; /**< Title of the page that's meant to be displayed on the top of the screen. */
public:
  /**
   * Constructor. As a virtual class, it will not be used. See subclasses for further information.
   */
  Page(std::string title) : title(title) {}
  /**
   * Not every Page has an arrowPos member variable. See subclasses for further information.
   */
  virtual short getArrowPos() {}
  /**
   * Not every Page has an arrowPos member variable. See subclasses for further information.
   */
  virtual void setArrowPos(short) {}
  /**
   * Not every Page has an arrowPos member variable. See subclasses for further information.
   */
  virtual void moveArrow(bool) {}
  /**
   * Different Page subclasses are displayed differently. See subclasses for further information.
   */
  virtual void printPage() {} 
  /**
   * Not every Page has an opts member variable. See subclasses for further information.
   */
  virtual short getOptValue(short pos) {}
  /**
   * Some Page subclasses are displayed static. See subclasses for further information.
   */
  virtual void refreshPage() {}
};

/**
 * Subclass to store data and display them on the screen.
 * Used to navigate and choose between options.
 * @see Page
 */
class MenuPage : public Page
{
private:
  std::vector<std::pair<std::string, short>> opts; /**< Stores options. Every option consists of a text and a corresponding value. The value may not be used with some pages. */
  short arrowPos = 0; /**< Stores the position of the arrow used for selection. It is used to navigate in the opts and to display on the screen. */
public:
  /**
   * Constructor.
   * @parameter title Title of the page.
   */
  MenuPage(std::string title) : Page(title) {}
  /**
   * Used to add an option to the options. 
   * @param _text Text part of the option.
   * @param _value Value part of the option.
   */
  void addOpt(std::string _text, short _value = 0) { opts.push_back(std::pair<std::string, short>(_text, _value)); }
  /**
   * Used to get the value part of an option.
   * @param pos Index of the option.
   * @return Value of the given option.
   */
  short getOptValue(short pos) { return opts[pos].second; }
  /**
   * Used to get the position of the arrow.
   * @return Position of the arrow.
   */
  short getArrowPos() { return arrowPos; }
  /**
   * Used to set the position of the arrow.
   * @param _arrowPos Desired position of the arrow.
   */
  void setArrowPos(short _arrowPos) { arrowPos = _arrowPos; }
  /**
   * Used to move the arrow and execute relevant actions. Movement through the menu items work circularly.
   * @param upDirection Direction of the movement. True for moving up and false for moving down. 
   */
  void moveArrow(bool upDirection)
  {
    Screen::writeLine(arrowPos + 1, "   " + opts[arrowPos].first); /**< Rewrites the line where the arrow has been displayed, without the arrow. With well working screens, only the affacted part could be rewritten. */
    if (upDirection && arrowPos == 0) arrowPos = opts.size() - 1; /**< If the first option has been selected and the arrow moves up, the last option will be selected. */
    else if (!upDirection && arrowPos == opts.size() - 1) arrowPos = 0; /**< If the last option has been selected and the arrow moves down, the first option will be selected. */
    else upDirection ? arrowPos-- : arrowPos++; /**< If no circular step happens, the neighbouring option in the proper direction will be selected. */
    Screen::writeLine(arrowPos + 1, " > " + opts[arrowPos].first); /**< Rewrites the line where the arrow should be displayed, with the arrow. With well working screens, only the affacted part could be rewritten. */
  }
  /**
   * Used to display a Page on the screen.
   */
  void printPage()
  {
    Screen::clear(); /**< Clears the screen. It is done for insuring the lack of glitches on the screen. With well working screens, it only adds unnecessary delay. */
    Screen::writeLine(0, title); /**< Writes the title in the first row. */
    for (short line = 1; line <= opts.size(); line++) Screen::writeLine(line, "   " + opts[line - 1].first); /**< Writes each option's text part. */
    for (short line = opts.size() + 1; line < HEIGHT; line++) Screen::writeLine(line, ""); /**< Fills all other lines */
    Screen::writeLine(arrowPos + 1, " > " + opts[arrowPos].first); /**< Rewrites the line where the arrow should be displayed, with the arrow. With well working screens, only the affacted part could be rewritten. */
  }
};

/**
 * Subclass to collect variables and display them on the screen.
 * Used to monitor algorithm values.
 * It is a singleton class.
 * @see Page
 */
class MonitoringPage : public Page
{
private:
  /**
   * Constructor.
   * @parameter title Title of the page.
   */
  MonitoringPage(std::string title) : Page(title) {}
  bool* motionStatePointer; /**< Pointer to the variable storing the motion sensor reading. Used to display the motion state. */
  unsigned long* lastTriggerTimePointer; /**< Pointer to the variable counting time since the last trigger. Used to display the seconds passed since the last trigger. */
  short* lightValuePointer; /**< Pointer to the variable storing the light sensor reading. Used to display the light value. */
protected:
  static MonitoringPage* instance; /**< Pointer to the only instance that this class should have. */
public:
  /**
   * Used to get the only instance of this class. If it does not exist yet, it is created.
   * @return The only instance of this class.
   */
  static MonitoringPage getInstance()
  {
    if(!instance) instance = new MonitoringPage("MONITORING");
    return *instance;
  }
  /**
   * Used to set the motionStatePointer.
   * @param _motionStatePointer Pointer to the variable storing the motion sensor reading.
   */
  void setMotionStatePointer(bool* _motionStatePointer) { motionStatePointer = _motionStatePointer; }
  /**
   * Used to set the lastTriggerTimePointer.
   * @param _lastTriggerTimePointer Pointer to the variable counting time since the last trigger.
   */
  void setLastTriggerTimePointer(unsigned long* _lastTriggerTimePointer) { lastTriggerTimePointer = _lastTriggerTimePointer; }
  /**
   * Used to set the lightValuePointer.
   * @param _lightValuePointer Pointer to the variable storing the light sensor reading.
   */ 
  void setLightValuePointer(short* _lightValuePointer) { lightValuePointer = _lightValuePointer; }
  /**
   * Used to refresh the dynamic information on the screen.
   */
  void refreshPage()
  {
    *motionStatePointer ? Screen::writeLine(2, "  YES") : Screen::writeLine(2, "  NO"); /**< Rewrites the line, where the motion state is displayed, depending on the value. It uses "YES" and "NO" options. */
    Screen::writeLine(4, "  " + std::to_string((unsigned long)(millis() - *lastTriggerTimePointer) / 1000) + " sec"); /**< Rewrites the line, where the last trigger time is displayed, depending on the value. It uses seconds. */
    Screen::writeLine(6, "  " + std::to_string(*lightValuePointer)); /**< Rewrites the line, where the light value is displayed, depending on the value. It uses the raw numeric value. */
  }
  /**
   * Used to display the Page on the screen.
   */
  void printPage()
  {
    Screen::clear(); /**< Clears the screen. It is done for insuring the lack of glitches on the screen. With well working screens, it can be replaced with rewriting the unused and the refreshing lines with nothing. */
    Screen::writeLine(0, title); /**< Writes the title in the first row. */
    Screen::writeLine(1, " Motion:"); /**< Writes the name "Motion" above said information. */
    Screen::writeLine(3, " Last trigger:"); /**< Writes the name "Last trigger" above said information. */
    Screen::writeLine(5, " Light value:"); /**< Writes the name "Light value" above said information. */
    Screen::writeLine(HEIGHT - 1, std::string(WIDTH - std::string("> Back").length(), ' ') + "> Back"); /**< Writes the "Back" functionality interface element with a fixed selection arrow. */
    refreshPage(); /**Uses refreshPage() to write the above mentioned pieces of information. */
  }
};
MonitoringPage* MonitoringPage::instance = 0; /**< Ensures the singleton status. */

#endif
