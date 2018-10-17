#ifndef HOME_HANDLER_H
#define HOME_HANDLER_H

#include "command_handler.h"
#include "home.h"

// * H - home
struct homeT : public commandT{
};

/*
 * CommandHandler to run the Home command (i.e. drive
 * all the steppers to their home positions.)
 */
class HomeHandler : public CommandHandler {
private:
  Home* _home;
public:
  HomeHandler(Home* _home);
  virtual size_t getMessageSize() { return sizeof(homeT); }
  virtual bool parseInto(char* msg, byte* buffer);
  virtual Operation* process(byte* msg);
  virtual char* getDescription();
};


#endif
