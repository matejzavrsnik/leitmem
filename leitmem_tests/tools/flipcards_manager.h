#ifndef QUESTIONS_MANAGER_H
#define QUESTIONS_MANAGER_H

#include <vector>
#include "string/split.h"

inline void prepare_flipcards(
   mzlib::ds::pnode flipcards, 
   std::string_view title)
{
   flipcards->set_name("flipcards");
   flipcards->add_attribute("title", title);
}

struct flipcard_data
{
   std::string_view question;
   std::string_view answer;
   std::string_view keywords;
};

inline void add_flipcard_node(
   mzlib::ds::pnode flipcards,
   std::string_view question,
   std::string_view answer,
   std::vector<std::string_view> keywords)
{
   auto flipcard = flipcards->add_node("flipcard");
   flipcard->add_attribute("question", question);
   flipcard->add_node("answer", answer);
   for(auto keyword : keywords)
   {
      flipcard->add_node("keywords", keyword); 
   }
}

inline flipcard_data test_flipcards(int i)
{
   static std::vector<flipcard_data> flipcards
   {
      {
         .question = "What are synthetic humans called?",
         .answer   = "Synthetic humans are called 'replicants'.",
         .keywords = "replicants"
      },
      {
         .question = "What book was Blade Runner based on?",
         .answer   = "Blade Runner was based on a book Do Androids Dream of Electric Sheep by Philip K. Dick.",
         .keywords = "Do Androids Dream of Electric Sheep"
      }
   };
   return flipcards[i];
}

inline flipcard_data test_flipcards(std::string_view question)
{
   if(question == test_flipcards(0).question)
      return test_flipcards(0);
   if(question == test_flipcards(1).question)
      return test_flipcards(1);
   throw;
}


class flipcards_manager
{
   
private:
   
   mzlib::ds::pnode m_flipcards;
   
public:
   
   void work_on(mzlib::ds::pnode flipcards)
   {
      m_flipcards = flipcards;
   }
   
   void add_question(flipcard_data flipcard)
   {
      add_flipcard_node(m_flipcards,
         flipcard.question,
         flipcard.answer,
         mzlib::split(flipcard.keywords, ",")); 
   }
   
//   std::string_view get_answer(std::string_view question)
//   {
//      if(question == test_flipcards(0).question)
//         return test_flipcards(0).answer;
//      if(question == test_flipcards(1).question)
//         return test_flipcards(1).answer;
//      return {""};
//   }

//   std::string_view get_correct_keywords(std::string_view question)
//   {
//     
//      if(question == test_flipcards(0).question)
//         return test_flipcards(0).keywords;
//      if(question == test_flipcards(1).question)
//         return test_flipcards(1).keywords;
//      return "";
//   }
   
};

#endif /* QUESTIONS_MANAGER_H */

