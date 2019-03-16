#include "lang/optional_string.h"

#include "leitmem.h"
#include "ui_cli.h"
#include "flipcards_from_xml_file.h"

#include "tools/parse_cli_arguments.h"
#include "tools/time_probe.h"
#include "tools/get_if_exists.h"
#include "lang/optionally_functional.h"

#include <iostream>
#include <string_view>
#include <optional>

using namespace mzlib;
using namespace std;


void display_help()
{
   std::cout << 
R"(
Usage:
           
--knowledge=file.xml  Load knowledge file.xml
--statistic=...       Display one of selected statistic:
   qt                    number of questions to be asked today
   qa                    number of questions in the file
)";
}

int main(int argc, char** argv) 
{
   locale::global(std::locale(""));
   std::map<std::string, std::string> cli_arguments = parse_arguments(argc, argv);
   
   auto knowledge = get_if_exists("knowledge"s, cli_arguments);
   if (!knowledge)
   {
      display_help();
      return 1;
   }

   flipcards_from_xml_file flipcard_store(*knowledge);
   
   mzlib::time_probe time_probe;
   leitmem engine(time_probe, flipcard_store);
   
   // stat: questions for this session
   if(auto statistic = get_if_exists("statistic"s, cli_arguments))
   {
      if (::mzlib::equal_to(statistic, "qt"_ostr))
         std::cout << engine.questions_today() << std::endl;
      if (::mzlib::equal_to(statistic, "qa"_ostr))
         std::cout << engine.all_questions() << std::endl;
   }
   // play
   else
   {
      std::cout << "Knowledge file: " << *knowledge << std::endl;
      ui_cli ui(engine);
      ui.main_loop();
   }

   return 0;
}

