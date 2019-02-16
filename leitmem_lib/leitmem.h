#ifndef LEITMEM_H
#define LEITMEM_H

#include "flipcard_store_interface.h"

#include "tools/datashelf.h"
#include "tools/time_probe.h"

#include <string_view>

class leitmem
{

public:
   
   leitmem(
      mzlib::time_probe_interface&,
      flipcards_store_interface&);
   
   int questions_left();
   std::string_view get_question();
   std::string_view get_answer();
   bool submit_answer(std::string_view answer);
   
   void save_knowledge();

private:
   
   mzlib::time_probe_interface& m_time_probe;
   flipcards_store_interface& m_flipcard_store;
   mzlib::ds::pnode m_flipcards;
   
   mzlib::ds::pnode m_being_asked;
   std::vector<mzlib::ds::pnode> m_ask_today;
   std::vector<mzlib::ds::pnode> m_ask_today_after;
   std::vector<mzlib::ds::pnode> m_ask_later;

   void sort_flipcards(const std::vector<mzlib::ds::pnode>& flipcards);
   
   void correctly_answered(mzlib::ds::pnode flipcard);
   void incorrectly_answered(mzlib::ds::pnode flipcard);

};

#endif /* LEITMEM_H */

