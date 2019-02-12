#include "utils.h"
#include "constants.h"

#include "string/case.h"
#include "string/all_words_appear.h"
#include "tools/wagner_fischer_distance.h"
#include "tools/time_probe.h"
#include "tools/time_operations.h"
#include "iterators/is_last.h"

using namespace std;
using namespace mzlib;

bool 
answer_is_similar_to_displayed(
   string_view answer, 
   string_view display)
{
   int difference = wagner_fischer_distance(answer, display);
   double percent_off = (double)difference / (double)display.length();
   return percent_off < 0.3;
}

bool 
evaluate_answer(
   string_view answer, 
   ds::pnode flipcard)
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

void 
advance_level(
   ds::pnode flipcard)
{
   std::string_view level = ds::get_or_add_attribute(
      flipcard, tag_level, levels[0])->value();

   auto level_it = std::find(levels.begin(), levels.end(), level);
   if (!mzlib::is_last(level_it, levels))
      level_it = std::next(level_it);
   ds::add_or_edit_attribute(flipcard, tag_level, *level_it);   
}

void 
reset_level(
   mzlib::ds::pnode flipcard)
{
   ds::add_or_edit_attribute(flipcard, tag_level, levels[0]);
}

double 
get_wait_time(
   ds::pnode flipcard)
{
   std::string_view level = ds::get_attribute(flipcard, tag_level)->value();
   double level_num = std::strtod(level.data(), nullptr);
   return level_num;
}

void 
mark_answered_today(
   ds::pnode flipcard,
   time_probe_interface& time_probe)
{
   std::string today = mzlib::convert_to_string(
      time_probe.get_today_local(), 
      value_date_format);
   
   ds::add_or_edit_attribute(flipcard, tag_answered, today);
}

void 
mark_never_answered(
   mzlib::ds::pnode flipcard)
{
   ds::add_or_edit_attribute(flipcard, tag_answered, value_never);
}

bool never_answered_correctly(mzlib::ds::pnode flipcard)
{
   std::string_view answered = 
      ds::get_attribute(
         flipcard, 
         tag_answered)
            ->value();

   return (answered.empty() || answered == value_never);
}

bool enough_days_passed(
   mzlib::ds::pnode flipcard, 
   time_probe_interface& time_probe)
{
   std::string_view answered = 
      ds::get_attribute(
         flipcard, 
         tag_answered)
            ->value();
   
   std::tm when_aswered = convert_from_string(
      answered, 
      value_date_format, 
      time_probe);
   
   std::tm today = time_probe.get_today_local();
   
   double days_passed = days_between(when_aswered, today);

   const double days_needed_to_pass = get_wait_time(flipcard);

   return (days_passed >= days_needed_to_pass);
}

bool 
ask_today(
   mzlib::ds::pnode flipcard, 
   time_probe_interface& time_probe)
{
   if (never_answered_correctly(flipcard))
      return true;

   if (enough_days_passed(flipcard, time_probe))
      return true;

   return false;
}

std::string_view 
get_question_from_flipcard(
   mzlib::ds::pnode flipcard)
{
   auto question = ds::get_attribute (flipcard, tag_question)->value();
   return question;
}