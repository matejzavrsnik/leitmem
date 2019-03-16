//
// Copyright (c) 2019 Matej Zavrsnik
//
// Web:  matejzavrsnik.com
// Mail: matejzavrsnik@gmail.com
//

#include "leitmem_fixture.h"

using namespace ::testing;

TEST_F(fixture_leitmem_logic, all_questions_returns_zero_when_store_empty) 
{
   NiceMock<mock_flipcard_store> store; 
   ON_CALL(store, load())
      .WillByDefault(Return(std::make_shared<mzlib::ds::node>()));
   leitmem engine(m_time_probe, store);
   
   ASSERT_EQ(engine.all_questions(), 0);
}

TEST_F(fixture_leitmem_logic, all_questions_unchanged_after_get_question) 
{
   m_test_questions.add_question_1();     
   leitmem engine(m_time_probe, m_flipcard_store);
   
   engine.get_question();
   
   ASSERT_EQ(engine.all_questions(), 1);
}

TEST_F(fixture_leitmem_logic, all_questions_one_before_answering_only_question) 
{
   m_test_questions.add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   
   ASSERT_EQ(engine.all_questions(), 1);
}

TEST_F(fixture_leitmem_logic, all_questions_still_one_after_answering_only_question) 
{
   m_test_questions.add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   auto question = engine.get_question();
   engine.submit_answer(m_test_questions.get_correct_keywords(question));
   
   ASSERT_EQ(engine.all_questions(), 1);
}

TEST_F(fixture_leitmem_logic, all_questions_still_one_after_incorrect_answer) 
{
   m_test_questions.add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   engine.submit_answer("wrong answer");
   
   ASSERT_EQ(engine.all_questions(), 1);
}

TEST_F(fixture_leitmem_logic, all_questions_is_two_when_two_questions) 
{
   m_test_questions.add_question_1();
   m_test_questions.add_question_2();
   leitmem engine(m_time_probe, m_flipcard_store);
   
   ASSERT_EQ(engine.all_questions(), 2);
}

