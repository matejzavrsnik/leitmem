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
   mzlib::ds::pnode m_flipcards;
};


TEST_F(fixture_leitmem, empty_knowledge_is_no_more_questions) 
{
   leitmem engine(m_flipcard_store);
   std::string_view question = engine.get_next_question();
   
   ASSERT_EQ(question, "No more questions.");
}
