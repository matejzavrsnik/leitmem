#ifndef MOCK_TIME_PROBE_H
#define MOCK_TIME_PROBE_H

#include "time_probe_interface.h"

class mock_time_probe : public time_probe_interface
{
   
public:
   
   MOCK_METHOD0(get_today_local, std::tm());
   
};

#endif /* MOCK_TIME_PROBE_H */

