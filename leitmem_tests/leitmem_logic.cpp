//
// Copyright (c) 2019 Matej Zavrsnik
//
// Web:  matejzavrsnik.com
// Mail: matejzavrsnik@gmail.com
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "leitmem.h"
#include "constants.h"

#include "mock_flipcard_store.h"
#include "mock_time_probe.h"

#include "tools/tm_calc.h"
#include "tools/datashelf.h"

#include <string_view>

using namespace ::testing;

namespace
{

void prepare_flipcards(
   mzlib::ds::pnode flipcards, 
   std::string_view title)
{
   flipcards->set_name("flipcards");
   flipcards->add_attribute("title", title);
}

void add_flipcard(
   mzlib::ds::pnode flipcards,
   std::string_view question,
   std::string_view answer,
   std::string_view keywords)
{
   auto flipcard = flipcards->add_node("flipcard");
   flipcard->add_attribute("question", question);
   flipcard->add_node("answer", answer);
   flipcard->add_node("keywords", keywords); 
}

std::tm create_some_time()
{
   std::tm some_time;
   some_time.tm_gmtoff = 0;	
   some_time.tm_hour = 19;	
   some_time.tm_isdst = 0;	
   some_time.tm_mday = 10;	
   some_time.tm_min = 20;
   some_time.tm_mon = 1;
   some_time.tm_sec = 21;	
   some_time.tm_wday = 0;
   some_time.tm_yday = 40;	
   some_time.tm_year = 119;	
   some_time.tm_zone = "GMT";
   return some_time;
}

}

class fixture_leitmem_logic : public Test 
{
   
protected:

   fixture_leitmem_logic()
   {
      prepare_flipcards(m_flipcards, 
         "Blade Runner");
            
      ON_CALL(m_flipcard_store, load())
         .WillByDefault(Return(m_flipcards));
      
      m_today = create_some_time();
      
      ON_CALL(m_time_probe, get_today_local())
         .WillByDefault(Return(m_today));
   }
   
   ~fixture_leitmem_logic() {}

   mzlib::ds::pnode m_flipcards = std::make_shared<mzlib::ds::node>();   
   NiceMock<mock_flipcard_store> m_flipcard_store;
   NiceMock<mock_time_probe> m_time_probe; 
   
   std::tm m_today;
   
   std::string_view m_question_1 = "What are synthetic humans called?";
   std::string_view m_answer_1 = "Synthetic humans are called 'replicants'.";
   std::string_view m_keyword_1 = "replicants";
   
   std::string_view m_question_2 = "What book was Blade Runner based on?";
   std::string_view m_keyword_2 = "Do Androids Dream of Electric Sheep";
   
   
   void add_question_1()
   {
      add_flipcard(m_flipcards,
         m_question_1,
         m_answer_1,
         m_keyword_1); 
   }

   void add_question_2()
   {
      add_flipcard(m_flipcards,
         m_question_2,
         "Blade Runner was based on a book Do Androids Dream of Electric Sheep by Philip K. Dick.",
         "Do Androids Dream of Electric Sheep"); 
   }

   std::string_view get_correct_answer(std::string_view question)
   {
      if(question == m_question_1)
         return m_keyword_1;
      if(question == m_question_2)
         return m_keyword_2;
      return "";
   }
};

TEST_F(fixture_leitmem_logic, nullptr_questions_yields_no_more_questions) 
{
   NiceMock<mock_flipcard_store> store; 
   ON_CALL(store, load())
      .WillByDefault(Return(nullptr));   
   leitmem engine(m_time_probe, store);
   
   std::string_view question = engine.get_question();
   
   ASSERT_EQ(question, "No more questions.");
}

TEST_F(fixture_leitmem_logic, zero_questions_yields_no_more_questions) 
{
   NiceMock<mock_flipcard_store> store; 
   ON_CALL(store, load())
      .WillByDefault(Return(std::make_shared<mzlib::ds::node>()));
   leitmem engine(m_time_probe, store);
   
   std::string_view question = engine.get_question();
   
   ASSERT_EQ(question, "No more questions.");
}

TEST_F(fixture_leitmem_logic, can_get_question) 
{
   add_question_1();     
   leitmem engine(m_time_probe, m_flipcard_store);
   
   std::string_view question = engine.get_question();
   
   ASSERT_EQ(question, m_question_1);
}

TEST_F(fixture_leitmem_logic, can_get_answer) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   
   std::string_view question = engine.get_question();
   std::string_view answer = engine.get_answer();
   
   ASSERT_EQ(question, m_question_1);
   ASSERT_EQ(answer, m_answer_1);
}

TEST_F(fixture_leitmem_logic, recognises_correct_answer) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   bool correct = engine.submit_answer(get_correct_answer(m_question_1));
   
   ASSERT_TRUE(correct);
}

TEST_F(fixture_leitmem_logic, recognises_incorrect_answer) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   bool correct = engine.submit_answer("wrong answer");
   
   ASSERT_FALSE(correct);
}


TEST_F(fixture_leitmem_logic, on_correct_answer_adds_attribute_level) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer(get_correct_answer(m_question_1));
   engine.save_knowledge();
   
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto level = mzlib::ds::get_attribute(flipcard, tag_level)->value();
   ASSERT_EQ(level, "1");
}

TEST_F(fixture_leitmem_logic, on_correct_answer_adds_attribute_date) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer(get_correct_answer(m_question_1));
   engine.save_knowledge();
   
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto answered = mzlib::ds::get_attribute(flipcard, tag_answered)->value();
   ASSERT_EQ(answered, "2019 02 10 19:20");
}

TEST_F(fixture_leitmem_logic, on_correct_answer_updates_attribute_level) 
{
   add_question_1();
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   mzlib::ds::add_or_edit_attribute(flipcard, tag_level, "0");
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer(get_correct_answer(m_question_1));
   engine.save_knowledge();
   
   flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto level = mzlib::ds::get_attribute(flipcard, tag_level)->value();
   ASSERT_EQ(level, "1");
}

TEST_F(fixture_leitmem_logic, on_correct_answer_updates_attribute_date) 
{
   add_question_1();
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   mzlib::ds::add_or_edit_attribute(flipcard, tag_answered, value_never);
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer(get_correct_answer(m_question_1));
   engine.save_knowledge();
   
   flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto answered = mzlib::ds::get_attribute(flipcard, tag_answered)->value();
   ASSERT_EQ(answered, "2019 02 10 19:20");
}

TEST_F(fixture_leitmem_logic, on_incorrect_answer_adds_attribute_level) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer("wrong answer");
   engine.save_knowledge();
   
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto level = mzlib::ds::get_attribute(flipcard, tag_level)->value();
   ASSERT_EQ(level, "0");
}

TEST_F(fixture_leitmem_logic, on_incorrect_answer_adds_attribute_date) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer("wrong answer");
   engine.save_knowledge();
   
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto answered = mzlib::ds::get_attribute(flipcard, tag_answered)->value();
   ASSERT_EQ(answered, value_never);
}

TEST_F(fixture_leitmem_logic, on_incorrect_answer_updates_attribute_level) 
{
   add_question_1();
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   mzlib::ds::add_or_edit_attribute(flipcard, tag_level, "1");
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer("wrong answer");
   engine.save_knowledge();
   
   flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto level = mzlib::ds::get_attribute(flipcard, tag_level)->value();
   ASSERT_EQ(level, "0");
}

TEST_F(fixture_leitmem_logic, on_incorrect_answer_level_always_set_to_0) 
{
   add_question_1();
   
   for(auto before_level : {"0","1","2","4","8","16","32"})
   {
      auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
      mzlib::ds::add_or_edit_attribute(flipcard, tag_level, before_level);
      leitmem engine(m_time_probe, m_flipcard_store);
      engine.get_question();

      engine.submit_answer("wrong answer");
      engine.save_knowledge();

      flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
      auto after_level = mzlib::ds::get_attribute(flipcard, tag_level)->value();
      ASSERT_EQ(after_level, "0");
   }
}

TEST_F(fixture_leitmem_logic, on_incorrect_answer_updates_attribute_date) 
{
   add_question_1();
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   mzlib::ds::add_or_edit_attribute(flipcard, tag_answered, "some value");
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer("wrong answer");
   engine.save_knowledge();
   
   flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto answered = mzlib::ds::get_attribute(flipcard, tag_answered)->value();
   ASSERT_EQ(answered, value_never);
}

TEST_F(fixture_leitmem_logic, if_not_answered_returns_the_same_question) 
{
   add_question_1();
   add_question_2();
   leitmem engine(m_time_probe, m_flipcard_store);
   
   std::string_view question1 = engine.get_question();
   std::string_view question2 = engine.get_question();
   std::string_view question3 = engine.get_question();
   std::string_view question4 = engine.get_question();
   
   ASSERT_EQ(question1, question2);
   ASSERT_EQ(question2, question3);
   ASSERT_EQ(question3, question4);
}

TEST_F(fixture_leitmem_logic, answering_question_incorrectly_will_ask_again) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer("wrong answer");
   std::string_view question = engine.get_question();
   
   ASSERT_EQ(question, m_question_1);
}

TEST_F(fixture_leitmem_logic, answering_question_incorrectly_will_ask_others_first) 
{
   add_question_1();
   add_question_2();
   leitmem engine(m_time_probe, m_flipcard_store);
   std::string_view question1 = engine.get_question();
   engine.submit_answer("wrong answer");
   
   std::string_view question2 = engine.get_question();
   
   ASSERT_NE(question1, question2);
}

TEST_F(fixture_leitmem_logic, answering_all_questions_incorrectly_will_start_again) 
{
   add_question_1();
   add_question_2();
   leitmem engine(m_time_probe, m_flipcard_store);
   std::string_view question1 = engine.get_question();
   engine.submit_answer("wrong answer");
   std::string_view question2 = engine.get_question();
   engine.submit_answer("wrong answer");
   
   // By this point, all todays questions have been incorrectly answered.
   // It should roll over and start asking them again now.
   
   std::string_view question3 = engine.get_question();
   engine.submit_answer("wrong answer");
   std::string_view question4 = engine.get_question();
   engine.submit_answer("wrong answer");
   
   ASSERT_NE(question1, question2);
   ASSERT_NE(question3, question4);
}

TEST_F(fixture_leitmem_logic, answering_question_correctly_will_not_ask_again_immediately) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   std::string_view question1 = engine.get_question();   
   engine.submit_answer(get_correct_answer(question1));
   
   std::string_view question2 = engine.get_question();
   
   ASSERT_EQ(question2, "No more questions.");
}

TEST_F(fixture_leitmem_logic, on_first_correct_answer_asked_again_day_after)
{
   add_question_1();   
   leitmem engine(m_time_probe, m_flipcard_store);
   std::string_view question1 = engine.get_question();
   engine.submit_answer(get_correct_answer(question1));
   std::tm tomorrow = mzlib::tm_calc{m_today}.plus(1).days();
   ON_CALL(m_time_probe, get_today_local())
      .WillByDefault(Return(tomorrow));
   
   std::string_view question2 = engine.get_question();
   
   ASSERT_EQ(question2, m_question_1);
}

TEST_F(fixture_leitmem_logic, on_correct_answers_progression_through_levels_works)
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
     
   std::tm when_to_ask = m_today;
   
   for(auto after_days : {1,2,4,8,16,32,32,32})
   {
      // answer correctly
      std::string_view question1 = engine.get_question();
      engine.submit_answer(get_correct_answer(question1));
      when_to_ask = mzlib::tm_calc{when_to_ask}
         .plus(after_days).days();
         
      // a day to soon - don't get the question
      std::tm a_day_to_early = mzlib::tm_calc{when_to_ask}
         .minus(1).days();
      ON_CALL(m_time_probe, get_today_local())
         .WillByDefault(Return(a_day_to_early));
      std::string_view question2 = engine.get_question();
      ASSERT_NE(question2, m_question_1)
          << "failed day " << after_days;
      
      // on the exact day - get the question again
      ON_CALL(m_time_probe, get_today_local())
         .WillByDefault(Return(when_to_ask));
      std::string_view question3 = engine.get_question();
      ASSERT_EQ(question3, m_question_1)
          << "failed day " << after_days;
   }
   
}