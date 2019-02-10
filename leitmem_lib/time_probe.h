#ifndef TIME_PROBE_H
#define TIME_PROBE_H

#include "time_probe_interface.h"

class time_probe : public time_probe_interface
{
   
public:
   
   std::tm get_today_local() override;
};

#endif /* TIME_PROBE_H */

