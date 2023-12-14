#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>

namespace bf
{
    class Interpreter
    {
    public:
        static std::optional<std::string> Interpret(const std::string& source_code);

    private:
        Interpreter(const std::string& source_code);
        std::optional<std::string> Interpret();

        void HandleOperation(const char op);

        void IncrementPointer();
        void DecrementPointer();
        void IncrementValue();
        void DecrementValue();
        void OutputByte();
        void BeginLoop();
        void EndLoop();

        std::uint16_t FindLoopMatch(std::int8_t direction);

        const std::string& _program;
        std::string _output;
        bool _hasError = false;

        static constexpr std::uint16_t _maxCycles = 10 * 1024;
        static constexpr std::uint16_t _tapeSize = 1024;

        std::uint16_t _instructionPointer = 0;
        std::uint16_t _totalCycles = 0;
        std::uint16_t _tapePointer = 0;
        std::array<std::uint8_t, _tapeSize> _tape;
    };
}