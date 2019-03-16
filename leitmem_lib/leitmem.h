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
   
   int questions_today();
   int all_questions();
   
   std::string_view get_question();
   std::string_view get_answer();
   bool submit_answer(std::string_view answer);
   
   void set_workset_size(int workset_size);
   
   void save_knowledge();

private:
   
   mzlib::time_probe_interface& m_time_probe;
   flipcards_store_interface& m_flipcard_store;
   int m_workset_size;
   
   mzlib::ds::pnode m_flipcards;  // all flipcards
   
   mzlib::ds::pnode m_being_asked; // currently out
   std::vector<mzlib::ds::pnode> m_ask_today; // to ask immediately
   std::vector<mzlib::ds::pnode> m_ask_today_after; // asked already, got wrong answer
   std::vector<mzlib::ds::pnode> m_ask_later; // scheduled for some other day
   std::vector<mzlib::ds::pnode> m_never_asked; // never asked yet

   void sort_flipcards();
   void pull_from_later();
   void top_up_ask_today();
   
   void correctly_answered(mzlib::ds::pnode flipcard);
   void incorrectly_answered(mzlib::ds::pnode flipcard);

};

#endif /* LEITMEM_H */

