#include "lang/optional_string.h"

#include "leitmem.h"
#include "ui_cli.h"
#include "flipcards_from_xml_file.h"

#include "tools/parse_cli_arguments.h"
#include "tools/time_probe.h"
#include "tools/get_if_exists.h"
#include "lang/optionally_functional.h"

#include <boost/program_options.hpp>

#include <iostream>
#include <string_view>
#include <optional>

using namespace mzlib;
using namespace std;
using namespace boost::program_options;

int main(int argc, char** argv) 
{
   string knowledge;
   bool qt, qa;

   options_description cli("Usage");
   cli.add_options()
      ("knowledge", value(&knowledge)->required(), 
         "knowledge file to use in this session")
      ("qt", bool_switch(&qt),
         "displays how many questions to be asked today")
      ("qa", bool_switch(&qa),
         "displays how many questions are in the file");
      
   try 
   { 
      locale::global(std::locale(""));

      variables_map vm;
      store(parse_command_line(argc, argv, cli), vm);

      notify(vm);

      flipcards_from_xml_file flipcard_store(knowledge);

      mzlib::time_probe time_probe;
      leitmem engine(time_probe, flipcard_store);

      if(qt) // return information
      {
         std::cout << engine.questions_today() << std::endl;
      }

      else if(qa) // return information
      {
         std::cout << engine.all_questions() << std::endl;
      }

      else // play
      {
         std::cout << "Knowledge file: " << knowledge << std::endl;
         ui_cli ui(engine);
         ui.main_loop();
      }

      return 0;
   }
   catch (const error_with_option_name& e) 
   {  
      cout << e.what() << "\n";
      cli.print(cout, 20);
   }
}

