#ifndef UI_CLI_H
#define UI_CLI_H

#include "leitmem.h"

#include <string_view>

class ui_cli
{

private:
   
   leitmem& m_engine;
   bool m_continue_game;
   
public:
   
   ui_cli(leitmem& engine);
   void main_loop();
   
private:
   
   void output(std::string_view s);
   void wait_for_enter();
   /*std::string_view */void present_question();
   std::string collect_user_input();
   bool is_command(std::string_view user_input);
   void handle_command(std::string_view command);
   void submit_answer(/*std::string_view question,*/ std::string_view answer);
   bool should_game_continue();
};


#endif /* UI_CLI_H */

