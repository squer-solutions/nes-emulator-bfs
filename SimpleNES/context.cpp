#include <stdint.h>
#include <emscripten/bind.h>
#include <emscripten/val.h>
#include "HeadlessScreen.h"
#include "HeadlessScreen.cpp"
#include "Emulator.h"
#include "Cartridge.cpp"
#include "Controller.cpp"
#include "CPU.cpp"
#include "Emulator.cpp"
#include "Log.cpp"
#include "MainBus.cpp"
#include "Mapper.cpp"
#include "Mapper11.cpp"
#include "MapperAxROM.cpp"
#include "MapperCNROM.cpp"
#include "MapperGxROM.cpp"
#include "MapperMMC3.cpp"
#include "MapperNROM.cpp"
#include "MapperSxROM.cpp"
#include "MapperUxROM.cpp"
#include "PictureBus.cpp"
#include "PPU.cpp"

using namespace emscripten;

class SimpleNesWasmContext {
public:

private:

};
