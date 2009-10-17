/* Generated by Together */

#ifndef GOTOTASK_H
#define GOTOTASK_H

#include "Tasks/AbstractTask.h"
struct WAYPOINT;

class GotoTask : public AbstractTask {
public:    
  GotoTask(const TaskEvents &te, TaskAdvance &ta);
  ~GotoTask();

  virtual TaskPoint* getActiveTaskPoint();
  virtual void setActiveTaskPoint(unsigned index);
  void do_goto(const WAYPOINT & wp);

  virtual void report(const AIRCRAFT_STATE &state);

  virtual bool update_sample(const AIRCRAFT_STATE &, 
                             const bool full_update);
protected:
  virtual bool check_transitions(const AIRCRAFT_STATE &, 
                                 const AIRCRAFT_STATE&);

private:    
    TaskPoint* tp;
};

#endif //GOTOTASK_H
