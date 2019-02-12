#ifndef UTILS_H
#define UTILS_H

#include "tools/datashelf.h"
#include "tools/time_probe.h"

#include <string_view>
#include <memory>

#include <iostream> // for debug print only

bool 
answer_is_similar_to_displayed(
   std::string_view answer, 
   std::string_view display);

bool 
evaluate_answer(
   std::string_view answer, 
   std::shared_ptr<mzlib::ds::node> flipcard);

void 
advance_level(
   mzlib::ds::pnode flipcard);

void 
reset_level(
   mzlib::ds::pnode flipcard);

double 
get_wait_time(
   mzlib::ds::pnode flipcard);

void 
mark_answered_today(
   mzlib::ds::pnode flipcard,
   mzlib::time_probe_interface& tp);

void 
mark_never_answered(
   mzlib::ds::pnode flipcard);

bool 
never_answered_correctly(
   mzlib::ds::pnode flipcard);

bool 
enough_days_passed(
   mzlib::ds::pnode flipcard,
   mzlib::time_probe_interface& time_probe);

bool 
ask_today(
   mzlib::ds::pnode flipcard,
   mzlib::time_probe_interface& time_probe);

std::string_view 
get_question_from_flipcard(
   mzlib::ds::pnode flipcard);

inline
void debug_print(std::vector<mzlib::ds::pnode> f)
{
   std::cout << f.size() << " items " << std::endl;
   for(auto a : f) 
      std::cout << mzlib::ds::get_attribute (a, "question")->value()
                << std::endl;
   std::cout << std::endl;
}

#endif /* UTILS_H */

