#include <iostream>
#include "Engine.h"

int main(int argc, char* argv[])
{
  Engine my_engine(argc, argv);
  my_engine.update();

  return EXIT_SUCCESS;
}
