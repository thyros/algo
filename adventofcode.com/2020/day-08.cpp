#include "utils.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

enum class Token {
    acc,
    jmp,
    nop
};

std::ostream& operator<<(std::ostream& ostr, Token token) {
    static const char* strings[] = {"acc", "jmp", "nop"};
    ostr << strings[static_cast<int>(token)];
    return ostr;
}

struct Instruction {
    Token token;
    int value;
};

using Code = std::vector<Instruction>;

Code ParseCode(const std::vector<std::string>& input) {
    Code code;

    for (const std::string& line: input) {
        const std::vector<std::string_view> tokens = Utils::Tokenize(line);

        const Token token = [token = tokens[0]]() -> Token {
            if (token == "acc") return Token::acc;
            if (token == "jmp") return Token::jmp;
            return Token::nop;
        }();

        const std::string_view& valueToken = tokens[1];
        const int value = atoi(valueToken.substr(1).data()) * (valueToken[0] == '+' ? 1 : -1);

        code.push_back(Instruction {token, value});
    }

    return code;
}

struct VirtualMachine {
    int accumulator = 0;
    size_t instructionPointer = 0;
};

void acc(VirtualMachine& virtualMachine, int value) {
    virtualMachine.accumulator += value;
    virtualMachine.instructionPointer += 1;
}

void jmp(VirtualMachine& virtualMachine, int value) {
    virtualMachine.instructionPointer += value;
}

void nop(VirtualMachine& virtualMachine, int value) {
    virtualMachine.instructionPointer += 1;
}

struct RunResult {
    bool success;
    int value;
};

RunResult RunCode(const Code& code, std::function<void(VirtualMachine&, Token, int)> dispatcher) {
    VirtualMachine vm;
    std::vector<bool> visitedInstructions(code.size(), false);

    while(vm.instructionPointer < code.size() && visitedInstructions[vm.instructionPointer] == false) {
        visitedInstructions[vm.instructionPointer] = true;

        const auto& [token, value] = code[vm.instructionPointer];

        dispatcher(vm, token, value);
    }

    return RunResult{vm.instructionPointer >= code.size(), vm.accumulator};
}

int PartOne(const Code& code) {

    const auto dispatcher = [](VirtualMachine& vm, Token token, int value) {
        if (token == Token::acc) acc(vm, value);
        else if (token == Token::jmp) jmp(vm, value);
        else nop(vm, value);
    };

    RunResult result = RunCode(code, dispatcher);

    return result.value;
}

int PartTwo(const Code& code) {
    for (size_t i = 0; i < code.size(); ++i) {

        const auto dispatcher = [i](VirtualMachine& vm, Token token, int value) {
            const bool fixInstruction = i == vm.instructionPointer;
            if (token == Token::acc) acc(vm, value);
            else if (token == Token::jmp) fixInstruction ? nop(vm, value) : jmp(vm, value);
            else fixInstruction ? jmp(vm, value) : nop(vm, value);
        };

        RunResult result = RunCode(code, dispatcher);
        if (result.success) {
            return result.value;
        }
    }
    return 0;
}

int main()
{
	std::ifstream ifstr("day-08.input");
	std::vector<std::string> input = Utils::ParseInput(ifstr);

    Code code = ParseCode(input);

    const int partOneResult = PartOne(code);
    std::cout << "Last value before loop: " << partOneResult << std::endl;

    const int partTwoResult = PartTwo(code);
    std::cout << "Fixed program value: " << partTwoResult << std::endl;
}