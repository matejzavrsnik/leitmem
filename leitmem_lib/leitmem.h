#ifndef LEITMEM_H
#define LEITMEM_H

#include "flipcard_store_interface.h"

#include "tools/datashelf.h"
#include "lang/binary_options.h"
#include "tools/time_probe.h"

#include <string_view>

class leitmem
{
   
private:
   
   mzlib::time_probe_interface& m_time_probe;
   flipcards_store_interface& m_flipcard_store;
   mzlib::ds::pnode m_flipcards;
   std::string m_knowledge_file;
   
   std::vector<mzlib::ds::pnode> m_ask_today;
   std::vector<mzlib::ds::pnode> m_ask_today_after;
   
   mzlib::ds::pnode get_flipcard(std::string_view question);

   void remove_from_todays_session(mzlib::ds::pnode flipcard);
   void correctly_answered(mzlib::ds::pnode flipcard);
   void incorrectly_answered(mzlib::ds::pnode flipcard);
   
   void save_knowledge();
   
public:
   
   leitmem(
      mzlib::time_probe_interface&,
      flipcards_store_interface&);
   
   std::string_view get_next_question();
   std::string_view get_answer(std::string_view question);
   bool submit_answer(std::string_view question, std::string_view answer);
   void quit();
};

#endif /* LEITMEM_H */

