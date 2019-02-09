#ifndef MOCK_FLIPCARD_STORE_H
#define MOCK_FLIPCARD_STORE_H

#include "../i_flipcard_store.h"

class mock_flipcard_store : public i_flipcards_store
{
   
public:
   
   MOCK_METHOD0(load, mzlib::ds::pnode());
   MOCK_METHOD1(save, void(mzlib::ds::pnode));
   
};

#endif /* MOCK_FLIPCARD_STORE_H */

