#ifndef UTILS_H
#define UTILS_H

#include "tools/datashelf.h"

#include <string_view>
#include <memory>

bool answer_is_similar_to_displayed(std::string_view answer, std::string_view display);

bool evaluate_answer(std::string_view answer, std::shared_ptr<mzlib::ds::node> flipcard);

void advance_level(mzlib::ds::pnode flipcard);

double get_wait_time(mzlib::ds::pnode flipcard);

void remember_it_was_answered_today(mzlib::ds::pnode flipcard);

std::string convert_to_string(std::tm date);
std::tm convert_from_string(std::string_view date);
std::tm convert_to_local_time(std::tm utc_time);
double days_between(std::tm from, std::tm to);
std::tm get_today_local();

#endif /* UTILS_H */

