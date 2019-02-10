#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string_view>

static const std::string_view tag_answered = "answered";
static const std::string_view tag_flipcard = "flipcard";
static const std::string_view tag_answer   = "answer";
static const std::string_view tag_question = "question";
static const std::string_view tag_keywords = "keywords";
static const std::string_view tag_level    = "level";

static const std::string_view value_never       = "never";
static const std::string_view value_date_format = "%Y %m %d %H:%M";

static const std::vector levels = {"0","1","2","4","8","16"};

#endif /* CONSTANTS_H */

