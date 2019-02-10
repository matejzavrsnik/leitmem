#ifndef UTILS_H
#define UTILS_H

#include "tools/datashelf.h"
#include "tools/time_probe.h"

#include <string_view>
#include <memory>

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

double 
get_wait_time(
   mzlib::ds::pnode flipcard);

void 
remember_it_was_answered_today(
   mzlib::ds::pnode flipcard,
   mzlib::time_probe_interface& tp);

std::vector<mzlib::ds::pnode> 
filter_which_to_ask_today(
   mzlib::ds::pnode all_flipcards,
   mzlib::time_probe_interface& tp);

#endif /* UTILS_H */

