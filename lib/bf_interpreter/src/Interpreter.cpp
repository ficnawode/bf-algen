#include "bf_interpreter/Interpreter.hpp"
#include <cassert>

namespace bf_interpreter
{

    Interpreter::Interpreter(const std::string& source_code) :
        _program{source_code}, _output{""}, _tape{}
    {
    }

    std::optional<std::string> Interpreter::Interpret(const std::string& source_code)
    {
        Interpreter interpreter = Interpreter(source_code);
        return interpreter.Run();
    }

    std::optional<std::string> Interpreter::Run()
    {
        while(_instructionPointer < _program.length())
        {
            if(_totalCycles > _maxCycles || _hasError)
                return {};
            HandleOperation(_program[_instructionPointer]);

            ++_instructionPointer;
            ++_totalCycles;
        }
        return _output;
    }

    void Interpreter::HandleOperation(const char op)
    {
        if(_tapePointer >= _tapeSize || _tapePointer < 0)
        {
            _hasError = true;
            return;
        }
        switch(op)
        {
        case '>':
            IncrementPointer();
            break;
        case '<':
            DecrementPointer();
            break;
        case '+':
            IncrementValue();
            break;
        case '-':
            DecrementValue();
            break;
        case '.':
            OutputByte();
            break;
        case '[':
            BeginLoop();
            break;
        case ']':
            EndLoop();
            break;
        }
    }

    void Interpreter::IncrementPointer()
    {
        ++_tapePointer;
    }

    void Interpreter::DecrementPointer()
    {
        --_tapePointer;
    }

    void Interpreter::IncrementValue()
    {
        ++_tape.at(_tapePointer);
    }

    void Interpreter::DecrementValue()
    {
        --_tape.at(_tapePointer);
    }

    void Interpreter::OutputByte()
    {
        _output += _tape.at(_tapePointer);
    }

    void Interpreter::BeginLoop()
    {
        if(_tape.at(_tapePointer) == 0)
            _instructionPointer = FindLoopMatch(Direction::Right) - 1;
    }

    void Interpreter::EndLoop()
    {
        if(_tape.at(_tapePointer != 0))
            _instructionPointer = FindLoopMatch(Direction::Left);
    }

    std::uint16_t Interpreter::FindLoopMatch(const Direction direction)
    {
        std::uint16_t numNestedLoops = 1;
        std::size_t instructionPointerTmp = _instructionPointer + direction;

        while(numNestedLoops > 0)
        {
            if(instructionPointerTmp <= 0 || instructionPointerTmp >= _program.length())
            {
                _hasError = true;
                return 0;
            }

            char c = _program[instructionPointerTmp];
            if(c == '[')
                numNestedLoops += direction;
            else if(c == ']')
                numNestedLoops -= direction;

            instructionPointerTmp += direction;
        }

        return instructionPointerTmp;
    }

}
