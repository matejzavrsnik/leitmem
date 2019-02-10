#ifndef MOCK_TIME_PROBE_H
#define MOCK_TIME_PROBE_H

#include "tools/time_probe.h"

class mock_time_probe : public mzlib::time_probe_interface
{
   
public:
   
   MOCK_CONST_METHOD0(get_today_local, std::tm());
   
};

#endif /* MOCK_TIME_PROBE_H */

