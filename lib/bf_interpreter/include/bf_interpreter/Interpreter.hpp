#pragma once

#include <array>
#include <cstdint>
#include <optional>
#include <string>

namespace bf_interpreter
{
    class Interpreter
    {
    public:
        static std::optional<std::string> Interpret(const std::string& source_code);

    private:
        Interpreter(const std::string& source_code);
        std::optional<std::string> Run();

        void HandleOperation(const char op);

        void IncrementPointer();
        void DecrementPointer();
        void IncrementValue();
        void DecrementValue();
        void OutputByte();
        void BeginLoop();
        void EndLoop();

        enum Direction
        {
            Left = -1,
            Right = 1
        };
        std::uint16_t FindLoopMatch(const Direction direction);

        const std::string& _program;
        std::string _output;
        bool _hasError = false;

        static constexpr std::uint16_t _maxCycles = 1024;
        static constexpr std::uint16_t _tapeSize = 512;

        std::uint16_t _instructionPointer = 0;
        std::uint16_t _totalCycles = 0;
        std::uint16_t _tapePointer = 0;
        std::array<char, _tapeSize> _tape;
    };
}