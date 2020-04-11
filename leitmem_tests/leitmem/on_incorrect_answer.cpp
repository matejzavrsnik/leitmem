//
// Copyright (c) 2019 Matej Zavrsnik
//
// Web:  matejzavrsnik.com
// Mail: matejzavrsnik@gmail.com
//

#include "leitmem_fixture.h"

using namespace ::testing;

TEST_F(fixture_leitmem_logic, on_incorrect_answer_adds_attribute_level) 
{
   add_flipcard(m_flipcards, test_flipcards(0));
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer("wrong answer");
   engine.save_knowledge();
   
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto level = mzlib::ds::get_attribute(flipcard, tag_level)->value();
   ASSERT_EQ(level, "0");
}

TEST_F(fixture_leitmem_logic, on_incorrect_answer_adds_attribute_date) 
{
   add_flipcard(m_flipcards, test_flipcards(0));
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer("wrong answer");
   engine.save_knowledge();
   
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto answered = mzlib::ds::get_attribute(flipcard, tag_answered)->value();
   ASSERT_EQ(answered, value_incorrectly);
}

TEST_F(fixture_leitmem_logic, on_incorrect_answer_updates_attribute_level) 
{
   add_flipcard(m_flipcards, test_flipcards(0));
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   mzlib::ds::add_or_edit_attribute(flipcard, tag_level, "1");
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer("wrong answer");
   engine.save_knowledge();
   
   flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto level = mzlib::ds::get_attribute(flipcard, tag_level)->value();
   ASSERT_EQ(level, "0");
}

TEST_F(fixture_leitmem_logic, on_incorrect_answer_level_always_set_to_0) 
{
   add_flipcard(m_flipcards, test_flipcards(0));
   
   for(auto before_level : {"0","1","2","4","8","16","32"})
   {
      auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
      mzlib::ds::add_or_edit_attribute(flipcard, tag_level, before_level);
      leitmem engine(m_time_probe, m_flipcard_store);
      engine.get_question();

      engine.submit_answer("wrong answer");
      engine.save_knowledge();

      flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
      auto after_level = mzlib::ds::get_attribute(flipcard, tag_level)->value();
      ASSERT_EQ(after_level, "0");
   }
}

TEST_F(fixture_leitmem_logic, on_incorrect_answer_updates_attribute_date) 
{
   add_flipcard(m_flipcards, test_flipcards(0));
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   mzlib::ds::add_or_edit_attribute(flipcard, tag_answered, "some value");
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer("wrong answer");
   engine.save_knowledge();
   
   flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto answered = mzlib::ds::get_attribute(flipcard, tag_answered)->value();
   ASSERT_EQ(answered, value_incorrectly);
}