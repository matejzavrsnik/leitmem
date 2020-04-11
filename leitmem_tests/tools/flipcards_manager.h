#ifndef QUESTIONS_MANAGER_H
#define QUESTIONS_MANAGER_H

#include <vector>
#include "string/split.h"

// two things are called "flipcard" in these tests so from now on I am trying to
// use the name -data for the content and -node for the flipcard in the deck that
// the engine is using. -data is more convenient for testing, -node is necessary
// for the engine to work.

// Groups the flipcard data into one abstraction.
struct flipcard_data
{
   std::string_view question;
   std::string_view answer;
   std::string_view keywords;
};

// Prepared set of flipcard data.
inline flipcard_data test_flipcards(int i)
{
   static std::vector<flipcard_data> flipcards
   {
      {
         .question = "What are synthetic humans called?",
         .answer   = "Synthetic humans are called 'replicants'.",
         .keywords = "replicants"
      },
      {
         .question = "What book was Blade Runner based on?",
         .answer   = "Blade Runner was based on a book Do Androids Dream of Electric Sheep by Philip K. Dick.",
         .keywords = "Do Androids Dream of Electric Sheep"
      }
   };
   return flipcards[i];
}

// For tests where I couldn't avoid randomised questions, this function will find
// flipcard data for a particular question.
inline flipcard_data test_flipcards(std::string_view question)
{
   if(question == test_flipcards(0).question)
      return test_flipcards(0);
   if(question == test_flipcards(1).question)
      return test_flipcards(1);
   throw;
}


// Initialises flipcards nodes. This deck of flipcards will be later returned to 
// the engine through mock as if they were read from the disk.
inline void initialise_flipcards(
   mzlib::ds::pnode flipcards, 
   std::string_view title)
{
   flipcards->set_name("flipcards");
   flipcards->add_attribute("title", title);
}

// Adds flipcard data to the deck of flipcard nodes. 
// This overload allows adding alternative keywords, which is a feature of leitmem 
// but not something you want to be setting up in most of tests.
inline void add_flipcard(
   mzlib::ds::pnode flipcards,
   std::string_view question,
   std::string_view answer,
   std::vector<std::string_view> keywords)
{
   auto flipcard = flipcards->add_node("flipcard");
   flipcard->add_attribute("question", question);
   flipcard->add_node("answer", answer);
   for(auto keyword : keywords)
   {
      flipcard->add_node("keywords", keyword); 
   }
}

// Adds flipcard data to the deck of flipcard nodes.
// This overload plays nicely with the prepared set of flipcard data.
inline void add_flipcard(
   mzlib::ds::pnode flipcards,
   flipcard_data flipcard)
{
   add_flipcard(flipcards,
      flipcard.question,
      flipcard.answer,
      mzlib::split(flipcard.keywords, ",")); 
}


#endif /* QUESTIONS_MANAGER_H */

