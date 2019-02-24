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

TEST_F(fixture_leitmem_logic, answer_recognition_keywords_order_does_not_matters) 
{
      add_flipcard(m_flipcards,
         "Blade Runner cast?",
         "Main actors were Harrison Ford and Rutger Hauer.",
         {"Harrison Ford, Rutger Hauer"}); 
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   bool correct = engine.submit_answer("Harrison Ford and Rutger Hauer");
   
   ASSERT_TRUE(correct);
}

TEST_F(fixture_leitmem_logic, answer_recognition_keywords_trimmed_of_spaces) 
{
   add_flipcard(m_flipcards,
      "Blade Runner cast?",
      "Main actors were Harrison Ford and Rutger Hauer.",
      // keywords: space after first, two spaces before second
      {"Harrison Ford ,  Rutger Hauer"}); 
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   // answer: no space after first, only one space after second
   // no way to match unless keywords are trimmed
   bool correct = engine.submit_answer("Harrison Ford, Rutger Hauer");
   
   ASSERT_TRUE(correct);
}

TEST_F(fixture_leitmem_logic, answer_recognition_word_order_of_keyword_itself_matters) 
{
      add_flipcard(m_flipcards,
         "What book was Blade Runner based on?",
         "Blade Runner was based on a book Do Androids Dream of Electric Sheep by Philip K. Dick.",
         {"Do Androids Dream of Electric Sheep"}); 
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   bool correct = engine.submit_answer("Do Electric Sheep Dream of Androids?");
   
   ASSERT_FALSE(correct);
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