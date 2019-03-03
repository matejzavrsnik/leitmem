//
// Copyright (c) 2019 Matej Zavrsnik
//
// Web:  matejzavrsnik.com
// Mail: matejzavrsnik@gmail.com
//

#include "leitmem_fixture.h"

using namespace ::testing;

TEST_F(fixture_leitmem_logic, will_ignore_flipcards_that_are_not_flipcard) 
{
   auto flipcard = m_flipcards->add_node("not_flipcard");
   
   leitmem engine(m_time_probe, m_flipcard_store);
   std::string_view question = engine.get_question();
   
   ASSERT_EQ(question, "No more questions.");
}

TEST_F(fixture_leitmem_logic, will_ignore_flipcards_without_question_tag) 
{
   auto flipcard = m_flipcards->add_node("flipcard");
   flipcard->add_node("answer", "a");
   flipcard->add_node("keywords", "k"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
   std::string_view question = engine.get_question();
   
   ASSERT_EQ(question, "No more questions.");
}

TEST_F(fixture_leitmem_logic, will_ignore_flipcards_without_answer_tag) 
{
   auto flipcard = m_flipcards->add_node("flipcard");
   flipcard->add_attribute("question", "q");
   flipcard->add_node("keywords", "k"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
   std::string_view question = engine.get_question();
   
   ASSERT_EQ(question, "No more questions.");
}

TEST_F(fixture_leitmem_logic, will_ignore_flipcards_without_keyword_tag) 
{
   auto flipcard = m_flipcards->add_node("flipcard");
   flipcard->add_attribute("question", "q");
   flipcard->add_node("answer", "a"); 
   
   leitmem engine(m_time_probe, m_flipcard_store);
   std::string_view question = engine.get_question();
   
   ASSERT_EQ(question, "No more questions.");
}



