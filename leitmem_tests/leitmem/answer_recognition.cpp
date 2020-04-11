//
// Copyright (c) 2019 Matej Zavrsnik
//
// Web:  matejzavrsnik.com
// Mail: matejzavrsnik@gmail.com
//

#include "leitmem_fixture.h"
#include "tools//converters.h"

using namespace ::testing;

TEST_F(fixture_leitmem_logic, answer_recognition_correct_keyword) 
{
   m_test_questions.add_question(test_flipcards(0));
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   bool correct = engine.submit_answer(test_flipcards(0).keywords);
   
   ASSERT_TRUE(correct);
}

TEST_F(fixture_leitmem_logic, answer_recognition_correct_keyword_ignores_case) 
{
   m_test_questions.add_question(test_flipcards(0));
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   std::string uppercase_answer = 
      mzlib::to_uppercase_copy(
         mzlib::convert<std::string>(
            test_flipcards(0).keywords));
   
   bool correct = engine.submit_answer(uppercase_answer);
   
   ASSERT_TRUE(correct);
}

TEST_F(fixture_leitmem_logic, answer_recognition_keywords_order_does_not_matters) 
{
      add_flipcard_node(m_flipcards,
         "Blade Runner cast?",
         "Main actors were Harrison Ford and Rutger Hauer.",
         {"Harrison Ford, Rutger Hauer"}); 
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   bool correct = engine.submit_answer("Rutger Hauer and Harrison Ford");
   
   ASSERT_TRUE(correct);
}

TEST_F(fixture_leitmem_logic, answer_recognition_keywords_trimmed_of_spaces) 
{
   add_flipcard_node(m_flipcards,
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
      add_flipcard_node(m_flipcards,
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
   m_test_questions.add_question(test_flipcards(0));
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   bool correct = engine.submit_answer("wrong answer");
   
   ASSERT_FALSE(correct);
}

TEST_F(fixture_leitmem_logic, answer_recognition_alternative_keywords_first_accepted)
{
   add_flipcard_node(m_flipcards,
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
   add_flipcard_node(m_flipcards,
      "What did Britain export to the rest of the Roman Empire?",
      "Britain was rich in reserves of metals and exported lead, silver, tin and iron.",
      {"metals", "lead, silver, tin, iron"});
   
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();

   bool correct = engine.submit_answer("lead, silver, tin, iron");

   ASSERT_TRUE (correct);
}

TEST_F(fixture_leitmem_logic, answer_recognition_superstring_of_keyword_cant_be_correct_answer)
{
   add_flipcard_node(m_flipcards,
      "Word for bearing provocation",
      "...",
      {"patient"});
   
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();

   bool correct = engine.submit_answer("impatient");

   ASSERT_FALSE (correct);
}

TEST_F(fixture_leitmem_logic, answer_recognition_substring_of_keyword_cant_be_correct_answer)
{
   add_flipcard_node(m_flipcards,
      "Restless or short of temper especially under irritation",
      "...",
      {"impatient"});
   
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();

   bool correct = engine.submit_answer("patient");

   ASSERT_FALSE (correct);
}