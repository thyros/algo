#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct Requirement
{
	int quantity;
	std::string bagColor;
};

using Rules = std::unordered_map<std::string, std::vector<Requirement>>;

std::vector<std::string> ParseInput(std::istream& istr)
{
	std::vector<std::string> input;
	std::string line;
	while (getline(istr, line))
	{
		input.push_back(std::move(line));
	}
	return input;
}

std::vector<std::string> Tokenize(const std::string& input, char delimeter)
{
	std::vector<std::string> tokens;

	size_t b = 0;

	while (b != std::string::npos)
	{
		size_t e = input.find(delimeter, b);

		tokens.push_back(input.substr(b, e - b));
		b = e == std::string::npos ? e : e + 1;
	};

	return tokens;
}

void AppendBag(Rules& rules, const std::string& input)
{
	// vibrant salmon bags contain 1 vibrant gold bag, 2 wavy aqua bags, 1 dotted crimson bag.
	// 2 words describing color | bags contain | [quantity color bag(s),]+

	std::vector<std::string> tokens = Tokenize(input, ' ');

	std::string bagColor = tokens[0] + ' ' + tokens[1];

	std::vector<Requirement> requirements;

	if (tokens.size() >= 8)
	{
		for (size_t i = 4; i < tokens.size(); i += 4)
		{
			int quantity = atoi(tokens[i].c_str());
			std::string nestedBagColor = tokens[i + 1] + ' ' + tokens[i + 2];

			requirements.push_back(Requirement{quantity, nestedBagColor});
		}
	}

	rules[bagColor] = std::move(requirements);
}

Rules ParseRules(const std::vector<std::string>& input)
{
	Rules rules;

	for (const std::string& i : input)
	{
		AppendBag(rules, i);
	}

	return rules;
}

bool ContainsColor(const Rules& rules, const std::vector<Requirement>& requirements)
{
	for (const Requirement& requirement : requirements)
	{
		if (requirement.bagColor == "shiny gold")
		{
			return true;
		}

		const auto it = rules.find(requirement.bagColor);
		if (ContainsColor(rules, it->second))
		{
			return true;
		}
	}

	return false;
}

int CountWrappersForShinyGold(const Rules& rules)
{
	int count = 0;

	for (const auto& [bagColor, requirements] : rules)
	{
		if (ContainsColor(rules, requirements))
		{
			++count;
		}
	}

	return count;
}

int CountBagsInside(const Rules& rules, const std::vector<Requirement>& requirements) {

    int bagsInside = 0;

    for (const Requirement& requirement: requirements) {
        bagsInside += requirement.quantity;

        const auto it = rules.find(requirement.bagColor);
        bagsInside += requirement.quantity * CountBagsInside(rules, it->second);
    }


    return bagsInside;

}

int main()
{
	std::ifstream ifstr("day-07.input");

	std::vector<std::string> input = ParseInput(ifstr);
	Rules rules = ParseRules(input);

	const int wrappersForShinyGold = CountWrappersForShinyGold(rules);
	std::cout << "Wrapprs count: " << wrappersForShinyGold << std::endl;

    const auto it = rules.find("shiny gold");
    const int bagsInsideShinyGold = CountBagsInside(rules, it->second);
	std::cout << "Bags inside: " << bagsInsideShinyGold << std::endl;
}