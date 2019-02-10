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

double 
get_wait_time(
   ds::pnode flipcard)
{
   std::string_view level = ds::get_attribute(flipcard, tag_level)->value();
   double level_num = std::strtod(level.data(), nullptr);
   return level_num;
}

void 
remember_it_was_answered_today(
   ds::pnode flipcard,
   time_probe_interface& tp)
{
   std::string today = mzlib::convert_to_string(
      tp.get_today_local(), value_date_format);
   ds::add_or_edit_attribute(flipcard, tag_answered, today);
}

std::vector<mzlib::ds::pnode> 
filter_which_to_ask_today(
   mzlib::ds::pnode flipcards_document,
   time_probe_interface& time_probe)
{
   std::vector<mzlib::ds::pnode> to_ask_today;
   
   if(!flipcards_document) return to_ask_today;
   
   std::vector<mzlib::ds::pnode> all_flipcards = ds::filter_by_name(
      flipcards_document->nodes(), tag_flipcard);
   
   for(auto flipcard : all_flipcards) 
   {
      std::string_view answered = 
         ds::get_attribute(
            flipcard, 
            tag_answered)
               ->value();
      
      if (answered.empty() || answered == value_never) {
         to_ask_today.push_back(flipcard);
      }
      else {
         double days_passed = 
            days_between(
               time_probe.get_today_local(), 
               convert_from_string(
                  answered, 
                  value_date_format, 
                  time_probe));
         
         double days_needed_to_pass = get_wait_time(flipcard);
         
         if(days_passed >= days_needed_to_pass)
            to_ask_today.push_back(flipcard);
      }
   }
   return to_ask_today;
}


