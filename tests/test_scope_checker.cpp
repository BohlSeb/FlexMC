#include <gtest/gtest.h>

#include "lexer.h"
#include "line_parser.h"
#include "block_parser.h"
#include "i_file.h"


using namespace flexMC;


namespace {
    std::vector<std::vector<std::string> > getCases(const std::string &f_name) {
        std::vector<std::string> all_cases = readLinesFromFile(f_name);
        std::vector<std::vector<std::string> > programs;
        std::vector<std::string> current;
        for (const auto &line: all_cases) {
            if (line.starts_with("===") && !current.empty()) {
                programs.push_back(current);
                current.clear();
            }
            else if (!line.empty()) {
                current.push_back(line);
            }
        }
        if (!current.empty()) {
            programs.push_back(current);
        }
        return programs;
    }
}


TEST(ScopeChecker, ValidCases) {
    const auto programs = getCases("test_files/scope_check_good.txt");
    ASSERT_GE(programs.size(), 1) << "No programs found in file";
    Lexer lexer;
    for (const auto &program: programs) {
        std::vector<LineParseResult> parsed_program;
        for (const auto &line: program) {
            const auto tokens = lexer.tokenize(line);
            const auto [parse_report, parse_result] = parseStartOfLine(tokens);
            ASSERT_FALSE(parse_report.isError()) << "Expected no error for valid line: " << line;
            parsed_program.push_back(parse_result);
        }
        const auto [report, lineno] = checkScopes(parsed_program);
        if (report.isError()) {
            std::cout << printError("Parser", program.at(lineno), report) << "\n";
            break;
        }
        ASSERT_FALSE(report.isError()) << "Expected no error for valid program";
    }
    const auto programs_bad = getCases("test_files/scope_check_bad.txt");
    ASSERT_GE(programs_bad.size(), 1) << "No programs found in file";
    for (const auto &program: programs_bad) {
        std::vector<LineParseResult> parsed_program;
        for (const auto &line: program) {
            const auto tokens = lexer.tokenize(line);
            const auto [parse_report, parse_result] = parseStartOfLine(tokens);
            ASSERT_FALSE(parse_report.isError()) << "Expected no error for valid line: " << line;
            parsed_program.push_back(parse_result);
        }
        const auto [report, lineno] = checkScopes(parsed_program);
        ASSERT_TRUE(report.isError()) << "Expected error for invalid program";
        std::cout << printError("ScopeChecker", program.at(lineno), report) << "\n";
    }
    std::cout << "Read " << programs.size() << " programs from file" << std::endl;
}
