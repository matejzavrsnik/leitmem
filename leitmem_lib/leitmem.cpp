#include "leitmem.h"
#include "utils.h"
#include "constants.h"

#include "iterators/get_random.h"

#include <sstream>

using namespace std;
using namespace mzlib;






ds::pnode leitmem::get_flipcard(string_view question)
{
   auto flipcard = ds::first_with_attribute(
      m_flipcards->nodes(), "flipcard", 
      "question", question);
   return flipcard;
}

void leitmem::remove_from_todays_session(ds::pnode flipcard)
{
   auto flipcard_it = std::find(m_ask_today.begin(), m_ask_today.end(), flipcard);
   m_ask_today.erase(flipcard_it);
}


void leitmem::correctly_answered(ds::pnode flipcard)
{
   remember_it_was_answered_today(flipcard);
   
   advance_level(flipcard);
   
   remove_from_todays_session(flipcard);
}

void leitmem::incorrectly_answered(ds::pnode flipcard)
{
   m_ask_today_after.push_back(flipcard);
   remove_from_todays_session(flipcard);
   ds::add_or_edit_attribute(flipcard, tag_answered, value_never);
   ds::add_or_edit_attribute(flipcard, tag_level, levels[0]);
}

std::vector<mzlib::ds::pnode> leitmem::filter_which_to_ask_today(mzlib::ds::pnode flipcards_document)
{
   std::vector<mzlib::ds::pnode> to_ask_today;
   std::vector<mzlib::ds::pnode> all_flipcards = ds::filter_by_name(
      flipcards_document->nodes(), tag_flipcard);
   
   for(auto flipcard : all_flipcards) 
   {
      std::string_view answered = ds::get_attribute(flipcard, tag_answered)->value();
      
      if (answered.empty() || answered == value_never) {
         to_ask_today.push_back(flipcard);
      }
      else {
         double days_passed = days_between(
            get_today_local(), 
            convert_from_string(answered));
         
         double days_needed_to_pass = get_wait_time(flipcard);
         
         if(days_passed >= days_needed_to_pass)
            to_ask_today.push_back(flipcard);
      }
   }
   return to_ask_today;
}

void leitmem::load_knowledge()
{  
   m_flipcards = m_flipcard_store.load();
   m_ask_today = filter_which_to_ask_today(m_flipcards);
}

void leitmem::save_knowledge()
{
   m_flipcard_store.save(m_flipcards);
}

leitmem::leitmem(
   i_flipcards_store& flipcard_store) :
      m_flipcard_store(flipcard_store),
      m_flipcards(m_flipcard_store.load())
{}

string_view leitmem::get_next_question()
{
   if (m_ask_today.empty())
      m_ask_today.swap(m_ask_today_after);
      
   if (m_ask_today.empty())
      return "No more questions. TODO"; // todo
   
   auto flipcard = *get_random_element(m_ask_today.begin(), m_ask_today.end());

   auto question = ds::get_attribute (flipcard, tag_question)->value();
   return question;
}

string_view leitmem::get_answer(string_view question)
{
   auto flipcard = get_flipcard(question);
   auto answer = ds::first(flipcard->nodes(), tag_answer)->value();
   return answer;
}

bool leitmem::submit_answer(string_view question, string_view answer)
{
   auto flipcard = get_flipcard(question);
   bool correct = evaluate_answer(answer, flipcard);
   if(correct) correctly_answered(flipcard);
   else incorrectly_answered(flipcard);
   return correct;
}

void leitmem::quit() 
{
   save_knowledge();
};