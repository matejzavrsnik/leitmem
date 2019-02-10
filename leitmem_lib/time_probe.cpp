#include "time_probe.h"

std::tm time_probe::get_today_local()
{
   std::time_t today = std::time(nullptr);
   return *std::localtime(&today);
}