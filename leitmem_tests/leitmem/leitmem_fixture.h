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

#include "../mocks/mock_flipcard_store.h"
#include "../mocks/mock_time_probe.h"
#include "../tools/flipcards_manager.h"
#include "../tools/bits_and_bobs.h"

#include "tools/tm_calc.h"
#include "tools/datashelf.h"
#include "string/case.h"

#include <string_view>

// TODO: new feature: verbatim answer for phrases

class fixture_leitmem_logic : public ::testing::Test 
{
   
protected:

   fixture_leitmem_logic()
   {
      prepare_flipcards(m_flipcards, 
         "Blade Runner");
            
      m_test_questions.work_on(m_flipcards);
      
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
   
   flipcards_manager m_test_questions;
};

#endif /* LEITMEM_FIXTURE_H */

