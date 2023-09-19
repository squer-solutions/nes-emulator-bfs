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
    SimpleNesWasmContext(int width, int height, std::string rom): width(width), height(height), rom(rom) {
        pScreen = new HeadlessScreen(width, height);
        pEmulator = new sn::Emulator(pScreen);
        pEmulator->setVideoWidth(width);
        pEmulator->setVideoHeight(height);
    }

    virtual ~SimpleNesWasmContext() {
        delete pEmulator;
        delete pScreen;
    }

    std::string run() {
        return pEmulator->run(rom);
    }

    void tick() {
        pEmulator->tick();
    }

    val rawScreen() const {
        return val(typed_memory_view(pScreen->getRawScreenSize(), pScreen->rawScreen()));
    }

    void press(const std::string& button) {
        pEmulator->m_controller1.press(button);
    }

    void release(const std::string& button) {
        pEmulator->m_controller1.release(button);
    }

private:
    const int width, height;
    const std::string rom;
    uint8_t currentColor = 0;
    HeadlessScreen* pScreen;
    bool increaseColor = true;
    sn::Emulator* pEmulator;
};

EMSCRIPTEN_BINDINGS(wasm_context) {
        class_<SimpleNesWasmContext>("SimpleNesWasmContext")
                .constructor<int, int, std::string>()
                .function("tick", &SimpleNesWasmContext::tick)
                .function("rawScreen", &SimpleNesWasmContext::rawScreen)
                .function("run", &SimpleNesWasmContext::run)
                .function("press", &SimpleNesWasmContext::press)
                .function("release", &SimpleNesWasmContext::release)
        ;
};
