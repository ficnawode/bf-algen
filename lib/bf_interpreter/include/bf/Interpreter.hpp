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
        Interpreter(const std::string source_code);
        std::optional<std::string> Interpret();
        void HandleOperation(char op);

        void IncrementPointer();
        void DecrementPointer();
        void IncrementValue();
        void DecrementValue();
        void OutputByte();

        void BeginLoop();
        void EndLoop();

        int FindLoopMatch(int direction);
        bool CheckSyntax();

        static constexpr unsigned int _maxCycles = 1024;
        static constexpr unsigned int _tapeSize = 1024;
        static constexpr std::string ERROR = "Error";

        std::string _program;
        std::string _output;
        bool _hasError = false;

        std::ptrdiff_t _instructionPointer = 0;
        std::ptrdiff_t _totalCycles = 0;
        std::ptrdiff_t _tapePointer = 0;
        std::array<std::uint8_t, _tapeSize> _tape;
    };
}