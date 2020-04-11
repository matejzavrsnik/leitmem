//
// Copyright (c) 2019 Matej Zavrsnik
//
// Web:  matejzavrsnik.com
// Mail: matejzavrsnik@gmail.com
//

#include "leitmem_fixture.h"

using namespace ::testing;

TEST_F(fixture_leitmem_logic, on_correct_answer_adds_attribute_level) 
{
   add_question(test_flipcards(0), m_flipcards);
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer(test_flipcards(0).keywords);
   engine.save_knowledge();
   
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto level = mzlib::ds::get_attribute(flipcard, tag_level)->value();
   ASSERT_EQ(level, "1");
}

TEST_F(fixture_leitmem_logic, on_correct_answer_adds_attribute_date) 
{
   add_question(test_flipcards(0), m_flipcards);
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer(test_flipcards(0).keywords);
   engine.save_knowledge();
   
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto answered = mzlib::ds::get_attribute(flipcard, tag_answered)->value();
   ASSERT_EQ(answered, "2019 02 10 19:20");
}

TEST_F(fixture_leitmem_logic, on_correct_answer_updates_attribute_level) 
{
   add_question(test_flipcards(0), m_flipcards);
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   mzlib::ds::add_or_edit_attribute(flipcard, tag_level, "0");
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer(test_flipcards(0).keywords);
   engine.save_knowledge();
   
   flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto level = mzlib::ds::get_attribute(flipcard, tag_level)->value();
   ASSERT_EQ(level, "1");
}

TEST_F(fixture_leitmem_logic, on_correct_answer_updates_attribute_date) 
{
   add_question(test_flipcards(0), m_flipcards);
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   mzlib::ds::add_or_edit_attribute(flipcard, tag_answered, value_incorrectly);
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer(test_flipcards(0).keywords);
   engine.save_knowledge();
   
   flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto answered = mzlib::ds::get_attribute(flipcard, tag_answered)->value();
   ASSERT_EQ(answered, "2019 02 10 19:20");
}
