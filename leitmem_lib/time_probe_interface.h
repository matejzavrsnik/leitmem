#ifndef TIME_PROBE_INTERFACE_H
#define TIME_PROBE_INTERFACE_H

#include <ctime>

class time_probe_interface
{
   
public:
   
   virtual std::tm get_today_local() = 0;
   
};

#endif /* TIME_PROBE_INTERFACE_H */

