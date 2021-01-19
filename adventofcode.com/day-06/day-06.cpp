#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <iterator>
#include <optional>
#include <sstream>
#include <vector>

using DeclarationForm = std::array<bool, 26>;
using GroupAnswers = std::vector<std::string>;

std::vector<GroupAnswers> ParseInput(std::istream& istr) {
    std::vector<GroupAnswers> groupsAnswers;

    std::string line;
    do {
        GroupAnswers groupAnswers;
        while(std::getline(istr, line)) {
            if (line.empty()) {
                break;
            }

            groupAnswers.push_back(std::move(line));
        }
        groupsAnswers.push_back(std::move(groupAnswers));
    } while(!istr.eof());

    return groupsAnswers;
}

DeclarationForm ParseGroupPart1(const GroupAnswers& groupAnswers) {
    DeclarationForm declarationForm = {false};

    const auto answerToIndex = [](const char c) { return c - 'a'; };

    for (const std::string& line: groupAnswers) {
        if (line.empty()) {
            break;
        }

        for(char c: line) {
            declarationForm[answerToIndex(c)] = true;
        }
    }

    return declarationForm;
}

DeclarationForm ParseGroupPart2(const GroupAnswers& groupAnswers) {
    const auto answerToIndex = [](const char c) { return c - 'a'; };

    std::array<int, 26> answers {0};

    std::string line;
    int partyMembersCount = 0;
    for (const std::string& line: groupAnswers) {
        if (line.empty()) {
            break;
        }

        for(char c: line) {
            ++answers[answerToIndex(c)];
        }
        ++partyMembersCount;
    }

    DeclarationForm declarationForm = {false};
    std::transform(begin(answers), end(answers), begin(declarationForm), [partyMembersCount](int c){return c == partyMembersCount; });
    return declarationForm;
}

int main() {
    std::ifstream ifstr("day-06.input");

    std::vector<GroupAnswers> groupsAnswers = ParseInput(ifstr);

    int sum = 0;
    for (const GroupAnswers& groupAnswers: groupsAnswers) {
        DeclarationForm declarationForm = ParseGroupPart2(groupAnswers);

        std::copy(begin(declarationForm), end(declarationForm), std::ostream_iterator<double>(std::cout, ""));
        const int groupValue = std::count(begin(declarationForm), end(declarationForm), true);
        std::cout << std::endl << groupValue << std::endl;

        sum += groupValue;

    } while (!ifstr.eof());

    std::cout << "Sum: " << sum << std::endl;
}