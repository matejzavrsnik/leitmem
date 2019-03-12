#ifndef QUESTIONS_MANAGER_H
#define QUESTIONS_MANAGER_H

#include <vector>


inline void prepare_flipcards(
   mzlib::ds::pnode flipcards, 
   std::string_view title)
{
   flipcards->set_name("flipcards");
   flipcards->add_attribute("title", title);
}

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

struct flipcard_data
{
   std::string question;
   std::string answer;
   std::string keywords;
   std::string user_answer;
};

class flipcards_manager
{

   std::string_view m_question_1 = "What are synthetic humans called?";
   std::string_view m_answer_1 = "Synthetic humans are called 'replicants'.";
   std::string_view m_keyword_1 = "replicants";
   
   std::string_view m_question_2 = "What book was Blade Runner based on?";
   std::string_view m_answer_2 = "Synthetic humans are called 'replicants'.";
   std::string_view m_keyword_2 = "Do Androids Dream of Electric Sheep";
   
   mzlib::ds::pnode m_flipcards;
   
   std::vector<flipcard_data> m_flipcard_data;
   
public:
   
   void add_questions(int number)
   {
      flipcard_data f;
      m_flipcard_data.push_back(f);
   }
   
   void work_on(mzlib::ds::pnode flipcards)
   {
      m_flipcards = flipcards;
   }
   
   void add_question_1()
   {
      add_flipcard(m_flipcards,
         m_question_1,
         m_answer_1,
         {m_keyword_1}); 
   }

   void add_question_2()
   {
      add_flipcard(m_flipcards,
         m_question_2,
         "Blade Runner was based on a book Do Androids Dream of Electric Sheep by Philip K. Dick.",
         {"Do Androids Dream of Electric Sheep"}); 
   }
   
   std::string_view get_question_1()
   {
      return m_question_1;
   }
   
   std::string_view get_answer(std::string_view question)
   {
      if(question == m_question_1)
         return m_answer_1;
      if(question == m_question_2)
         return m_answer_2;
      return "";
   }

   std::string_view get_correct_keywords(std::string_view question)
   {
      if(question == m_question_1)
         return m_keyword_1;
      if(question == m_question_2)
         return m_keyword_2;
      return "";
   }
   
};

#endif /* QUESTIONS_MANAGER_H */

