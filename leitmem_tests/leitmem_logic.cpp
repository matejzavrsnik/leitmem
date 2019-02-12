/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

//
// Copyright (c) 2019 Matej Zavrsnik
//
// Web:  matejzavrsnik.com
// Mail: matejzavrsnik@gmail.com
//

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "leitmem.h"

#include "mock_flipcard_store.h"
#include "mock_time_probe.h"

#include "tools/tm_calc.h"

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

std::string_view get_correct_answer(std::string_view question)
{
   if(question == "What are synthetic humans called?")
      return "replicants";
   if(question == "What book was Blade Runner based on?")
      return "Do Androids Dream of Electric Sheep";
   return "";
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
      
      today = create_some_time();
      
      ON_CALL(m_time_probe, get_today_local())
         .WillByDefault(Return(today));
      
   }
   
   ~fixture_leitmem_logic() {}
   
   NiceMock<mock_flipcard_store> m_flipcard_store; 
   mzlib::ds::pnode m_flipcards = std::make_shared<mzlib::ds::node>();
   
   NiceMock<mock_time_probe> m_time_probe; 
   
   std::tm today;
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

TEST_F(fixture_leitmem_logic, one_question_can_get_question) 
{
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
   std::string_view question = engine.get_question();
   
   ASSERT_EQ(question, "What are synthetic humans called?");
}

TEST_F(fixture_leitmem_logic, one_question_can_get_answer) 
{
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
   std::string_view question = engine.get_question();
   std::string_view answer = engine.get_answer();
   
   ASSERT_EQ(question, "What are synthetic humans called?");
   ASSERT_EQ(answer, "Synthetic humans are called 'replicants'.");
}



TEST_F(fixture_leitmem_logic, one_question_recognises_correct_answer) 
{
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
   
   /*std::string_view question =*/ engine.get_question();
   bool correct = engine.submit_answer("replicants");
   
   ASSERT_TRUE(correct);
}

TEST_F(fixture_leitmem_logic, one_question_recognises_incorrect_answer) 
{
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
   /*std::string_view question =*/ engine.get_question();
   bool correct = engine.submit_answer("wrong answer");
   
   ASSERT_FALSE(correct);
}

TEST_F(fixture_leitmem_logic, if_not_answered_returns_the_same_question) 
{
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants"); 
   
   add_flipcard(m_flipcards,
      "What book was Blade Runner based on?",
      "Blade Runner was based on a book Do Androids Dream of Electric Sheep by Philip K. Dick.",
      "Do Androids Dream of Electric Sheep"); 
   
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
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
   
   /*std::string_view question =*/ engine.get_question();
   
   engine.submit_answer("wrong answer");
   
   std::string_view question = engine.get_question();
   
   ASSERT_EQ(question, "What are synthetic humans called?");
}

TEST_F(fixture_leitmem_logic, answering_question_incorrectly_will_ask_others_first) 
{
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants");
   
   add_flipcard(m_flipcards,
      "What book was Blade Runner based on?",
      "Blade Runner was based on a book Do Androids Dream of Electric Sheep by Philip K. Dick.",
      "Do Androids Dream of Electric Sheep"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
   
   std::string_view question1 = engine.get_question();
   
   engine.submit_answer("wrong answer");
   
   std::string_view question2 = engine.get_question();
   
   ASSERT_NE(question1, question2);
}

TEST_F(fixture_leitmem_logic, answering_all_questions_incorrectly_will_start_again) 
{
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants");
   
   add_flipcard(m_flipcards,
      "What book was Blade Runner based on?",
      "Blade Runner was based on a book Do Androids Dream of Electric Sheep by Philip K. Dick.",
      "Do Androids Dream of Electric Sheep"); 
   
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
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);

   std::string_view question1 = engine.get_question();   
   
   engine.submit_answer(get_correct_answer(question1));
   
   std::string_view question2 = engine.get_question();
   
   ASSERT_NE(question2, "What are synthetic humans called?");
}

TEST_F(fixture_leitmem_logic, on_first_correct_answer_asked_again_day_after)
{
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
   
   std::string_view question1 = engine.get_question();
   
   engine.submit_answer(get_correct_answer(question1));
   
   std::tm tomorrow = mzlib::tm_calc{today}.plus(1).days();
   
   ON_CALL(m_time_probe, get_today_local())
      .WillByDefault(Return(tomorrow));
   
   std::string_view question2 = engine.get_question();
   
   ASSERT_EQ(question2, "What are synthetic humans called?");
}

TEST_F(fixture_leitmem_logic, on_correct_answers_progression_through_levels_works)
{
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
     
   std::tm when_to_ask = today;
   
   for(auto after_days : {1,2,4,8,16,32})
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
      ASSERT_NE(question2, "What are synthetic humans called?")
          << "failed day " << after_days;
      
      // on the exact day - get the question again
      ON_CALL(m_time_probe, get_today_local())
         .WillByDefault(Return(when_to_ask));
      std::string_view question3 = engine.get_question();
      ASSERT_EQ(question3, "What are synthetic humans called?")
          << "failed day " << after_days;
   }
   
}