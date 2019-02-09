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

using namespace ::testing;

class fixture_leitmem : public Test 
{
   
protected:

   fixture_leitmem() {}
   virtual ~fixture_leitmem() {}
   virtual void SetUp() {}
   virtual void TearDown() {}
   
   NiceMock<mock_flipcard_store> m_flipcard_store; 
   mzlib::ds::pnode m_flipcards;
};


TEST_F(fixture_leitmem, demo) 
{
   ON_CALL(m_flipcard_store, load())
      .WillByDefault(Return(m_flipcards));

   leitmem engine(m_flipcard_store);
   
   SUCCEED();
}
