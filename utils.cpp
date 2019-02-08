#include "utils.h"
#include "constants.h"

#include "string/case.h"
#include "tools/wagner_fischer_distance.h"
#include "string/all_words_appear.h"
#include "nature/units.h"
#include "iterators/is_last.h"

#include <iomanip>
#include <cstdlib>

using namespace std;
using namespace mzlib;

bool answer_is_similar_to_displayed(string_view answer, string_view display)
{
   int difference = wagner_fischer_distance(answer, display);
   double percent_off = (double)difference / (double)display.length();
   return percent_off < 0.3;
}

bool evaluate_answer(string_view answer, ds::pnode flipcard)
{
   string_view mixed_case_flipcard_answer = ds::first(flipcard->nodes(), tag_answer)->value();
   string_view mixed_case_flipcard_keywords = ds::first(flipcard->nodes(), tag_keywords)->value();
   
   string user_answer = mzlib::to_lowercase_copy<string>(answer);
   string flipcard_answer = mzlib::to_lowercase_copy<string>(mixed_case_flipcard_answer);
   string flipcard_keywords = mzlib::to_lowercase_copy<string>(mixed_case_flipcard_keywords);
      
   if (user_answer.size() == 0)
      return false;
    
   if (mzlib::all_words_appear(flipcard_keywords, user_answer))
      return true;
    
   if (answer_is_similar_to_displayed(user_answer, flipcard_answer))
      return true;
    
   // not correct otherwise
   return false;
}

void advance_level(ds::pnode flipcard)
{
   std::string_view level = ds::get_or_add_attribute(
      flipcard, tag_level, levels[0])->value();

   auto level_it = std::find(levels.begin(), levels.end(), level);
   if (!mzlib::is_last(level_it, levels))
      level_it = std::next(level_it);
   ds::add_or_edit_attribute(flipcard, tag_level, *level_it);   
}

double get_wait_time(ds::pnode flipcard)
{
   std::string_view level = ds::get_attribute(flipcard, tag_level)->value();
   double level_num = std::strtod(level.data(), nullptr);
   return level_num;
}

void remember_it_was_answered_today(ds::pnode flipcard)
{
   std::string today = convert_to_string(get_today_local());
   ds::add_or_edit_attribute(flipcard, tag_answered, today);
}


std::string convert_to_string(std::tm date)
{
   std::stringstream ss;
   ss << std::put_time(&date, value_date_format.data());
   std::string today = ss.str();
   return today;
}

std::tm convert_from_string(string_view date)
{
   std::istringstream ss(date.data());
   // assume string represents local time zone
   std::tm today = get_today_local();
   // then change what string says
   ss >> std::get_time(&today, value_date_format.data());
   return today;
}

std::tm convert_to_local_time(std::tm utc_time)
{
   std::time_t utc_time_t = mktime(&utc_time);
   return *std::localtime(&utc_time_t);
}

double days_between(std::tm time_end, std::tm time_beg)
{
   using namespace units;
   double passed_time = std::difftime(mktime(&time_end), mktime(&time_beg)); // todo: really?
   double days = passed_time / 1.0_day;
   return days;
}

std::tm get_today_local()
{
   std::time_t today = std::time(nullptr);
   return *std::localtime(&today);
}