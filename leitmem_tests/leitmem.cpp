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

#include <string_view>

using namespace ::testing;

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

class fixture_leitmem : public Test 
{
   
protected:

   fixture_leitmem() 
   {
      ON_CALL(m_flipcard_store, load())
         .WillByDefault(Return(m_flipcards));
      
      prepare_flipcards(m_flipcards, 
         "Blade Runner");
      
      ON_CALL(m_time_probe, get_today_local())
         .WillByDefault(Return(today));
   }
   
   ~fixture_leitmem() {}
   
   NiceMock<mock_flipcard_store> m_flipcard_store; 
   mzlib::ds::pnode m_flipcards = std::make_shared<mzlib::ds::node>();
   
   NiceMock<mock_time_probe> m_time_probe; 
   std::tm today;
};

TEST_F(fixture_leitmem, nullptr_questions_yields_no_more_questions) 
{
   NiceMock<mock_flipcard_store> store; 
   ON_CALL(store, load())
      .WillByDefault(Return(nullptr));
   
   leitmem engine(m_time_probe, store);
   std::string_view question = engine.get_next_question();
   
   ASSERT_EQ(question, "No more questions.");
}

TEST_F(fixture_leitmem, zero_questions_yields_no_more_questions) 
{
   NiceMock<mock_flipcard_store> store; 
   ON_CALL(store, load())
      .WillByDefault(Return(std::make_shared<mzlib::ds::node>()));
   
   leitmem engine(m_time_probe, store);
   std::string_view question = engine.get_next_question();
   
   ASSERT_EQ(question, "No more questions.");
}

TEST_F(fixture_leitmem, one_question_can_get_next_question) 
{
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
   std::string_view question = engine.get_next_question();
   
   ASSERT_EQ(question, "What are synthetic humans called?");
}

TEST_F(fixture_leitmem, one_question_can_get_answer) 
{
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
   std::string_view question = engine.get_next_question();
   std::string_view answer = engine.get_answer(question);
   
   ASSERT_EQ(answer, "Synthetic humans are called 'replicants'.");
}

TEST_F(fixture_leitmem, one_question_recognises_correct_answer) 
{
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
   bool is_correct = engine.submit_answer(
      "What are synthetic humans called?",
      "replicants");
   
   ASSERT_TRUE(is_correct);
}

TEST_F(fixture_leitmem, one_question_recognises_incorrect_answer) 
{
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
   bool is_correct = engine.submit_answer(
      "What are synthetic humans called?",
      "synths");
   
   ASSERT_FALSE(is_correct);
}

TEST_F(fixture_leitmem, answering_question_incorrectly_will_ask_again) 
{
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.submit_answer(
      "What are synthetic humans called?",
      "synths");
   
   std::string_view question = engine.get_next_question();
   
   ASSERT_EQ(question, "What are synthetic humans called?");
}

TEST_F(fixture_leitmem, answering_question_correctly_will_not_ask_again_immediately) 
{
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.submit_answer(
      "What are synthetic humans called?",
      "replicants");
   
   std::string_view question = engine.get_next_question();
   
   ASSERT_NE(question, "What are synthetic humans called?");
}
