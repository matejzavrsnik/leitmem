#include "leitmem.h"
#include "utils.h"
#include "constants.h"

#include "iterators/get_random.h"
#include "tools/relocate.h"

#include <sstream>
#include <algorithm>

using namespace std;
using namespace mzlib;

leitmem::leitmem(
   time_probe_interface& time_probe,
   flipcards_store_interface& flipcard_store) :
      m_time_probe(time_probe),
      m_flipcard_store(flipcard_store),
      m_workset_size(15),
      m_flipcards(m_flipcard_store.load())
{          
   if (m_flipcards)
   {
      sort_flipcards();
      //debug_print(m_flipcards->nodes());
      top_up_ask_today();
   }
}

int leitmem::questions_today()
{
   return 
      m_ask_today.size() + 
      m_ask_today_after.size();
}

int leitmem::all_questions()
{
   return 
      m_ask_today.size() +
      m_ask_today_after.size() +
      m_ask_later.size() +
      m_never_asked.size();
}

string_view leitmem::get_question()
{             
   // Reconsider flipcards that were at first decided not to be asked today. 
   // Time has passed, some of the "laters" might have become "todays".
   pull_from_later();
   
   // When all todays questions are answered, correctly or not,
   // continue with ones that were answered incorrectly.
   if (m_ask_today.empty())
      m_ask_today.swap(m_ask_today_after);
   
   // If all questions due to be asked today do not add up to a
   // configured working set size, pull in some of the questions
   // that were never asked before.
   top_up_ask_today();
      
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
   sort_flipcards();
}

void leitmem::save_knowledge()
{
   m_flipcard_store.save(m_flipcards);
}

void leitmem::sort_flipcards()
{          
   m_ask_today.clear();
   m_never_asked.clear();
   m_ask_later.clear();
   
   for (auto& flipcard : m_flipcards->nodes()) 
   {
      //todo: test when answered="garbage"
      if (!is_valid_flipcard(flipcard)) 
         continue;
      
      if (ask_today(flipcard, m_time_probe)) {
         m_ask_today.push_back(flipcard);
      }
      else if (never_asked(flipcard)) {
         m_never_asked.push_back(flipcard);
      }
      else {
         m_ask_later.push_back(flipcard);
      }
   }
}

void leitmem::pull_from_later()
{
   for (auto& flipcard : m_ask_later) {
      if (ask_today(flipcard, m_time_probe)) {
         relocate(flipcard, m_ask_later, m_ask_today);
      }
   }
}

template<typename Function>
void repeat_n_times(int times, Function fun)
{
   for(int i=0; i<times; ++i) fun();
}

void leitmem::top_up_ask_today()
{
   size_t how_many = m_workset_size - questions_today();
   
   how_many = std::clamp(how_many, (size_t)0, m_never_asked.size());
   
   if (how_many > 0)
   {
      repeat_n_times (how_many,
         [this](){
            mzlib::ds::pnode never_asked_flipcard = 
               *get_random_element(
                  m_never_asked.begin(), 
                  m_never_asked.end());
            
            relocate(
               never_asked_flipcard, 
               m_never_asked, 
               m_ask_today);
         });
   }
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
