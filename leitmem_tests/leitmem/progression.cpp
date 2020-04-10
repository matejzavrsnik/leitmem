//
// Copyright (c) 2019 Matej Zavrsnik
//
// Web:  matejzavrsnik.com
// Mail: matejzavrsnik@gmail.com
//

#include "leitmem_fixture.h"

using namespace ::testing;

TEST_F(fixture_leitmem_logic, answering_question_incorrectly_will_ask_again) 
{
   m_test_questions.add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   auto question1 = engine.get_question();
   
   engine.submit_answer("wrong answer");
   auto question2 = engine.get_question();
   
   ASSERT_EQ(question1, question2);
}

TEST_F(fixture_leitmem_logic, answering_question_incorrectly_will_ask_others_first) 
{
   m_test_questions.add_question_1();
   m_test_questions.add_question_2();
   leitmem engine(m_time_probe, m_flipcard_store);
   std::string_view question1 = engine.get_question();
   engine.submit_answer("wrong answer");
   
   std::string_view question2 = engine.get_question();
   
   ASSERT_NE(question1, question2);
}

TEST_F(fixture_leitmem_logic, answering_all_questions_incorrectly_will_start_again) 
{
   m_test_questions.add_question_1();
   m_test_questions.add_question_2();
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
   m_test_questions.add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   std::string_view question1 = engine.get_question();   
   auto keywords1 = m_test_questions.get_correct_keywords_joined(question1);
   
   engine.submit_answer(keywords1);
   
   std::string_view question2 = engine.get_question();
   
   ASSERT_EQ(question2, "No more questions.");
}

TEST_F(fixture_leitmem_logic, on_first_correct_answer_asked_again_day_after)
{
   m_test_questions.add_question_1();   
   leitmem engine(m_time_probe, m_flipcard_store);
   std::string_view question1 = engine.get_question();
   auto keywords1 = m_test_questions.get_correct_keywords_joined(question1);
   engine.submit_answer(keywords1);
   std::tm tomorrow = mzlib::tm_calc{m_today}.plus(1).days();
   ON_CALL(m_time_probe, get_today_local())
      .WillByDefault(Return(tomorrow));
   
   std::string_view question2 = engine.get_question();
   
   ASSERT_EQ(question2, question1);
}

TEST_F(fixture_leitmem_logic, on_correct_answers_progression_through_levels_works)
{
   m_test_questions.add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
     
   std::tm when_to_ask = m_today;
   
   for(auto after_days : {1,2,4,8,16,32,32,32})
   {
      // answer correctly
      std::string_view question1 = engine.get_question();
      auto keywords1 = m_test_questions.get_correct_keywords_joined(question1);
      engine.submit_answer(keywords1);
      when_to_ask = mzlib::tm_calc{when_to_ask}
         .plus(after_days).days();
         
      // a day to soon - don't get the question
      std::tm a_day_to_early = mzlib::tm_calc{when_to_ask}
         .minus(1).days();
      ON_CALL(m_time_probe, get_today_local())
         .WillByDefault(Return(a_day_to_early));
      std::string_view question2 = engine.get_question();
      ASSERT_NE(question2, question1)
          << "failed day " << after_days;
      
      // on the exact day - get the question again
      ON_CALL(m_time_probe, get_today_local())
         .WillByDefault(Return(when_to_ask));
      std::string_view question3 = engine.get_question();
      ASSERT_EQ(question3, question1)
          << "failed day " << after_days;
   }
   
}

TEST_F(fixture_leitmem_logic, when_lots_of_questions_available_prioritise_already_seen) 
{
   m_test_questions.add_question_1();
   m_test_questions.add_question_2();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.set_workset_size(1);
   auto question = engine.get_question();
   
   engine.submit_answer("wrong answer");
   auto next_question = engine.get_question();
   
   ASSERT_EQ(question, next_question);
}

TEST_F(fixture_leitmem_logic, when_lots_of_questions_available_continue_when_all_answered) 
{
   m_test_questions.add_question_1();
   m_test_questions.add_question_2();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.set_workset_size(1);   
   auto question = engine.get_question();
   auto keywords = m_test_questions.get_correct_keywords_joined(question);
   
   engine.submit_answer(keywords);
   auto next_question = engine.get_question();
   
   ASSERT_NE(question, next_question);
}