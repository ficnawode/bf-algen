#include "bf_interpreter/Interpreter.hpp"
#include <cassert>
#include <cstring>

namespace bf_interpreter
{

    Interpreter::Interpreter(const std::string& source_code) :
        _program{source_code}, _output{""}
    {
        std::memset(_tape.data(), 0, _tape.size());
    }

    std::optional<std::string> Interpreter::Interpret(const std::string& source_code)
    {
        Interpreter interpreter = Interpreter(source_code);
        return interpreter.Interpret();
    }

    std::optional<std::string> Interpreter::Interpret()
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

        if(_tapePointer >= _tapeSize)
        {
            _hasError = true;
        }
    }

    void Interpreter::DecrementPointer()
    {
        --_tapePointer;

        if(_tapePointer < 0)
        {
            _hasError = true;
        }
    }

    void Interpreter::IncrementValue()
    {
        if(_tapePointer >= _tapeSize || _tapePointer < 0)
        {
            _hasError = true;
            return;
        }
        ++_tape[_tapePointer];
    }

    void Interpreter::DecrementValue()
    {
        if(_tapePointer >= _tapeSize || _tapePointer < 0)
        {
            _hasError = true;
            return;
        }
        --_tape[_tapePointer];
    }

    void Interpreter::OutputByte()
    {
        if(_tapePointer >= _tapeSize || _tapePointer < 0)
        {
            _hasError = true;
            return;
        }
        _output += _tape[_tapePointer];
    }

    void Interpreter::BeginLoop()
    {
        if(_tapePointer >= _tapeSize || _tapePointer < 0)
        {
            _hasError = true;
            return;
        }
        if(!_tape[_tapePointer])
            _instructionPointer = FindLoopMatch(1) - 1;
    }

    void Interpreter::EndLoop()
    {
        if(_tapePointer >= _tapeSize || _tapePointer < 0)
        {
            _hasError = true;
            return;
        }
        if(_tape[_tapePointer])
            _instructionPointer = FindLoopMatch(-1);
    }

    std::uint16_t Interpreter::FindLoopMatch(const std::int8_t direction)
    {
        assert(direction == -1 || direction == 1);
        uint16_t numNestedLoops = 1;
        uint16_t instructionPointerTmp = _instructionPointer + direction;

        if(instructionPointerTmp == INT16_MAX)
        {
            _hasError = true;
            return 0;
        }

        while(numNestedLoops > 0)
        {
            if(instructionPointerTmp == 0 ||
               instructionPointerTmp >= static_cast<std::uint16_t>(_program.length()))
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
