#include "leitmem.h"
#include "utils.h"
#include "constants.h"

#include "iterators/get_random.h"
#include "tools/relocate.h"

#include <sstream>

using namespace std;
using namespace mzlib;

leitmem::leitmem(
   time_probe_interface& time_probe,
   flipcards_store_interface& flipcard_store) :
      m_time_probe(time_probe),
      m_flipcard_store(flipcard_store),
      m_workset_size(10),
      m_flipcards(m_flipcard_store.load())
{          
   if (m_flipcards)
      sort_flipcards(m_flipcards->nodes());
   //debug_print(m_flipcards->nodes());
}

int leitmem::questions_left()
{
   return m_ask_today.size() + m_ask_today_after.size();
}

string_view leitmem::get_question()
{             
   sort_flipcards(m_ask_later);
   
   // when all todays questions are answered, correctly or not,
   // continue with ones that were answered incorrectly.
   if (m_ask_today.empty())
      m_ask_today.swap(m_ask_today_after);
   
   //if (m_ask_today.size() < m_workset_size)
   //   fill_up_ask_today();
      
   if (m_ask_today.empty())
      return "No more questions.";
   
   m_being_asked = *get_random_element(m_ask_today.begin(), m_ask_today.end());

   return get_question_from_flipcard(m_being_asked);
}

string_view leitmem::get_answer()
{
   if (!m_being_asked)
      return "";
   
   auto answer = ds::first(m_being_asked->nodes(), tag_answer)->value();
   return answer;
}

bool leitmem::submit_answer(string_view answer)
{
   if (m_being_asked)
   {
      bool correct = evaluate_answer(answer, m_being_asked);
      
      if (correct) {
         correctly_answered(m_being_asked);
      }
      else {
         incorrectly_answered(m_being_asked);
      }
      return correct;
   }
   return false;
}

void leitmem::set_workset_size(int workset_size)
{
   m_workset_size = workset_size;
}

void leitmem::save_knowledge()
{
   m_flipcard_store.save(m_flipcards);
}

void leitmem::sort_flipcards(const std::vector<mzlib::ds::pnode>& flipcards)
{          
   std::vector<mzlib::ds::pnode> new_ask_later;
   
   for (auto& flipcard : flipcards) 
   {
      if (!is_valid_flipcard(flipcard)) 
         continue;
      
      if (ask_today(flipcard, m_time_probe)) 
      {
         m_ask_today.push_back(flipcard);
      }
      else {
         new_ask_later.push_back(flipcard);
      }
   }
   
   //fill_up_ask_today();
   
   m_ask_later.swap(new_ask_later);
}

void leitmem::correctly_answered(ds::pnode flipcard)
{
   relocate(flipcard, m_ask_today, m_ask_later);
   mark_answered_today(flipcard, m_time_probe);
   advance_level(flipcard);
}

void leitmem::incorrectly_answered(ds::pnode flipcard)
{
   relocate(flipcard, m_ask_today, m_ask_today_after);
   mark_never_answered(flipcard);
   reset_level(flipcard);
}
