#ifndef FLIPCARDS_FROM_XML_FILE_H
#define FLIPCARDS_FROM_XML_FILE_H

#include "i_flipcard_store.h"

#include "tools/datashelf.h"

#include <string>

class flipcards_from_xml_file : public i_flipcards_store
{
   
private:
   
   std::string m_xml_file;
   
public:
   
   flipcards_from_xml_file(std::string_view file);
   
   mzlib::ds::pnode load() override;
   void save(mzlib::ds::pnode flipcards) override;

};

#endif /* FLIPCARDS_FROM_XML_FILE_H */

