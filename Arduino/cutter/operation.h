#ifndef OPERATION_H
#define OPERATION_H
class Operation {
public:

  virtual char* name();
  virtual bool isRunning();
  virtual void step();
  virtual void stop();
};
#endif


