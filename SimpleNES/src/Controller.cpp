#include "Controller.h"

namespace sn
{
    Controller::Controller() :
        m_keyStates(0)
    {
        m_keyBindings["j"] = A;
        m_keyBindings["k"] = B;
        m_keyBindings["rshift"] = Select;
        m_keyBindings["space"] = Start;
        m_keyBindings["w"] = Up;
        m_keyBindings["s"] = Down;
        m_keyBindings["a"] = Left;
        m_keyBindings["d"] = Right;
    }

    void Controller::strobe(Byte b)
    {
       m_strobe = (b & 1);
       if (!m_strobe)
       {
           m_keyStates = 0;
           int shift = 0;
           for (int button = A; button < TotalButtons; ++button)
           {
               bool isKeyPressed = m_pressedButtons.find(static_cast<Buttons>(button)) != m_pressedButtons.end();
               m_keyStates |= (isKeyPressed << shift);
               ++shift;
           }
       }
    }

    Byte Controller::read()
    {
       Byte ret;
       if (m_strobe)
            ret = m_pressedButtons.find(A) != m_pressedButtons.end();
       else
       {
           ret = (m_keyStates & 1);
           m_keyStates >>= 1;
       }
       return ret | 0x40;
        return 0;
    }

    void Controller::press(const std::string& button) {
        m_pressedButtons.insert(m_keyBindings[button]);
    }

    void Controller::release(const std::string& button) {
        m_pressedButtons.erase(m_keyBindings[button]);
    }
}