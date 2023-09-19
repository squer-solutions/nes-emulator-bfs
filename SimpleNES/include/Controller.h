#ifndef CONTROLLER_H
#define CONTROLLER_H
#include <cstdint>
#include <unordered_set>
#include <unordered_map>

namespace sn
{
    using Byte = std::uint8_t;
    class Controller
    {
    public:
        Controller();
        enum Buttons
        {
            A,
            B,
            Select,
            Start,
            Up,
            Down,
            Left,
            Right,
            TotalButtons,
        };

        void strobe(Byte b);
        Byte read();

        void press(const std::string& button);
        void release(const std::string& button);
        std::unordered_set<Buttons> m_pressedButtons;
    private:
        bool m_strobe;
        unsigned int m_keyStates;

        std::unordered_map<std::string, Buttons> m_keyBindings;
    };
}

#endif // CONTROLLER_H
