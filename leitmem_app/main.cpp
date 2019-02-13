#include "leitmem.h"
#include "ui_cli.h"
#include "flipcards_from_xml_file.h"

#include "tools/parse_cli_arguments.h"
#include "tools/time_probe.h"

#include <iostream>
#include <string_view>

using namespace mzlib;
using namespace std;

int main(int argc, char** argv) 
{
   locale::global(std::locale(""));
   auto cli_arguments = parse_arguments(argc, argv);
   
   std::cout << "Reading " << cli_arguments["knowledge"] << std::endl;
   
   flipcards_from_xml_file flipcard_store(
      cli_arguments["knowledge"]);
   
   mzlib::time_probe time_probe;
   
   leitmem engine(time_probe, flipcard_store);
   
   ui_cli ui(engine);
   ui.main_loop();

   return 0;
}

