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

#include <string_view>

using namespace ::testing;

class fixture_leitmem : public Test 
{
   
protected:

   fixture_leitmem() 
   {
      ON_CALL(m_flipcard_store, load())
         .WillByDefault(Return(m_flipcards));
   }
   
   ~fixture_leitmem() {}
   
   NiceMock<mock_flipcard_store> m_flipcard_store; 
   mzlib::ds::pnode m_flipcards = std::make_shared<mzlib::ds::node>();
};


TEST_F(fixture_leitmem, zero_questions_yields_no_more_questions) 
{
   leitmem engine(m_flipcard_store);
   std::string_view question = engine.get_next_question();
   
   ASSERT_EQ(question, "No more questions.");
}

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

TEST_F(fixture_leitmem, one_question_can_get_next_question) 
{
   prepare_flipcards(m_flipcards, 
      "Blade Runner");
   
   add_flipcard(m_flipcards,
      "What are synthetic humans called?",
      "Synthetic humans are called 'replicants'.",
      "replicants"); 
      
   leitmem engine(m_flipcard_store);
   std::string_view question = engine.get_next_question();
   
   ASSERT_EQ(question, "What are synthetic humans called?");
}