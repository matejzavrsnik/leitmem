#include "flipcards_from_xml_file.h"

#include "extension/libxmlpp_datashelf.h"

flipcards_from_xml_file::flipcards_from_xml_file(
   std::string_view file) :
      m_xml_file(file)
{}

mzlib::ds::pnode flipcards_from_xml_file::load()
{
   return mzlib::create_data_shelf_from_xml_file(
      m_xml_file);
}

void flipcards_from_xml_file::save(mzlib::ds::pnode flipcards)
{
   mzlib::save_datashelf_to_xml_file(
      flipcards, 
      m_xml_file);
}
