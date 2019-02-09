#ifndef I_FLIPCARD_STORE_H
#define I_FLIPCARD_STORE_H

#include "tools/datashelf.h"

class i_flipcards_store
{
   
public:
   
   virtual mzlib::ds::pnode load() = 0;
   virtual void save(mzlib::ds::pnode) = 0;
   
};

#endif /* I_FLIPCARD_STORE_H */

