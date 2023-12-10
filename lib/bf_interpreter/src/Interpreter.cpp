#include "bf/Interpreter.hpp"
#include <cstring>

namespace bf
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
        if(!CheckSyntax())
        {
            return {};
        }

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
        default:
            break;
        }
    }

    void Interpreter::IncrementPointer()
    {
        ++_tapePointer;

        if(_tapePointer >= _tapeSize)
            _hasError = true;
    }

    void Interpreter::DecrementPointer()
    {
        --_tapePointer;

        if(_tapePointer < 0)
            _hasError = true;
    }

    void Interpreter::IncrementValue()
    {
        ++_tape[_tapePointer];
    }

    void Interpreter::DecrementValue()
    {
        --_tape[_tapePointer];
    }

    void Interpreter::OutputByte()
    {
        _output += _tape[_tapePointer];
    }

    void Interpreter::BeginLoop()
    {
        if(!_tape[_tapePointer])
            _instructionPointer = FindLoopMatch(1) - 1;
    }

    void Interpreter::EndLoop()
    {
        if(_tape[_tapePointer])
            _instructionPointer = FindLoopMatch(-1);
    }

    std::int16_t Interpreter::FindLoopMatch(std::int16_t direction)
    {
        int num_nested_loops = 1; // We count the current loop as a nested loop.
        int instructionPointerTmp = _instructionPointer + direction;

        /* We need to walk through the program to figure out where the end of the loop is, by counting how many nested loops there are.
           Count either backwards or forwards depending on direction. */
        while(num_nested_loops > 0)
        {
            if(instructionPointerTmp < 0 ||
               instructionPointerTmp >= static_cast<int>(_program.length()))
            {
                _hasError = true;
                return -1; // -1 has no meaning. Just needed to return.
            }

            char c = _program[instructionPointerTmp];

            if(c == '[')
                num_nested_loops += (direction > 0 ? 1 : -1);
            else if(c == ']')
                num_nested_loops += (direction > 0 ? -1 : 1);

            instructionPointerTmp += direction;
        }

        return instructionPointerTmp;
    }

    bool Interpreter::CheckSyntax()
    {
        for(size_t i = 0; i < _program.length(); ++i)
        {
            _instructionPointer = i;

            if(_program[i] == '[')
                FindLoopMatch(1);
            else if(this->_program[i] == ']')
                FindLoopMatch(-1);

            if(_hasError)
                return false;
        }

        _instructionPointer = 0;
        return true;
    }
}
