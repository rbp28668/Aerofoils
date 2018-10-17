#ifndef STEPPER_H
#define STEPPER_H

class Stepper {

private:
  long _steps; // total number of steps this will be called for.
  long _dist;  // distance to move in those number of steps
  long _count; // of steps made
  long _err;   // current error value for bresenham

public:
  Stepper();
  
  // Set up for doing dist pulses over a a total of steps calls to step().
  void start(long dist, long steps);

  // True if not finished (i.e. haven't yet done steps calls)
  bool isRunning();

  // Does a single step.  Returns true if the stepper should be moved.
  bool step();

  // Terminates the algorithm
  void stop(); 
};

#endif

