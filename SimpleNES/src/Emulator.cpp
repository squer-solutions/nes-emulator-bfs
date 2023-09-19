#include "Emulator.h"
#include "Log.h"

//#include <thread>
#include <chrono>

namespace sn
{
    Emulator::Emulator(HeadlessScreen* screen) :
        m_cpu(m_bus),
        m_ppu(m_pictureBus, screen),
        pScreen(screen),
        m_screenScale(2.f),
        m_cycleTimer(),
        m_cpuCycleDuration(std::chrono::nanoseconds(559))
    {
        if(!m_bus.setReadCallback(PPUSTATUS, [&](void) {return m_ppu.getStatus();}) ||
            !m_bus.setReadCallback(PPUDATA, [&](void) {return m_ppu.getData();}) ||
            !m_bus.setReadCallback(JOY1, [&](void) {return m_controller1.read();}) ||
            !m_bus.setReadCallback(JOY2, [&](void) {return m_controller2.read();}) ||
            !m_bus.setReadCallback(OAMDATA, [&](void) {return m_ppu.getOAMData();}))
        {
            LOG(Error) << "Critical error: Failed to set I/O callbacks" << std::endl;
        }


        if(!m_bus.setWriteCallback(PPUCTRL, [&](Byte b) {m_ppu.control(b);}) ||
            !m_bus.setWriteCallback(PPUMASK, [&](Byte b) {m_ppu.setMask(b);}) ||
            !m_bus.setWriteCallback(OAMADDR, [&](Byte b) {m_ppu.setOAMAddress(b);}) ||
            !m_bus.setWriteCallback(PPUADDR, [&](Byte b) {m_ppu.setDataAddress(b);}) ||
            !m_bus.setWriteCallback(PPUSCROL, [&](Byte b) {m_ppu.setScroll(b);}) ||
            !m_bus.setWriteCallback(PPUDATA, [&](Byte b) {m_ppu.setData(b);}) ||
            !m_bus.setWriteCallback(OAMDMA, [&](Byte b) {DMA(b);}) ||
            !m_bus.setWriteCallback(JOY1, [&](Byte b) {m_controller1.strobe(b); m_controller2.strobe(b);}) ||
            !m_bus.setWriteCallback(OAMDATA, [&](Byte b) {m_ppu.setOAMData(b);}))
        {
            LOG(Error) << "Critical error: Failed to set I/O callbacks" << std::endl;
        }

        m_ppu.setInterruptCallback([&](){ m_cpu.interrupt(CPU::NMI); });
    }

    std::string Emulator::run(const std::string& rom) {
        if (!m_cartridge.loadFromFile("/roms/" + rom))
            return "Cannot load rom " + rom;

        m_mapper = Mapper::createMapper(static_cast<Mapper::Type>(m_cartridge.getMapper()),
                                        m_cartridge,
                                        [&](){ m_pictureBus.updateMirroring(); });
        if (!m_mapper)
        {
            LOG(Error) << "Creating Mapper failed. Probably unsupported." << std::endl;
            return "Creating Mapper failed";
        }

        if (!m_bus.setMapper(m_mapper.get()) ||
            !m_pictureBus.setMapper(m_mapper.get()))
            return "Cannot set mapper";

        m_cpu.reset();
        m_ppu.reset();

        return "Success";
    }

    void Emulator::tick() {
        for (int i = 0; i < 29781; ++i) {
            //PPU
            m_ppu.step();
            m_ppu.step();
            m_ppu.step();
            //CPU
            m_cpu.step();
        }
    }

    void Emulator::DMA(Byte page)
    {
        m_cpu.skipDMACycles();
        auto page_ptr = m_bus.getPagePtr(page);
        m_ppu.doDMA(page_ptr);
    }

    void Emulator::setVideoHeight(int height)
    {
        m_screenScale = height / float(NESVideoHeight);
        LOG(Info) << "Scale: " << m_screenScale << " set. Screen: "
                  << int(NESVideoWidth * m_screenScale) << "x" << int(NESVideoHeight * m_screenScale) << std::endl;
    }

    void Emulator::setVideoWidth(int width)
    {
        m_screenScale = width / float(NESVideoWidth);
        LOG(Info) << "Scale: " << m_screenScale << " set. Screen: "
                  << int(NESVideoWidth * m_screenScale) << "x" << int(NESVideoHeight * m_screenScale) << std::endl;

    }
    void Emulator::setVideoScale(float scale)
    {
        m_screenScale = scale;
        LOG(Info) << "Scale: " << m_screenScale << " set. Screen: "
                  << int(NESVideoWidth * m_screenScale) << "x" << int(NESVideoHeight * m_screenScale) << std::endl;
    }

//    void Emulator::setKeys(std::vector<sf::Keyboard::Key>& p1, std::vector<sf::Keyboard::Key>& p2)
//    {
//        m_controller1.setKeyBindings(p1);
//        m_controller2.setKeyBindings(p2);
//    }

}
