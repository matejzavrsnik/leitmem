#include "leitmem.h"
#include "ui_cli.h"
#include "flipcards_from_xml_file.h"
#include "time_probe.h"

#include "tools/parse_cli_arguments.h"

#include <iostream>
#include <string_view>

using namespace mzlib;
using namespace std;

int main(int argc, char** argv) 
{
   locale::global(std::locale(""));
   auto cli_arguments = parse_arguments(argc, argv);
   
   flipcards_from_xml_file flipcard_store(
      cli_arguments["knowledge"]);
   
   time_probe time_probe;
   
   leitmem engine(time_probe, flipcard_store);
   
   ui_cli ui(engine);
   ui.main_loop();

   return 0;
}

