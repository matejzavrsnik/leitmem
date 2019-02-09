#include "ui_cli.h"

#include <string_view>
#include <iostream>

using namespace std;
   
ui_cli::ui_cli(leitmem& engine) :
   m_engine(engine),
   m_continue_game(true)
{}

void ui_cli::main_loop()
{
   while (should_game_continue())
   {
      string_view question = present_question();
      string user_input = collect_user_input();

      if(is_command(user_input))
         handle_command(user_input);
      else
         submit_answer(question, user_input);
   }

   m_engine.quit();
}

void ui_cli::output(string_view s)
{
   cout << s << endl;
}

void ui_cli::wait_for_enter()
{
   cin.ignore();
}

string_view ui_cli::present_question()
{
   string_view question = m_engine.get_next_question();
   cout << question << endl;
   return question;
}

string ui_cli::collect_user_input()
{
   string user_input;
   cout << "> ";
   getline(cin, user_input);
   return user_input;
}

bool ui_cli::is_command(string_view user_input)
{
   return (user_input.size() > 0 && user_input[0] == ':');
}

void ui_cli::handle_command(string_view command)
{
   if(command == ":q") {
      output("Goodbye");
      m_continue_game = false;
   }
   else {
      output("Unknown command");
   }
}

void ui_cli::submit_answer(string_view question, string_view answer)
{
   bool correct = m_engine.submit_answer(question, answer);

   if (correct) output("Correct :)");
   else output("Wrong :(");

   output(m_engine.get_answer(question));
   wait_for_enter();
}

bool ui_cli::should_game_continue()
{
   return m_continue_game;
}

