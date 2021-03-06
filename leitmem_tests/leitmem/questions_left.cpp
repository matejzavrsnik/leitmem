//
// Copyright (c) 2019 Matej Zavrsnik
//
// Web:  matejzavrsnik.com
// Mail: matejzavrsnik@gmail.com
//

#include "leitmem_fixture.h"

using namespace ::testing;

TEST_F(fixture_leitmem_logic, questions_left_returns_zero_when_store_empty) 
{
   NiceMock<mock_flipcard_store> store; 
   ON_CALL(store, load())
      .WillByDefault(Return(std::make_shared<mzlib::ds::node>()));
   leitmem engine(m_time_probe, store);
   
   ASSERT_EQ(engine.questions_today(), 0);
}

TEST_F(fixture_leitmem_logic, questions_left_unchanged_after_get_question) 
{
   add_flipcard(m_flipcards, test_flipcards(0));
   leitmem engine(m_time_probe, m_flipcard_store);
   
   engine.get_question();
   
   ASSERT_EQ(engine.questions_today(), 1);
}

TEST_F(fixture_leitmem_logic, questions_left_one_before_answering_only_question) 
{
   add_flipcard(m_flipcards, test_flipcards(0));
   leitmem engine(m_time_probe, m_flipcard_store);
   
   ASSERT_EQ(engine.questions_today(), 1);
}

TEST_F(fixture_leitmem_logic, questions_left_zero_after_answering_only_question) 
{
   add_flipcard(m_flipcards, test_flipcards(0));
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   engine.submit_answer(test_flipcards(0).keywords);
   
   ASSERT_EQ(engine.questions_today(), 0);
}

TEST_F(fixture_leitmem_logic, questions_left_unchanged_after_incorrect_answer) 
{
   add_flipcard(m_flipcards, test_flipcards(0));
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   engine.submit_answer("wrong answer");
   
   ASSERT_EQ(engine.questions_today(), 1);
}

TEST_F(fixture_leitmem_logic, questions_left_is_two_when_two_questions) 
{
   add_flipcard(m_flipcards, test_flipcards(0));
   add_flipcard(m_flipcards, test_flipcards(1));
   leitmem engine(m_time_probe, m_flipcard_store);
   
   ASSERT_EQ(engine.questions_today(), 2);
}

TEST_F(fixture_leitmem_logic, questions_left_decremented_after_correct_answer) 
{
   add_flipcard(m_flipcards, test_flipcards(0));
   add_flipcard(m_flipcards, test_flipcards(1));
   leitmem engine(m_time_probe, m_flipcard_store);
   std::string_view question = engine.get_question();
   std::string_view keywords = test_flipcards(question).keywords;
   
   engine.submit_answer(keywords);
   
   ASSERT_EQ(engine.questions_today(), 1);
}
