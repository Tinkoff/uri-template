#pragma once

#include "uri-template/uri-template.h"
#include "gtest/gtest.h"

namespace {
    bool ExpandedEqual(const std::string& str1, const std::string& str2) {
        if (str1 == str2) {
            return true;
        }
        // DICT VarValue (std::unordered_map) may be expanded with any order
        // Compare length and set of characteds
        if (str1.length() != str2.length()) {
            return false;
        }

        std::unordered_set<char> uri_characters;
        for (const auto& c : str2) {
            uri_characters.insert(c);
        }
        for (const auto& c : str1) {
            if (uri_characters.count(c) == 0) {
                return false;
            }
        }

        return true;
    }
}

struct TestParams
{
    std::string uri_template_str;
    std::string uri_str;
    std::unordered_map<std::string, URI::Template::VarValue> values;
};

inline std::ostream& operator<<(std::ostream& os, const TestParams& test_param)
{
    os << "{";
    os << "'" << test_param.uri_str << "', ";
    os << "'" << test_param.uri_template_str << "', {";
    std::size_t i = 0;
    for (const auto& [name, value] : test_param.values) {
        if (i) {
            os << ", ";
        }
        os << "'" << name << "': '" << value;
        ++i;
    }
    os << "}}";
    return os;
}

class TemplateNotParse: public testing::TestWithParam<TestParams>
{
protected:
    ::testing::AssertionResult NotParsed(const TestParams& test_param) const
    {
        try {
            URI::Template::ParseTemplate(test_param.uri_template_str);
        } catch (...) {
            return ::testing::AssertionSuccess();
        }
        return ::testing::AssertionFailure() << "'" << test_param.uri_template_str << "' is parsed";
    }
};

class TemplateMatch: public testing::TestWithParam<TestParams>
{
protected:
    ::testing::AssertionResult Matched(const TestParams& test_param) const
    {
        URI::Template::Template uri_template;
        try {
            uri_template = URI::Template::ParseTemplate(test_param.uri_template_str);
        } catch (...) {
            return ::testing::AssertionFailure() << "'" << test_param.uri_template_str << "' is not parsed";
        }

        std::unordered_map<std::string, URI::Template::VarValue> matched_values;
        bool matched = URI::Template::MatchURI(uri_template, test_param.uri_str, &matched_values);

        if (!matched) {
            return ::testing::AssertionFailure() << "'" << test_param.uri_template_str
                                                 << "' template is not matched against '" << test_param.uri_str << "'";
        }
        for (const auto& [ex_name, ex_value] : test_param.values) {
            const auto var_lookup = matched_values.find(ex_name);
            if (var_lookup == matched_values.end()) {
                return ::testing::AssertionFailure() << "'" << ex_name << "' variable is not matched";
            }
            if (ex_value != var_lookup->second) {
                return ::testing::AssertionFailure()
                       << "'" << var_lookup->second << "' differs from expected '" << ex_value << "'";
            }
        }

        std::string expanded_str;
        try {
            expanded_str = URI::Template::ExpandTemplate(uri_template, test_param.values);
        } catch (...) {
            return ::testing::AssertionFailure() << "'" << test_param.uri_template_str << "' is not expanded";
        }

        if (!ExpandedEqual(expanded_str, test_param.uri_str)) {
            return ::testing::AssertionFailure()
                   << "expanded '" << expanded_str << "' != expected '" << test_param.uri_str << "'";
        }
        return ::testing::AssertionSuccess();
    }
};

class TemplateNotMatch: public testing::TestWithParam<TestParams>
{
protected:
    ::testing::AssertionResult NotMatched(const TestParams& test_param) const
    {
        URI::Template::Template uri_template;
        try {
            uri_template = URI::Template::ParseTemplate(test_param.uri_template_str);
        } catch (...) {
            return ::testing::AssertionFailure() << "'" << test_param.uri_template_str << "' is not parsed";
        }

        if (URI::Template::MatchURI(uri_template, test_param.uri_str)) {
            return ::testing::AssertionFailure() << "'" << test_param.uri_template_str
                                                 << "' template is matched against '" << test_param.uri_str << "'";
        }
        return ::testing::AssertionSuccess();
    }
};

class TemplateExpand: public testing::TestWithParam<TestParams>
{
protected:
    ::testing::AssertionResult Expanded(const TestParams& test_param) const
    {
        URI::Template::Template uri_template;
        try {
            uri_template = URI::Template::ParseTemplate(test_param.uri_template_str);
        } catch (...) {
            return ::testing::AssertionFailure() << "'" << test_param.uri_template_str << "' is not parsed";
        }

        std::string expanded_str;
        try {
            expanded_str = URI::Template::ExpandTemplate(uri_template, test_param.values);
        } catch (...) {
            return ::testing::AssertionFailure() << "'" << test_param.uri_template_str << "' is not expanded";
        }

        if (!ExpandedEqual(expanded_str, test_param.uri_str)) {
            return ::testing::AssertionFailure()
                   << "expanded '" << expanded_str << "' != expected '" << test_param.uri_str << "'";
        }
        return ::testing::AssertionSuccess();
    }
};
