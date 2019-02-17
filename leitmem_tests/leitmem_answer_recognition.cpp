//
// Copyright (c) 2019 Matej Zavrsnik
//
// Web:  matejzavrsnik.com
// Mail: matejzavrsnik@gmail.com
//

#include "leitmem_fixture.h"

using namespace ::testing;

TEST_F(fixture_leitmem_logic, answer_recognition_correct_keyword) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   bool correct = engine.submit_answer(get_correct_answer(m_question_1));
   
   ASSERT_TRUE(correct);
}

TEST_F(fixture_leitmem_logic, answer_recognition_correct_keyword_ignores_case) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   std::string uppercase_answer = 
      mzlib::to_uppercase_copy<std::string>(
         get_correct_answer(m_question_1));
   
   bool correct = engine.submit_answer(uppercase_answer);
   
   ASSERT_TRUE(correct);
}

TEST_F(fixture_leitmem_logic, answer_recognition_incorrect_keyword) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   bool correct = engine.submit_answer("wrong answer");
   
   ASSERT_FALSE(correct);
}

TEST_F(fixture_leitmem_logic, answer_recognition_alternative_keywords_first_accepted)
{
   add_flipcard(m_flipcards,
      "What did Britain export to the rest of the Roman Empire?",
      "Britain was rich in reserves of metals and exported lead, silver, tin and iron.",
      {"metals", "lead, silver, tin, iron"});
   
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();

   bool correct = engine.submit_answer("metals");

   ASSERT_TRUE (correct);
}

TEST_F(fixture_leitmem_logic, answer_recognition_alternative_keywords_second_accepted)
{
   add_flipcard(m_flipcards,
      "What did Britain export to the rest of the Roman Empire?",
      "Britain was rich in reserves of metals and exported lead, silver, tin and iron.",
      {"metals", "lead, silver, tin, iron"});
   
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();

   bool correct = engine.submit_answer("lead, silver, tin, iron");

   ASSERT_TRUE (correct);
}