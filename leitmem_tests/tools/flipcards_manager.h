#ifndef QUESTIONS_MANAGER_H
#define QUESTIONS_MANAGER_H

#include <vector>
#include "string/join.h"

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
   std::vector<std::string_view> keywords;
};

inline void add_flipcard(
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

inline const std::vector<flipcard_data>& test_flipcards()
{
   static std::vector<flipcard_data> flipcards
   {
      {
         .question = "What are synthetic humans called?",
         .answer   = "Synthetic humans are called 'replicants'.",
         .keywords = { "replicants" }
      },
      {
         .question = "What book was Blade Runner based on?",
         .answer   = "Blade Runner was based on a book Do Androids Dream of Electric Sheep by Philip K. Dick.",
         .keywords = { "Do Androids Dream of Electric Sheep" }
      }
   };
   return flipcards;
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
   
   void add_question_1()
   {
      add_flipcard(m_flipcards,
         test_flipcards()[0].question,
         test_flipcards()[0].answer,
         test_flipcards()[0].keywords); 
   }

   void add_question_2()
   {
      add_flipcard(m_flipcards,
         test_flipcards()[1].question,
         test_flipcards()[1].answer,
         test_flipcards()[1].keywords); 
   }
   
   std::string_view get_question_1()
   {
      return test_flipcards()[0].question;
   }
   
   std::string_view get_answer(std::string_view question)
   {
      if(question == test_flipcards()[0].question)
         return test_flipcards()[0].answer;
      if(question == test_flipcards()[1].question)
         return test_flipcards()[1].answer;
      return {""};
   }

   std::string get_correct_keywords_joined(std::string_view question)
   {
     
      if(question == test_flipcards()[0].question)
         return mzlib::join(test_flipcards()[0].keywords, " ");
      if(question == test_flipcards()[1].question)
         return mzlib::join(test_flipcards()[1].keywords, " ");
      return "";
   }
   
};

#endif /* QUESTIONS_MANAGER_H */

