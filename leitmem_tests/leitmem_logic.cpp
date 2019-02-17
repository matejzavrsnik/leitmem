//
// Copyright (c) 2019 Matej Zavrsnik
//
// Web:  matejzavrsnik.com
// Mail: matejzavrsnik@gmail.com
//

#include "leitmem_fixture.h"

using namespace ::testing;

// TODO: new feature: verbatim answer for phrases

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

TEST_F(fixture_leitmem_logic, get_answer_before_asking_question_doesnt_segfault) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   
   engine.get_answer();
   
   SUCCEED();
}

TEST_F(fixture_leitmem_logic, after_correct_answer_can_still_get_answer) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   engine.submit_answer(get_correct_answer(m_question_1));
   
   auto answer = engine.get_answer();
   
   ASSERT_EQ(answer, m_answer_1);
}

TEST_F(fixture_leitmem_logic, after_incorrect_answer_can_still_get_answer) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   engine.submit_answer("wrong answer");
   
   auto answer = engine.get_answer();
   
   ASSERT_EQ(answer, m_answer_1);
}
