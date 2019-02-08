#include "leitmem.h"
#include "ui_cli.h"

#include "tools/parse_cli_arguments.h"

#include <iostream>
#include <string_view>

using namespace mzlib;
using namespace std;

int f(int& a) {return 0;}
int f(int a) {return 0;}

int main(int argc, char** argv) 
{
   int b = 1;
   int a = f(b);

   
   
   locale::global(std::locale(""));
   auto cli_arguments = parse_arguments(argc, argv);
   
   leitmem engine;
   engine.load_knowledge_file(cli_arguments["knowledge"]);
   
   ui_cli ui(engine);
   ui.main_loop();

   return 0;
}

