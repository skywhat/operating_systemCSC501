#include "sched.h"

void setschedclass(int sched_class){
  schedclass=sched_class;
}

int getschedclass(){
  return schedclass;
}
