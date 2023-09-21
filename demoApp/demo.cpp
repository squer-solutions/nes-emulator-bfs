#include <emscripten.h>
#include <iostream>

extern "C" {
  extern void squer_import_function();

  EMSCRIPTEN_KEEPALIVE void triggerSquer() {
    squer_import_function();
  }
}
