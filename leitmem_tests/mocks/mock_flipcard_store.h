#ifndef MOCK_FLIPCARD_STORE_H
#define MOCK_FLIPCARD_STORE_H

#include "flipcard_store_interface.h"

class mock_flipcard_store : public flipcards_store_interface
{
   
public:
   
   MOCK_METHOD0(load, mzlib::ds::pnode());
   MOCK_METHOD1(save, void(mzlib::ds::pnode));
   
};

#endif /* MOCK_FLIPCARD_STORE_H */

