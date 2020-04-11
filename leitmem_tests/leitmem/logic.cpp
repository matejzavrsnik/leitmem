//
// Copyright (c) 2019 Matej Zavrsnik
//
// Web:  matejzavrsnik.com
// Mail: matejzavrsnik@gmail.com
//

#include "leitmem_fixture.h"

using namespace ::testing;

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
   add_question(test_flipcards(0), m_flipcards);
   leitmem engine(m_time_probe, m_flipcard_store);
   
   std::string_view question = engine.get_question();
   
   ASSERT_EQ(question, test_flipcards(0).question);
}

TEST_F(fixture_leitmem_logic, can_get_answer) 
{
   add_question(test_flipcards(0), m_flipcards);
   leitmem engine(m_time_probe, m_flipcard_store);
   
   engine.get_question();
   std::string_view answer = engine.get_answer();
   
   ASSERT_EQ(answer, test_flipcards(0).answer);
}

TEST_F(fixture_leitmem_logic, get_answer_before_asking_question_doesnt_segfault) 
{
   add_question(test_flipcards(0), m_flipcards);
   leitmem engine(m_time_probe, m_flipcard_store);
   
   engine.get_answer();
   
   SUCCEED();
}

TEST_F(fixture_leitmem_logic, after_correct_answer_can_still_get_answer) 
{
   add_question(test_flipcards(0), m_flipcards);
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer(test_flipcards(0).keywords);
   
   auto answer = engine.get_answer();
   
   ASSERT_EQ(answer, test_flipcards(0).answer);
}

TEST_F(fixture_leitmem_logic, after_incorrect_answer_can_still_get_answer) 
{
   add_question(test_flipcards(0), m_flipcards);
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   engine.submit_answer("wrong answer");
   
   auto answer = engine.get_answer();
   
   ASSERT_EQ(answer, test_flipcards(0).answer);
}
