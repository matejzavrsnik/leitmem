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

TEST_F(fixture_leitmem_logic, on_correct_answer_adds_attribute_level) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer(get_correct_answer(m_question_1));
   engine.save_knowledge();
   
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto level = mzlib::ds::get_attribute(flipcard, tag_level)->value();
   ASSERT_EQ(level, "1");
}

TEST_F(fixture_leitmem_logic, on_correct_answer_adds_attribute_date) 
{
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer(get_correct_answer(m_question_1));
   engine.save_knowledge();
   
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto answered = mzlib::ds::get_attribute(flipcard, tag_answered)->value();
   ASSERT_EQ(answered, "2019 02 10 19:20");
}

TEST_F(fixture_leitmem_logic, on_correct_answer_updates_attribute_level) 
{
   add_question_1();
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   mzlib::ds::add_or_edit_attribute(flipcard, tag_level, "0");
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer(get_correct_answer(m_question_1));
   engine.save_knowledge();
   
   flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto level = mzlib::ds::get_attribute(flipcard, tag_level)->value();
   ASSERT_EQ(level, "1");
}

TEST_F(fixture_leitmem_logic, on_correct_answer_updates_attribute_date) 
{
   add_question_1();
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   mzlib::ds::add_or_edit_attribute(flipcard, tag_answered, value_never);
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer(get_correct_answer(m_question_1));
   engine.save_knowledge();
   
   flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto answered = mzlib::ds::get_attribute(flipcard, tag_answered)->value();
   ASSERT_EQ(answered, "2019 02 10 19:20");
}

TEST_F(fixture_leitmem_logic, on_incorrect_answer_adds_attribute_level) 
{
   add_question_1();
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
   add_question_1();
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer("wrong answer");
   engine.save_knowledge();
   
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto answered = mzlib::ds::get_attribute(flipcard, tag_answered)->value();
   ASSERT_EQ(answered, value_never);
}

TEST_F(fixture_leitmem_logic, on_incorrect_answer_updates_attribute_level) 
{
   add_question_1();
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
   add_question_1();
   
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
   add_question_1();
   auto flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   mzlib::ds::add_or_edit_attribute(flipcard, tag_answered, "some value");
   leitmem engine(m_time_probe, m_flipcard_store);
   engine.get_question();
   
   engine.submit_answer("wrong answer");
   engine.save_knowledge();
   
   flipcard = mzlib::ds::first(m_flipcards->nodes(), tag_flipcard);
   auto answered = mzlib::ds::get_attribute(flipcard, tag_answered)->value();
   ASSERT_EQ(answered, value_never);
}