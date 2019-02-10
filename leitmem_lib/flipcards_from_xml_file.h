#ifndef FLIPCARDS_FROM_XML_FILE_H
#define FLIPCARDS_FROM_XML_FILE_H

#include "flipcard_store_interface.h"

#include "tools/datashelf.h"

#include <string>

class flipcards_from_xml_file : public flipcards_store_interface
{
   
private:
   
   std::string m_xml_file;
   
public:
   
   flipcards_from_xml_file(std::string_view file);
   
   mzlib::ds::pnode load() override;
   void save(mzlib::ds::pnode flipcards) override;

};

#endif /* FLIPCARDS_FROM_XML_FILE_H */

