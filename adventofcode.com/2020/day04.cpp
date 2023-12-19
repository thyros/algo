// https://adventofcode.com/2020/day/4

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <istream>
#include <optional>
#include <sstream>
#include <string_view>

enum class FieldType {
    cid = 0,
    byr = 1 << 0,
    iyr = 1 << 1,
    eyr = 1 << 2,
    hgt = 1 << 3,
    hcl = 1 << 4,
    ecl = 1 << 5,
    pid = 1 << 6
};

constexpr char sample[] =
"pid:087499704 hgt:74in ecl:grn iyr:2012 eyr:2030 byr:1980\n"
"hcl:#623a2f\n"
"\n"
"eyr:2029 ecl:blu cid:129 byr:1989\n"
"iyr:2014 pid:896056539 hcl:#a97842 hgt:165cm\n"
"\n"
"hcl:#888785\n"
"hgt:164cm byr:2001 iyr:2015 cid:88\n"
"pid:545766238 ecl:hzl\n"
"eyr:2022\n"
"\n"
"iyr:2010 hgt:158cm hcl:#b6652a ecl:blu byr:1944 eyr:2021 pid:093154719";


using Passport = int;

bool IsPassportValid(Passport passport) {
    return passport == 0b1111111;
}

Passport ReadPassport(std::istream& istr) {
    const auto byrCheck = [](const std::string&  property) -> std::optional<FieldType> {
        // byr (Birth Year) - four digits; at least 1920 and at most 2002.
        const int birthYear = atoi(property.c_str());
        const bool isValid = birthYear >= 1920 && birthYear <= 2002;
        if (isValid) return std::make_optional(FieldType::byr);
        return {};
    };
    const auto iyrCheck = [](const std::string&  property) -> std::optional<FieldType>  {
        // iyr (Issue Year) - four digits; at least 2010 and at most 2020.
        const int issueYear = atoi(property.c_str());
        const bool isValid = issueYear >= 2010 && issueYear <= 2020;
        if (isValid) return std::make_optional(FieldType::iyr);
        return {};
    };
    const auto eyrCheck = [](const std::string&  property) -> std::optional<FieldType>  {
        // eyr (Expiration Year) - four digits; at least 2020 and at most 2030.
        const int expirationYear = atoi(property.c_str());
        const bool isValid = expirationYear >= 2020 && expirationYear <= 2030;
        if (isValid) return std::make_optional(FieldType::eyr);
        return {};
    };
    const auto hgtCheck = [](const std::string&  property) -> std::optional<FieldType> {
        // hgt (Height) - a number followed by either cm or in:
        //    If cm, the number must be at least 150 and at most 193.
        //    If in, the number must be at least 59 and at most 76.
        const int height = atoi(property.substr(0, property.size() - 2).c_str());
        const std::string unit = property.substr(property.size() - 2);
        const bool isValid = (unit == "cm" && height >= 150 && height <= 193) || (unit == "in" && height >= 59 && height <= 76);
        if (isValid) return std::make_optional(FieldType::hgt);
        return {};
    };
    const auto hclCheck = [](const std::string&  property) -> std::optional<FieldType>  {
        // hcl (Hair Color) - a # followed by exactly six characters 0-9 or a-f.
        const auto isHex = [](const char c) -> bool { return c >= '0' && c <= '9' || c >= 'a' && c <= 'f'; };
        const bool isValidColor = property.size() == 7 && property[0] == '#' && std::all_of(begin(property) + 1, end(property), isHex);
        if (isValidColor) return std::make_optional(FieldType::hcl);
        return {};
    };
    const auto eclCheck = [](const std::string&  property) -> std::optional<FieldType>  {
        // ecl (Eye Color) - exactly one of: .
        std::array<const char*, 7> validColors = {"amb","blu","brn","gry","grn", "hzl", "oth"};
        const bool isValidColor = std::find(begin(validColors), end(validColors), property) != end(validColors);
        if (isValidColor) return std::make_optional(FieldType::ecl);
        return {};
    };
    const auto pidCheck = [](const std::string&  property) -> std::optional<FieldType>  {
        // pid (Passport ID) - a nine-digit number, including leading zeroes.
        const auto isDigit = [](const char c) -> bool { return c >= '0' && c <= '9'; };
        const bool isValidNumber = property.size() == 9 && all_of(begin(property), end(property), isDigit);
        if (isValidNumber) return std::make_optional(FieldType::pid);
        return {};
    };
    const auto ReadField = [&](std::string_view field) -> std::optional<FieldType> {
        const std::string_view id = field.substr(0, 3);
        const std::string property = field.substr(4).data();
        if (id == "byr") return byrCheck(property);
        if (id == "iyr") return iyrCheck(property);
        if (id == "eyr") return eyrCheck(property);
        if (id == "hgt") return hgtCheck(property);
        if (id == "hcl") return hclCheck(property);
        if (id == "ecl") return eclCheck(property);
        if (id == "pid") return pidCheck(property);
        return {};
    };

    Passport passport = 0;
    std::string line;
    while(std::getline(istr, line))
    {
        if (line.empty()) {
            break;
        }

        std::string field;
        std::istringstream lstr(line);
        while (lstr >> field) {
            std::cout << "Checking: " << field;
            if (auto fieldValue = ReadField(field))
            {
                std::cout << " valid" << std::endl;
                passport |= static_cast<int>(fieldValue.value());
            }
            else {
                std::cout << " not valid" << std::endl;
            }
        }
    }
    return passport;
}

int main()
{
    std::ifstream ifstr("day04.input");

    int validPassportsCount = 0;
    Passport passport = 0;
    do {
        passport = ReadPassport(ifstr);
        std::cout << "Passport " << (IsPassportValid(passport) ? "valid" : "not valid") << std::endl << std::endl;;
        validPassportsCount += IsPassportValid(passport) ? 1 : 0;
    } while(!ifstr.eof());

    std::cout << "Total valid passports: " << validPassportsCount << std::endl;
}
