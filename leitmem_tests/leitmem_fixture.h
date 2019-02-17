//
// Copyright (c) 2019 Matej Zavrsnik
//
// Web:  matejzavrsnik.com
// Mail: matejzavrsnik@gmail.com
//

#ifndef LEITMEM_FIXTURE_H
#define LEITMEM_FIXTURE_H

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "leitmem.h"
#include "constants.h"

#include "mock_flipcard_store.h"
#include "mock_time_probe.h"

#include "tools/tm_calc.h"
#include "tools/datashelf.h"
#include "string/case.h"

#include <string_view>

// TODO: new feature: verbatim answer for phrases

inline void prepare_flipcards(
   mzlib::ds::pnode flipcards, 
   std::string_view title)
{
   flipcards->set_name("flipcards");
   flipcards->add_attribute("title", title);
}

inline void add_flipcard(
   mzlib::ds::pnode flipcards,
   std::string_view question,
   std::string_view answer,
   std::vector<std::string_view> keywords)
{
   auto flipcard = flipcards->add_node("flipcard");
   flipcard->add_attribute("question", question);
   flipcard->add_node("answer", answer);
   for(auto keyword : keywords)
   {
      flipcard->add_node("keywords", keyword); 
   }
}

inline std::tm create_some_time()
{
   std::tm some_time;
   some_time.tm_gmtoff = 0;	
   some_time.tm_hour = 19;	
   some_time.tm_isdst = 0;	
   some_time.tm_mday = 10;	
   some_time.tm_min = 20;
   some_time.tm_mon = 1;
   some_time.tm_sec = 21;	
   some_time.tm_wday = 0;
   some_time.tm_yday = 40;	
   some_time.tm_year = 119;	
   some_time.tm_zone = "GMT";
   return some_time;
}

class fixture_leitmem_logic : public ::testing::Test 
{
   
protected:

   fixture_leitmem_logic()
   {
      prepare_flipcards(m_flipcards, 
         "Blade Runner");
            
      ON_CALL(m_flipcard_store, load())
         .WillByDefault(::testing::Return(m_flipcards));
      
      m_today = create_some_time();
      
      ON_CALL(m_time_probe, get_today_local())
         .WillByDefault(::testing::Return(m_today));
   }
   
   ~fixture_leitmem_logic() {}

   mzlib::ds::pnode m_flipcards = std::make_shared<mzlib::ds::node>();   
   ::testing::NiceMock<mock_flipcard_store> m_flipcard_store;
   ::testing::NiceMock<mock_time_probe> m_time_probe; 
   
   std::tm m_today;
   
   std::string_view m_question_1 = "What are synthetic humans called?";
   std::string_view m_answer_1 = "Synthetic humans are called 'replicants'.";
   std::string_view m_keyword_1 = "replicants";
   
   std::string_view m_question_2 = "What book was Blade Runner based on?";
   std::string_view m_keyword_2 = "Do Androids Dream of Electric Sheep";
   
   
   void add_question_1()
   {
      add_flipcard(m_flipcards,
         m_question_1,
         m_answer_1,
         {m_keyword_1}); 
   }

   void add_question_2()
   {
      add_flipcard(m_flipcards,
         m_question_2,
         "Blade Runner was based on a book Do Androids Dream of Electric Sheep by Philip K. Dick.",
         {"Do Androids Dream of Electric Sheep"}); 
   }

   std::string_view get_correct_answer(std::string_view question)
   {
      if(question == m_question_1)
         return m_keyword_1;
      if(question == m_question_2)
         return m_keyword_2;
      return "";
   }
};

#endif /* LEITMEM_FIXTURE_H */

