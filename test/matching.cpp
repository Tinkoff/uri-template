#include "fixtures.h"

TEST_P(TemplateMatch, Test)
{
    ASSERT_TRUE(Matched(GetParam()));
}

TEST_P(TemplateNotMatch, Test)
{
    ASSERT_TRUE(NotMatched(GetParam()));
}


// clang-format off
INSTANTIATE_TEST_CASE_P(
    Simple, TemplateMatch,
    ::testing::Values(
        TestParams{"{var}", "value",
                   {{"var", URI::Template::VarValue("value")}}
        },
        TestParams{"asd{var}asd", "asdvalueasd",
                   {{"var", URI::Template::VarValue("value")}}
        },
        TestParams{"{hello}", "Hello%20World%21",
                   {{"hello", URI::Template::VarValue("Hello%20World%21")}}
        },
        TestParams{"{half}", "50%25",
                   {{"half", URI::Template::VarValue("50%25")}}
        },
        TestParams{"{x,y}", "1024,768",
                   {{"x", URI::Template::VarValue("1024")},
                    {"y", URI::Template::VarValue("768")}}
        },
        TestParams{"{x,hello,y}", "1024,Hello%20World%21,768",
                   {{"x", URI::Template::VarValue("1024")},
                    {"hello", URI::Template::VarValue("Hello%20World%21")},
                    {"y", URI::Template::VarValue("768")}}
        },
        TestParams{"{list}", ",,",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"", "", ""})}}
        },
        TestParams{"{base}index", "http%3A%2F%2Fexample.com%2Fhome%2Findex",
                   {{"base", URI::Template::VarValue("http%3A%2F%2Fexample.com%2Fhome%2F")}}
        },
        TestParams{"{42}", "The%20Answer%20to%20the%20Ultimate%20Question%20of%20Life%2C%20the%20Universe%2C%20and%20Everything",
                   {{"42", URI::Template::VarValue("The%20Answer%20to%20the%20Ultimate%20Question%20of%20Life%2C%20the%20Universe%2C%20and%20Everything")}}
        },
        TestParams{"{1337}", "leet,as,it,can,be",
                   {{"1337", URI::Template::VarValue(std::vector<std::string>{"leet", "as", "it", "can", "be"})}}
        }
    )
);

INSTANTIATE_TEST_CASE_P(
    Undefined, TemplateMatch,
    ::testing::Values(
        TestParams{"?{x,empty}", "?1024,", {{"x", URI::Template::VarValue("1024")},
                                            {"empty", URI::Template::VarValue("")}}},
        TestParams{"?{empty,y}", "?,768", {{"empty", URI::Template::VarValue("")},
                                           {"y", URI::Template::VarValue("768")}}},
        TestParams{"?{x,undef}", "?1024", {{"x", URI::Template::VarValue("1024")},
                                           {"undef", URI::Template::VarValue()}}},
        TestParams{"?{undef,y}", "?768", {{"undef", URI::Template::VarValue("768")},
                                          {"y", URI::Template::VarValue()}}},
        TestParams{"O{empty}X", "OX", {{"empty", URI::Template::VarValue()}}},
        TestParams{"O{undef}X", "OX", {{"undef", URI::Template::VarValue()}}},
        TestParams{"{/empty_list}", "", {{"empty_list", URI::Template::VarValue()}}},
        TestParams{"{/empty_list*}", "", {{"empty_list", URI::Template::VarValue()}}},
        TestParams{"{?empty_list}", "", {{"empty_list", URI::Template::VarValue()}}},
        TestParams{"{?empty_keys}", "", {{"empty_keys", URI::Template::VarValue()}}},
        TestParams{"{?empty_keys*}", "", {{"empty_keys", URI::Template::VarValue()}}}
    )
);

INSTANTIATE_TEST_CASE_P(
    Modifiers, TemplateMatch,
    ::testing::Values(
        TestParams{"{var:3}", "val",
                   {{"var", URI::Template::VarValue("val")}}
        },
        TestParams{"{var:30}", "value",
                   {{"var", URI::Template::VarValue("value")}}
        },
        TestParams{"{list}", "red,green,blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{list*}", "red,green,blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{keys}", "semi,%3B,dot,.,comma,%2C",
                   {{"keys", URI::Template::VarValue(std::vector<std::string>{"semi", "%3B", "dot", ".", "comma", "%2C"})}}
        },
        TestParams{"{keys*}", "semi=%3B,dot=.,comma=%2C",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"comma", "%2C"},
                                                                                                  {"semi", "%3B"},
                                                                                                  {"dot", "."}})}}
        }
    )
);

INSTANTIATE_TEST_CASE_P(
    Plus, TemplateMatch,
    ::testing::Values(
        TestParams{"{+path}/here", "/foo/bar/here", {{"path", URI::Template::VarValue("/foo/bar")}}},
        TestParams{"here?ref={+path}", "here?ref=/foo/bar", {{"path", URI::Template::VarValue("/foo/bar")}}},
        TestParams{"up{+path}{var}/here", "up/foo/barvalue/here", {{"path", URI::Template::VarValue("/foo/barvalue")},
                                                                   {"var", URI::Template::VarValue()}}},
        TestParams{"{+path,x}/here", "/foo/bar,1024/here", {{"path", URI::Template::VarValue("/foo/bar")},
                                                            {"x", URI::Template::VarValue("1024")}}}
    )
);

INSTANTIATE_TEST_CASE_P(
    Hash, TemplateMatch,
    ::testing::Values(
        TestParams{"{#var}", "#value", {{"var", URI::Template::VarValue("value")}}},
        TestParams{"{#hello}", "#Hello%20World!", {{"hello", URI::Template::VarValue("Hello%20World!")}}},
        TestParams{"{#half}", "#50%25", {{"half", URI::Template::VarValue("50%25")}}},
        TestParams{"foo{#empty}", "foo#", {{"empty", URI::Template::VarValue("")}}},
        TestParams{"foo{#undef}", "foo", {{"undef", URI::Template::VarValue()}}},
        TestParams{"{#x,hello,y}", "#1024,Hello%20World!,768", {{"x", URI::Template::VarValue("1024")},
                                                                {"hello", URI::Template::VarValue("Hello%20World!")},
                                                                {"y", URI::Template::VarValue("768")}}},
        TestParams{"{#path,x}/here", "#/foo/bar,1024/here", {{"path", URI::Template::VarValue("/foo/bar")},
                                                             {"x", URI::Template::VarValue("1024")}}},
        TestParams{"{#path:6}/here", "#/foo/b/here", {{"path", URI::Template::VarValue("/foo/b")}}},
        TestParams{"{#list}", "#red,green,blue", {{"list", URI::Template::VarValue("red,green,blue")}}},
        TestParams{"{#list*}", "#red,green,blue", {{"list", URI::Template::VarValue("red,green,blue")}}},
        TestParams{"{#keys}", "#semi,;,dot,.,comma,,", {{"keys", URI::Template::VarValue("semi,;,dot,.,comma,,")}}},
        TestParams{"{#keys*}", "#semi=;,dot=.,comma=,", {{"keys", URI::Template::VarValue("semi=;,dot=.,comma=,")}}}
    )
);

INSTANTIATE_TEST_CASE_P(
    Dot, TemplateMatch,
    ::testing::Values(
        TestParams{"{.who}", ".fred",
                   {{"who", URI::Template::VarValue("fred")}}
        },
        TestParams{"{.who,who}", ".fred.fred",
                   {{"who", URI::Template::VarValue("fred")}}
        },
        TestParams{"{.half,who}", ".50%25.fred",
                   {{"half", URI::Template::VarValue("50%25")},
                   {"who", URI::Template::VarValue("fred")}}
        },
        TestParams{"www{.dom*}", "www.example.com",
                   {{"dom", URI::Template::VarValue(std::vector<std::string>{"example", "com"})}}
        },
        TestParams{"X{.var}", "X.value",
                   {{"var", URI::Template::VarValue("value")}}
        },
        TestParams{"X{.empty}", "X.",
                   {{"empty", URI::Template::VarValue("")}}
        },
        TestParams{"X{.undef}", "X",
                   {{"undef", URI::Template::VarValue()}}
        },
        TestParams{"X{.var:3}", "X.val",
                   {{"var", URI::Template::VarValue("val")}}
        },
        TestParams{"X{.list}", "X.red,green,blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"X{.list*}", "X.red.green.blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"X{.keys}", "X.semi,%3B,dot,.,comma,%2C",
                   {{"keys", URI::Template::VarValue(std::vector<std::string>{"semi", "%3B", "dot", ".", "comma", "%2C"})}}
        },
        TestParams{"X{.keys*}", "X.semi=%3B.dot=..comma=%2C",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"comma", "%2C"},
                                                                                                  {"semi", "%3B"},
                                                                                                  {"dot", "."}})}}
        },
        TestParams{"X{.empty_keys}", "X",
                   {{"empty_keys", URI::Template::VarValue()}}
        },
        TestParams{"X{.empty_keys*}", "X",
                   {{"empty_keys", URI::Template::VarValue()}}
        },
        TestParams{"X{.list}", "X.,,",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"", "", ""})}}
        }
    )
);

INSTANTIATE_TEST_CASE_P(
    Slash, TemplateMatch,
    ::testing::Values(
        TestParams{"{/who}", "/fred",
                   {{"who", URI::Template::VarValue("fred")}}
        },
        TestParams{"asd{/who}asd", "asd/fredasd",
                   {{"who", URI::Template::VarValue("fred")}}
        },
        TestParams{"{/who,who}", "/fred/fred",
                   {{"who", URI::Template::VarValue("fred")}}
        },
        TestParams{"{/half,who}", "/50%25/fred",
                   {{"half", URI::Template::VarValue("50%25")},
                    {"who", URI::Template::VarValue("fred")}}
        },
        TestParams{"{/who,dub}", "/fred/me%2Ftoo",
                   {{"who", URI::Template::VarValue("fred")},
                    {"dub", URI::Template::VarValue("me%2Ftoo")}}
        },
        TestParams{"{/var}", "/value",
                   {{"var", URI::Template::VarValue("value")}}
        },
        TestParams{"{/var,empty}", "/value/",
                   {{"var", URI::Template::VarValue("value")},
                    {"empty", URI::Template::VarValue("")}}
        },
        TestParams{"{/var,undef}", "/value",
                   {{"var", URI::Template::VarValue("value")},
                    {"undef", URI::Template::VarValue()}}
        },
        TestParams{"{/var,x}/here", "/value/1024/here",
                   {{"var", URI::Template::VarValue("value")},
                    {"x", URI::Template::VarValue("1024")}}
        },
        TestParams{"{/var:1,vari}", "/v/value",
                   {{"var", URI::Template::VarValue("v")},
                    {"vari", URI::Template::VarValue("value")}}
        },
        TestParams{"{/list}", "/red,green,blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{/list,path:4}", "/red,green,blue/%2Ffoo",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})},
                    {"path", URI::Template::VarValue("%2Ffoo")}}
        },
        TestParams{"{/list*}", "/red/green/blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{/list*,path:4}", "/red/green/blue/%2Ffoo",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue", "%2Ffoo"})},
                    {"path", URI::Template::VarValue()}}
        },
        TestParams{"{/keys}", "/semi,%3B,dot,.,comma,%2C",
                   {{"keys", URI::Template::VarValue(std::vector<std::string>{"semi", "%3B", "dot", ".", "comma", "%2C"})}}
        },
        TestParams{"{/keys*}", "/semi=%3B/dot=./comma=%2C",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"comma", "%2C"},
                                                                                                  {"semi", "%3B"},
                                                                                                  {"dot", "."}})}}
        },
        TestParams{"{/list}", "/,,",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"", "", ""})}}
        }
    )
);

INSTANTIATE_TEST_CASE_P(
    Semicolon, TemplateMatch,
    ::testing::Values(
        TestParams{"{;who}", ";who=fred",
                   {{"who", URI::Template::VarValue("fred")}}
        },
        TestParams{"{;half}", ";half=50%25",
                   {{"half", URI::Template::VarValue("50%25")}}
        },
        TestParams{"{;empty}", ";empty",
                   {{"empty", URI::Template::VarValue("")}}
        },
        TestParams{"{;v,empty,who}", ";v=6;empty;who=fred",
                   {{"v", URI::Template::VarValue("6")},
                    {"empty", URI::Template::VarValue("")},
                    {"who", URI::Template::VarValue("fred")}}
        },
        TestParams{"{;v,bar,who}", ";v=6;who=fred",
                   {{"v", URI::Template::VarValue("6")},
                    {"bar", URI::Template::VarValue()},
                    {"who", URI::Template::VarValue("fred")}}
        },
        TestParams{"{;x,y}", ";x=1024;y=768",
                   {{"x", URI::Template::VarValue("1024")},
                    {"y", URI::Template::VarValue("768")}}
        },
        TestParams{"{;x,y,empty}", ";x=1024;y=768;empty",
                   {{"x", URI::Template::VarValue("1024")},
                    {"y", URI::Template::VarValue("768")},
                    {"empty", URI::Template::VarValue("")}}
        },
        TestParams{"{;x,y,undef}", ";x=1024;y=768",
                   {{"x", URI::Template::VarValue("1024")},
                    {"y", URI::Template::VarValue("768")},
                    {"undef", URI::Template::VarValue()}}
        },
        TestParams{"{;hello:5}", ";hello=Hello",
                   {{"hello", URI::Template::VarValue("Hello")}}
        },
        TestParams{"{;list}", ";list=red,green,blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{;list*}", ";list=red;list=green;list=blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{;keys}", ";keys=semi,%3B,dot,.,comma,%2C",
                   {{"keys", URI::Template::VarValue(std::vector<std::string>{"semi", "%3B", "dot", ".", "comma", "%2C"})}}
        },
        TestParams{"{;keys*}", ";semi=%3B;dot=.;comma=%2C",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"comma", "%2C"},
                                                                                                  {"semi", "%3B"},
                                                                                                  {"dot", "."}})}}
        },
        TestParams{"{;list}", ";list=,,",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"", "", ""})}}
        }
    )
);

INSTANTIATE_TEST_CASE_P(
    Question, TemplateMatch,
    ::testing::Values(
        TestParams{"{?who}", "?who=fred",
                   {{"who", URI::Template::VarValue("fred")}}
        },
        TestParams{"{?half}", "?half=50%25",
                   {{"half", URI::Template::VarValue("50%25")}}
        },
        TestParams{"{?x,y}", "?x=1024&y=768",
                   {{"x", URI::Template::VarValue("1024")},
                    {"y", URI::Template::VarValue("768")}}
        },
        TestParams{"{?x,y,empty}", "?x=1024&y=768&empty=",
                   {{"x", URI::Template::VarValue("1024")},
                    {"y", URI::Template::VarValue("768")},
                    {"empty", URI::Template::VarValue("")}}
        },
        TestParams{"{?x,y,undef}", "?x=1024&y=768",
                   {{"x", URI::Template::VarValue("1024")},
                    {"y", URI::Template::VarValue("768")},
                    {"undef", URI::Template::VarValue()}}
        },
        TestParams{"{?var:3}", "?var=val",
                   {{"var", URI::Template::VarValue("val")}}
        },
        TestParams{"{?list}", "?list=red,green,blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{?list*}", "?list=red&list=green&list=blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{?keys}", "?keys=semi,%3B,dot,.,comma,%2C",
                   {{"keys", URI::Template::VarValue(std::vector<std::string>{"semi", "%3B", "dot", ".", "comma", "%2C"})}}
        },
        TestParams{"{?keys*}", "?semi=%3B&dot=.&comma=%2C",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"comma", "%2C"},
                                                                                                  {"semi", "%3B"},
                                                                                                  {"dot", "."}})}}
        },
        TestParams{"{?list}", "?list=,,",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"", "", ""})}}
        },
        TestParams{"{?42}", "?42=The%20Answer%20to%20the%20Ultimate%20Question%20of%20Life%2C%20the%20Universe%2C%20and%20Everything",
                   {{"42", URI::Template::VarValue("The%20Answer%20to%20the%20Ultimate%20Question%20of%20Life%2C%20the%20Universe%2C%20and%20Everything")}}
        },
        TestParams{"{?1337*}", "?1337=leet&1337=as&1337=it&1337=can&1337=be",
                   {{"1337", URI::Template::VarValue(std::vector<std::string>{"leet", "as", "it", "can", "be"})}}
        }
    )
);

INSTANTIATE_TEST_CASE_P(
    Ampersand, TemplateMatch,
    ::testing::Values(
        TestParams{"{&who}", "&who=fred",
                   {{"who", URI::Template::VarValue("fred")}}
        },
        TestParams{"{&half}", "&half=50%25",
                   {{"half", URI::Template::VarValue("50%25")}}
        },
        TestParams{"?fixed=yes{&x}", "?fixed=yes&x=1024",
                   {{"x", URI::Template::VarValue("1024")}}
        },
        TestParams{"{&x,y,empty}", "&x=1024&y=768&empty=",
                   {{"x", URI::Template::VarValue("1024")},
                    {"y", URI::Template::VarValue("768")},
                    {"empty", URI::Template::VarValue("")}}
        },
        TestParams{"{&x,y,undef}", "&x=1024&y=768",
                   {{"x", URI::Template::VarValue("1024")},
                    {"y", URI::Template::VarValue("768")},
                    {"undef", URI::Template::VarValue()}}
        },
        TestParams{"{&var:3}", "&var=val",
                   {{"var", URI::Template::VarValue("val")}}
        },
        TestParams{"{&list}", "&list=red,green,blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{&list*}", "&list=red&list=green&list=blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{&keys}", "&keys=semi,%3B,dot,.,comma,%2C",
                   {{"keys", URI::Template::VarValue(std::vector<std::string>{"semi", "%3B", "dot", ".", "comma", "%2C"})}}
        },
        TestParams{"{&keys*}", "&semi=%3B&dot=.&comma=%2C",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"comma", "%2C"},
                                                                                                  {"semi", "%3B"},
                                                                                                  {"dot", "."}})}}
        },
        TestParams{"{&list}", "&list=,,",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"", "", ""})}}
        }
    )
);

INSTANTIATE_TEST_CASE_P(
    PathQuery, TemplateMatch,
    ::testing::Values(
        TestParams{"{/id*}{?fields,token}",
                   "/person/albums?fields=id,name,picture&token=12345",
                   {{"id", URI::Template::VarValue(std::vector<std::string>{"person", "albums"})},
                    {"fields", URI::Template::VarValue(std::vector<std::string>{"id", "name", "picture"})},
                    {"token", URI::Template::VarValue("12345")}}
        },
        TestParams{"{/id*}{?fields,first_name,last.name,token}",
                   "/person?fields=id,name,picture&first_name=John&last.name=Doe&token=12345",
                   {{"id", URI::Template::VarValue("person")},
                    {"fields", URI::Template::VarValue(std::vector<std::string>{"id", "name", "picture"})},
                    {"first_name", URI::Template::VarValue("John")},
                    {"last.name", URI::Template::VarValue("Doe")},
                    {"token", URI::Template::VarValue("12345")}}
        },
        TestParams{"{?id,token,keys*}",
                   "?id=admin&token=12345&key1=val1&key2=val2",
                   {{"id", URI::Template::VarValue("admin")},
                    {"token", URI::Template::VarValue("12345")},
                    {"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"key2", "val2"},
                                                                                                  {"key1", "val1"}})}}
        },
        TestParams{"/user{/id}{?token,tab}{&keys*}",
                   "/user/admin?token=12345&tab=overview&key1=val1&key2=val2",
                   {{"id", URI::Template::VarValue("admin")},
                    {"token", URI::Template::VarValue("12345")},
                    {"tab", URI::Template::VarValue("overview")},
                    {"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"key2", "val2"},
                                                                                                  {"key1", "val1"}})}}
        },
        TestParams{"/search.{format}{?q,geo,lang,locale,page,result_type}",
                   "/search.json?q=URI%20Templates&geo=37.76,-122.427&lang=en&locale=en-US&page=5&result_type=",
                   {{"format", URI::Template::VarValue("json")},
                    {"q", URI::Template::VarValue("URI%20Templates")},
                    {"geo", URI::Template::VarValue(std::vector<std::string>{"37.76", "-122.427"})},
                    {"lang", URI::Template::VarValue("en")},
                    {"locale", URI::Template::VarValue("en-US")},
                    {"page", URI::Template::VarValue("5")},
                    {"result_type", URI::Template::VarValue("")}}
        },
        TestParams{"/search.{format}{?q,geo,lang,locale,page,result_type}",
                   "/search.json?q=URI%20Templates&geo=37.76,-122.427&lang=en&locale=en-US&page=5",
                   {{"format", URI::Template::VarValue("json")},
                    {"q", URI::Template::VarValue("URI%20Templates")},
                    {"geo", URI::Template::VarValue(std::vector<std::string>{"37.76", "-122.427"})},
                    {"lang", URI::Template::VarValue("en")},
                    {"locale", URI::Template::VarValue("en-US")},
                    {"page", URI::Template::VarValue("5")},
                    {"result_type", URI::Template::VarValue()}}
        },
        TestParams{"/search.{format}{?q,geo,lang,locale,page,result_type}",
                   "/search.json?q=URI%20Templates&geo=37.76,-122.427&lang=en&page=5&result_type=1",
                   {{"format", URI::Template::VarValue("json")},
                    {"q", URI::Template::VarValue("URI%20Templates")},
                    {"geo", URI::Template::VarValue(std::vector<std::string>{"37.76", "-122.427"})},
                    {"lang", URI::Template::VarValue("en")},
                    {"locale", URI::Template::VarValue()},
                    {"page", URI::Template::VarValue("5")},
                    {"result_type", URI::Template::VarValue("1")}}
        },
        TestParams{"/search.{format}{?params*}",
                   "/search.json?q=URI%20Templates&geocode=487150&lang=en&page=5&result_type=1",
                   {{"format", URI::Template::VarValue("json")},
                    {"params", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"page", "5"},
                                                                                                    {"lang", "en"},
                                                                                                    {"q", "URI%20Templates"},
                                                                                                    {"result_type", "1"},
                                                                                                    {"geocode", "487150"}})}}
        }
    )
);

INSTANTIATE_TEST_CASE_P(
    ProtoHostPath, TemplateMatch,
    ::testing::Values(
        TestParams{"{proto}://{www}tinkoff.ru/business{/node*}", "http://www.tinkoff.ru/business/a/b/c/1/2/3/",
                   {{"proto", URI::Template::VarValue("http")},
                    {"www", URI::Template::VarValue("www.")},
                    {"node", URI::Template::VarValue(std::vector<std::string>{"a", "b", "c", "1", "2", "3", ""})}}
        },
        TestParams{"{proto}://{www}tinkoff.ru/business{/node*}", "http://www.tinkoff.ru/business/a/b/c/1/2/3",
                   {{"proto", URI::Template::VarValue("http")},
                    {"www", URI::Template::VarValue("www.")},
                    {"node", URI::Template::VarValue(std::vector<std::string>{"a", "b", "c", "1", "2", "3"})}}
        },
        TestParams{"{proto}://{www}tinkoff.ru/business{/node*}", "http://www.tinkoff.ru/business/a/b/c",
                   {{"proto", URI::Template::VarValue("http")},
                    {"www", URI::Template::VarValue("www.")},
                    {"node", URI::Template::VarValue(std::vector<std::string>{"a", "b", "c"})}}
        },
        TestParams{"{proto}://{www}tinkoff.ru/business{/node*}", "http://www.tinkoff.ru/business/1/2/3",
                   {{"proto", URI::Template::VarValue("http")},
                    {"www", URI::Template::VarValue("www.")},
                    {"node", URI::Template::VarValue(std::vector<std::string>{"1", "2", "3"})}}
        },
        TestParams{"{proto}://{www}tinkoff.ru/business{/node*}", "http://www.tinkoff.ru/business/",
                   {{"proto", URI::Template::VarValue("http")},
                    {"www", URI::Template::VarValue("www.")},
                    {"node", URI::Template::VarValue("")}}
        },
        TestParams{"{proto}://{www}tinkoff.ru/business{/node*}", "http://www.tinkoff.ru/business",
                   {{"proto", URI::Template::VarValue("http")},
                    {"www", URI::Template::VarValue("www.")},
                    {"node", URI::Template::VarValue()}}
        },
        TestParams{"{proto}://{www}tinkoff.ru/business{/node*}", "https://www.tinkoff.ru/business/a/b/c/1/2/3",
                   {{"proto", URI::Template::VarValue("https")},
                    {"www", URI::Template::VarValue("www.")},
                    {"node", URI::Template::VarValue(std::vector<std::string>{"a", "b", "c", "1", "2", "3"})}}
        },
        TestParams{"{proto}://{www}tinkoff.ru/business{/node*}", "ftp://www.tinkoff.ru/business/a/b/c/1/2/3",
                   {{"proto", URI::Template::VarValue("ftp")},
                    {"www", URI::Template::VarValue("www.")},
                    {"node", URI::Template::VarValue(std::vector<std::string>{"a", "b", "c", "1", "2", "3"})}}
        },
        TestParams{"{proto}://{www}tinkoff.ru/business{/node*}", "https://tinkoff.ru/business/a/b/c/1/2/3",
                   {{"proto", URI::Template::VarValue("https")},
                    {"www", URI::Template::VarValue()},
                    {"node", URI::Template::VarValue(std::vector<std::string>{"a", "b", "c", "1", "2", "3"})}}
        },
        TestParams{"{proto}://{www}tinkoff.ru/business{/node*}", "https://tinkoff.ru/business/a/b/c",
                   {{"proto", URI::Template::VarValue("https")},
                    {"www", URI::Template::VarValue()},
                    {"node", URI::Template::VarValue(std::vector<std::string>{"a", "b", "c"})}}
        },
        TestParams{"{proto}://{www}tinkoff.ru/business{/node*}", "https://tinkoff.ru/business",
                   {{"proto", URI::Template::VarValue("https")},
                    {"www", URI::Template::VarValue()},
                    {"node", URI::Template::VarValue()}}
        }
    )
);

INSTANTIATE_TEST_CASE_P(
    Simple, TemplateNotMatch,
    ::testing::Values(
        TestParams{"{val}", ":", {/* not matched */}},
        TestParams{"{val}", "/", {/* not matched */}},
        TestParams{"{val}", "?", {/* not matched */}},
        TestParams{"{val}", "#", {/* not matched */}},
        TestParams{"{val}", "[", {/* not matched */}},
        TestParams{"{val}", "]", {/* not matched */}},
        TestParams{"{val}", "@", {/* not matched */}},
        TestParams{"{val}", "!", {/* not matched */}},
        TestParams{"{val}", "$", {/* not matched */}},
        TestParams{"{val}", "&", {/* not matched */}},
        TestParams{"{val}", "'", {/* not matched */}},
        TestParams{"{val}", "(", {/* not matched */}},
        TestParams{"{val}", ")", {/* not matched */}},
        TestParams{"{val}", "*", {/* not matched */}},
        TestParams{"{val}", "+", {/* not matched */}},
        TestParams{"{val}", ";", {/* not matched */}},
        TestParams{"{val}", "=", {/* not matched */}},
        TestParams{"{val}", "foo:", {/* not matched */}},
        TestParams{"{val}", "foo/", {/* not matched */}},
        TestParams{"{val}", "foo?", {/* not matched */}},
        TestParams{"{val}", "foo#", {/* not matched */}},
        TestParams{"{val}", "foo[", {/* not matched */}},
        TestParams{"{val}", "foo]", {/* not matched */}},
        TestParams{"{val}", "foo@", {/* not matched */}},
        TestParams{"{val}", "foo!", {/* not matched */}},
        TestParams{"{val}", "foo$", {/* not matched */}},
        TestParams{"{val}", "foo&", {/* not matched */}},
        TestParams{"{val}", "foo'", {/* not matched */}},
        TestParams{"{val}", "foo(", {/* not matched */}},
        TestParams{"{val}", "foo)", {/* not matched */}},
        TestParams{"{val}", "foo*", {/* not matched */}},
        TestParams{"{val}", "foo+", {/* not matched */}},
        TestParams{"{val}", "foo;", {/* not matched */}},
        TestParams{"{val}", "foo=", {/* not matched */}},
        TestParams{"{val}", ":bar", {/* not matched */}},
        TestParams{"{val}", "/bar", {/* not matched */}},
        TestParams{"{val}", "?bar", {/* not matched */}},
        TestParams{"{val}", "#bar", {/* not matched */}},
        TestParams{"{val}", "[bar", {/* not matched */}},
        TestParams{"{val}", "]bar", {/* not matched */}},
        TestParams{"{val}", "@bar", {/* not matched */}},
        TestParams{"{val}", "!bar", {/* not matched */}},
        TestParams{"{val}", "$bar", {/* not matched */}},
        TestParams{"{val}", "&bar", {/* not matched */}},
        TestParams{"{val}", "'bar", {/* not matched */}},
        TestParams{"{val}", "(bar", {/* not matched */}},
        TestParams{"{val}", ")bar", {/* not matched */}},
        TestParams{"{val}", "*bar", {/* not matched */}},
        TestParams{"{val}", "+bar", {/* not matched */}},
        TestParams{"{val}", ";bar", {/* not matched */}},
        TestParams{"{val}", "=bar", {/* not matched */}}
    )
);

INSTANTIATE_TEST_CASE_P(
    Dot, TemplateNotMatch,
    ::testing::Values(
        TestParams{"{.val}", ".:", {/* not matched */}},
        TestParams{"{.val}", "./", {/* not matched */}},
        TestParams{"{.val}", ".?", {/* not matched */}},
        TestParams{"{.val}", ".#", {/* not matched */}},
        TestParams{"{.val}", ".[", {/* not matched */}},
        TestParams{"{.val}", ".]", {/* not matched */}},
        TestParams{"{.val}", ".@", {/* not matched */}},
        TestParams{"{.val}", ".!", {/* not matched */}},
        TestParams{"{.val}", ".$", {/* not matched */}},
        TestParams{"{.val}", ".&", {/* not matched */}},
        TestParams{"{.val}", ".'", {/* not matched */}},
        TestParams{"{.val}", ".(", {/* not matched */}},
        TestParams{"{.val}", ".)", {/* not matched */}},
        TestParams{"{.val}", ".*", {/* not matched */}},
        TestParams{"{.val}", ".+", {/* not matched */}},
        TestParams{"{.val}", ".;", {/* not matched */}},
        TestParams{"{.val}", ".=", {/* not matched */}},
        TestParams{"{.val}", "foo", {/* not matched */}},
        TestParams{"{.val}", ".foo:", {/* not matched */}},
        TestParams{"{.val}", ".foo/", {/* not matched */}},
        TestParams{"{.val}", ".foo?", {/* not matched */}},
        TestParams{"{.val}", ".foo#", {/* not matched */}},
        TestParams{"{.val}", ".foo[", {/* not matched */}},
        TestParams{"{.val}", ".foo]", {/* not matched */}},
        TestParams{"{.val}", ".foo@", {/* not matched */}},
        TestParams{"{.val}", ".foo!", {/* not matched */}},
        TestParams{"{.val}", ".foo$", {/* not matched */}},
        TestParams{"{.val}", ".foo&", {/* not matched */}},
        TestParams{"{.val}", ".foo'", {/* not matched */}},
        TestParams{"{.val}", ".foo(", {/* not matched */}},
        TestParams{"{.val}", ".foo)", {/* not matched */}},
        TestParams{"{.val}", ".foo*", {/* not matched */}},
        TestParams{"{.val}", ".foo+", {/* not matched */}},
        TestParams{"{.val}", ".foo;", {/* not matched */}},
        TestParams{"{.val}", ".foo=", {/* not matched */}},
        TestParams{"{.val}", ".:bar", {/* not matched */}},
        TestParams{"{.val}", "./bar", {/* not matched */}},
        TestParams{"{.val}", ".?bar", {/* not matched */}},
        TestParams{"{.val}", ".#bar", {/* not matched */}},
        TestParams{"{.val}", ".[bar", {/* not matched */}},
        TestParams{"{.val}", ".]bar", {/* not matched */}},
        TestParams{"{.val}", ".@bar", {/* not matched */}},
        TestParams{"{.val}", ".!bar", {/* not matched */}},
        TestParams{"{.val}", ".$bar", {/* not matched */}},
        TestParams{"{.val}", ".&bar", {/* not matched */}},
        TestParams{"{.val}", ".'bar", {/* not matched */}},
        TestParams{"{.val}", ".(bar", {/* not matched */}},
        TestParams{"{.val}", ".)bar", {/* not matched */}},
        TestParams{"{.val}", ".*bar", {/* not matched */}},
        TestParams{"{.val}", ".+bar", {/* not matched */}},
        TestParams{"{.val}", ".;bar", {/* not matched */}},
        TestParams{"{.val}", ".=bar", {/* not matched */}}
    )
);

INSTANTIATE_TEST_CASE_P(
    Slash, TemplateNotMatch,
    ::testing::Values(
        TestParams{"{/val}", "/:", {/* not matched */}},
        TestParams{"{/val}", "//", {/* not matched */}},
        TestParams{"{/val}", "/?", {/* not matched */}},
        TestParams{"{/val}", "/#", {/* not matched */}},
        TestParams{"{/val}", "/[", {/* not matched */}},
        TestParams{"{/val}", "/]", {/* not matched */}},
        TestParams{"{/val}", "/@", {/* not matched */}},
        TestParams{"{/val}", "/!", {/* not matched */}},
        TestParams{"{/val}", "/$", {/* not matched */}},
        TestParams{"{/val}", "/&", {/* not matched */}},
        TestParams{"{/val}", "/'", {/* not matched */}},
        TestParams{"{/val}", "/(", {/* not matched */}},
        TestParams{"{/val}", "/)", {/* not matched */}},
        TestParams{"{/val}", "/*", {/* not matched */}},
        TestParams{"{/val}", "/+", {/* not matched */}},
        TestParams{"{/val}", "/;", {/* not matched */}},
        TestParams{"{/val}", "/=", {/* not matched */}},
        TestParams{"{/val}", "foo", {/* not matched */}},
        TestParams{"{/val}", "/foo:", {/* not matched */}},
        TestParams{"{/val}", "/foo/", {/* not matched */}},
        TestParams{"{/val}", "/foo?", {/* not matched */}},
        TestParams{"{/val}", "/foo#", {/* not matched */}},
        TestParams{"{/val}", "/foo[", {/* not matched */}},
        TestParams{"{/val}", "/foo]", {/* not matched */}},
        TestParams{"{/val}", "/foo@", {/* not matched */}},
        TestParams{"{/val}", "/foo!", {/* not matched */}},
        TestParams{"{/val}", "/foo$", {/* not matched */}},
        TestParams{"{/val}", "/foo&", {/* not matched */}},
        TestParams{"{/val}", "/foo'", {/* not matched */}},
        TestParams{"{/val}", "/foo(", {/* not matched */}},
        TestParams{"{/val}", "/foo)", {/* not matched */}},
        TestParams{"{/val}", "/foo*", {/* not matched */}},
        TestParams{"{/val}", "/foo+", {/* not matched */}},
        TestParams{"{/val}", "/foo;", {/* not matched */}},
        TestParams{"{/val}", "/foo=", {/* not matched */}},
        TestParams{"{/val}", "/:bar", {/* not matched */}},
        TestParams{"{/val}", "//bar", {/* not matched */}},
        TestParams{"{/val}", "/?bar", {/* not matched */}},
        TestParams{"{/val}", "/#bar", {/* not matched */}},
        TestParams{"{/val}", "/[bar", {/* not matched */}},
        TestParams{"{/val}", "/]bar", {/* not matched */}},
        TestParams{"{/val}", "/@bar", {/* not matched */}},
        TestParams{"{/val}", "/!bar", {/* not matched */}},
        TestParams{"{/val}", "/$bar", {/* not matched */}},
        TestParams{"{/val}", "/&bar", {/* not matched */}},
        TestParams{"{/val}", "/'bar", {/* not matched */}},
        TestParams{"{/val}", "/(bar", {/* not matched */}},
        TestParams{"{/val}", "/)bar", {/* not matched */}},
        TestParams{"{/val}", "/*bar", {/* not matched */}},
        TestParams{"{/val}", "/+bar", {/* not matched */}},
        TestParams{"{/val}", "/;bar", {/* not matched */}},
        TestParams{"{/val}", "/=bar", {/* not matched */}}
    )
);

INSTANTIATE_TEST_CASE_P(
    Semicolon, TemplateNotMatch,
    ::testing::Values(
        TestParams{"{;val}", ";val=:", {/* not matched */}},
        TestParams{"{;val}", ";val=/", {/* not matched */}},
        TestParams{"{;val}", ";val=?", {/* not matched */}},
        TestParams{"{;val}", ";val=#", {/* not matched */}},
        TestParams{"{;val}", ";val=[", {/* not matched */}},
        TestParams{"{;val}", ";val=]", {/* not matched */}},
        TestParams{"{;val}", ";val=@", {/* not matched */}},
        TestParams{"{;val}", ";val=!", {/* not matched */}},
        TestParams{"{;val}", ";val=$", {/* not matched */}},
        TestParams{"{;val}", ";val=&", {/* not matched */}},
        TestParams{"{;val}", ";val='", {/* not matched */}},
        TestParams{"{;val}", ";val=(", {/* not matched */}},
        TestParams{"{;val}", ";val=)", {/* not matched */}},
        TestParams{"{;val}", ";val=*", {/* not matched */}},
        TestParams{"{;val}", ";val=+", {/* not matched */}},
        TestParams{"{;val}", ";val=;", {/* not matched */}},
        TestParams{"{;val}", ";val==", {/* not matched */}},
        TestParams{"{;val}", "foo", {/* not matched */}},
        TestParams{"{;val}", ";val=foo:", {/* not matched */}},
        TestParams{"{;val}", ";val=foo/", {/* not matched */}},
        TestParams{"{;val}", ";val=foo?", {/* not matched */}},
        TestParams{"{;val}", ";val=foo#", {/* not matched */}},
        TestParams{"{;val}", ";val=foo[", {/* not matched */}},
        TestParams{"{;val}", ";val=foo]", {/* not matched */}},
        TestParams{"{;val}", ";val=foo@", {/* not matched */}},
        TestParams{"{;val}", ";val=foo!", {/* not matched */}},
        TestParams{"{;val}", ";val=foo$", {/* not matched */}},
        TestParams{"{;val}", ";val=foo&", {/* not matched */}},
        TestParams{"{;val}", ";val=foo'", {/* not matched */}},
        TestParams{"{;val}", ";val=foo(", {/* not matched */}},
        TestParams{"{;val}", ";val=foo)", {/* not matched */}},
        TestParams{"{;val}", ";val=foo*", {/* not matched */}},
        TestParams{"{;val}", ";val=foo+", {/* not matched */}},
        TestParams{"{;val}", ";val=foo;", {/* not matched */}},
        TestParams{"{;val}", ";val=foo=", {/* not matched */}},
        TestParams{"{;val}", ";val=:bar", {/* not matched */}},
        TestParams{"{;val}", ";val=/bar", {/* not matched */}},
        TestParams{"{;val}", ";val=?bar", {/* not matched */}},
        TestParams{"{;val}", ";val=#bar", {/* not matched */}},
        TestParams{"{;val}", ";val=[bar", {/* not matched */}},
        TestParams{"{;val}", ";val=]bar", {/* not matched */}},
        TestParams{"{;val}", ";val=@bar", {/* not matched */}},
        TestParams{"{;val}", ";val=!bar", {/* not matched */}},
        TestParams{"{;val}", ";val=$bar", {/* not matched */}},
        TestParams{"{;val}", ";val=&bar", {/* not matched */}},
        TestParams{"{;val}", ";val='bar", {/* not matched */}},
        TestParams{"{;val}", ";val=(bar", {/* not matched */}},
        TestParams{"{;val}", ";val=)bar", {/* not matched */}},
        TestParams{"{;val}", ";val=*bar", {/* not matched */}},
        TestParams{"{;val}", ";val=+bar", {/* not matched */}},
        TestParams{"{;val}", ";val=;bar", {/* not matched */}},
        TestParams{"{;val}", ";val==bar", {/* not matched */}}
    )
);

INSTANTIATE_TEST_CASE_P(
    Question, TemplateNotMatch,
    ::testing::Values(
        TestParams{"{?val}", "?val=:", {/* not matched */}},
        TestParams{"{?val}", "?val=/", {/* not matched */}},
        TestParams{"{?val}", "?val=?", {/* not matched */}},
        TestParams{"{?val}", "?val=#", {/* not matched */}},
        TestParams{"{?val}", "?val=[", {/* not matched */}},
        TestParams{"{?val}", "?val=]", {/* not matched */}},
        TestParams{"{?val}", "?val=@", {/* not matched */}},
        TestParams{"{?val}", "?val=!", {/* not matched */}},
        TestParams{"{?val}", "?val=$", {/* not matched */}},
        TestParams{"{?val}", "?val=&", {/* not matched */}},
        TestParams{"{?val}", "?val='", {/* not matched */}},
        TestParams{"{?val}", "?val=(", {/* not matched */}},
        TestParams{"{?val}", "?val=)", {/* not matched */}},
        TestParams{"{?val}", "?val=*", {/* not matched */}},
        TestParams{"{?val}", "?val=+", {/* not matched */}},
        TestParams{"{?val}", "?val=;", {/* not matched */}},
        TestParams{"{?val}", "?val==", {/* not matched */}},
        TestParams{"{?val}", "foo", {/* not matched */}},
        TestParams{"{?val}", "?val=foo:", {/* not matched */}},
        TestParams{"{?val}", "?val=foo/", {/* not matched */}},
        TestParams{"{?val}", "?val=foo?", {/* not matched */}},
        TestParams{"{?val}", "?val=foo#", {/* not matched */}},
        TestParams{"{?val}", "?val=foo[", {/* not matched */}},
        TestParams{"{?val}", "?val=foo]", {/* not matched */}},
        TestParams{"{?val}", "?val=foo@", {/* not matched */}},
        TestParams{"{?val}", "?val=foo!", {/* not matched */}},
        TestParams{"{?val}", "?val=foo$", {/* not matched */}},
        TestParams{"{?val}", "?val=foo&", {/* not matched */}},
        TestParams{"{?val}", "?val=foo'", {/* not matched */}},
        TestParams{"{?val}", "?val=foo(", {/* not matched */}},
        TestParams{"{?val}", "?val=foo)", {/* not matched */}},
        TestParams{"{?val}", "?val=foo*", {/* not matched */}},
        TestParams{"{?val}", "?val=foo+", {/* not matched */}},
        TestParams{"{?val}", "?val=foo;", {/* not matched */}},
        TestParams{"{?val}", "?val=foo=", {/* not matched */}},
        TestParams{"{?val}", "?val=:bar", {/* not matched */}},
        TestParams{"{?val}", "?val=/bar", {/* not matched */}},
        TestParams{"{?val}", "?val=?bar", {/* not matched */}},
        TestParams{"{?val}", "?val=#bar", {/* not matched */}},
        TestParams{"{?val}", "?val=[bar", {/* not matched */}},
        TestParams{"{?val}", "?val=]bar", {/* not matched */}},
        TestParams{"{?val}", "?val=@bar", {/* not matched */}},
        TestParams{"{?val}", "?val=!bar", {/* not matched */}},
        TestParams{"{?val}", "?val=$bar", {/* not matched */}},
        TestParams{"{?val}", "?val=&bar", {/* not matched */}},
        TestParams{"{?val}", "?val='bar", {/* not matched */}},
        TestParams{"{?val}", "?val=(bar", {/* not matched */}},
        TestParams{"{?val}", "?val=)bar", {/* not matched */}},
        TestParams{"{?val}", "?val=*bar", {/* not matched */}},
        TestParams{"{?val}", "?val=+bar", {/* not matched */}},
        TestParams{"{?val}", "?val=;bar", {/* not matched */}},
        TestParams{"{?val}", "?val==bar", {/* not matched */}}
    )
);

INSTANTIATE_TEST_CASE_P(
    Ampersand, TemplateNotMatch,
    ::testing::Values(
        TestParams{"{&val}", "&val=:", {/* not matched */}},
        TestParams{"{&val}", "&val=/", {/* not matched */}},
        TestParams{"{&val}", "&val=?", {/* not matched */}},
        TestParams{"{&val}", "&val=#", {/* not matched */}},
        TestParams{"{&val}", "&val=[", {/* not matched */}},
        TestParams{"{&val}", "&val=]", {/* not matched */}},
        TestParams{"{&val}", "&val=@", {/* not matched */}},
        TestParams{"{&val}", "&val=!", {/* not matched */}},
        TestParams{"{&val}", "&val=$", {/* not matched */}},
        TestParams{"{&val}", "&val=&", {/* not matched */}},
        TestParams{"{&val}", "&val='", {/* not matched */}},
        TestParams{"{&val}", "&val=(", {/* not matched */}},
        TestParams{"{&val}", "&val=)", {/* not matched */}},
        TestParams{"{&val}", "&val=*", {/* not matched */}},
        TestParams{"{&val}", "&val=+", {/* not matched */}},
        TestParams{"{&val}", "&val=;", {/* not matched */}},
        TestParams{"{&val}", "&val==", {/* not matched */}},
        TestParams{"{&val}", "foo", {/* not matched */}},
        TestParams{"{&val}", "&val=foo:", {/* not matched */}},
        TestParams{"{&val}", "&val=foo/", {/* not matched */}},
        TestParams{"{&val}", "&val=foo?", {/* not matched */}},
        TestParams{"{&val}", "&val=foo#", {/* not matched */}},
        TestParams{"{&val}", "&val=foo[", {/* not matched */}},
        TestParams{"{&val}", "&val=foo]", {/* not matched */}},
        TestParams{"{&val}", "&val=foo@", {/* not matched */}},
        TestParams{"{&val}", "&val=foo!", {/* not matched */}},
        TestParams{"{&val}", "&val=foo$", {/* not matched */}},
        TestParams{"{&val}", "&val=foo&", {/* not matched */}},
        TestParams{"{&val}", "&val=foo'", {/* not matched */}},
        TestParams{"{&val}", "&val=foo(", {/* not matched */}},
        TestParams{"{&val}", "&val=foo)", {/* not matched */}},
        TestParams{"{&val}", "&val=foo*", {/* not matched */}},
        TestParams{"{&val}", "&val=foo+", {/* not matched */}},
        TestParams{"{&val}", "&val=foo;", {/* not matched */}},
        TestParams{"{&val}", "&val=foo=", {/* not matched */}},
        TestParams{"{&val}", "&val=:bar", {/* not matched */}},
        TestParams{"{&val}", "&val=/bar", {/* not matched */}},
        TestParams{"{&val}", "&val=?bar", {/* not matched */}},
        TestParams{"{&val}", "&val=#bar", {/* not matched */}},
        TestParams{"{&val}", "&val=[bar", {/* not matched */}},
        TestParams{"{&val}", "&val=]bar", {/* not matched */}},
        TestParams{"{&val}", "&val=@bar", {/* not matched */}},
        TestParams{"{&val}", "&val=!bar", {/* not matched */}},
        TestParams{"{&val}", "&val=$bar", {/* not matched */}},
        TestParams{"{&val}", "&val=&bar", {/* not matched */}},
        TestParams{"{&val}", "&val='bar", {/* not matched */}},
        TestParams{"{&val}", "&val=(bar", {/* not matched */}},
        TestParams{"{&val}", "&val=)bar", {/* not matched */}},
        TestParams{"{&val}", "&val=*bar", {/* not matched */}},
        TestParams{"{&val}", "&val=+bar", {/* not matched */}},
        TestParams{"{&val}", "&val=;bar", {/* not matched */}},
        TestParams{"{&val}", "&val==bar", {/* not matched */}}
    )
);

INSTANTIATE_TEST_CASE_P(
    Composite, TemplateNotMatch,
    ::testing::Values(
        TestParams{"{;val*}", ";val==foo", {/* not matched */}},
        TestParams{"{;val*}", ";val=foo!", {/* not matched */}},
        TestParams{"{;val*}", ";val=foo!val=bar", {/* not matched */}},
        TestParams{"{;val*}", ";not_val=foo,not_val=bar", {/* not matched */}},
        TestParams{"{;val*}", ";not_val=foo/not_val=bar", {/* not matched */}},
        TestParams{"{;val*}", ";not_val=foo?not_val=bar", {/* not matched */}},
        TestParams{"{;val*}", ";not_val=foo&not_val=bar", {/* not matched */}},
        TestParams{"{;val*}", ";not_val=foo.not_val=bar", {/* not matched */}},
        TestParams{"{;val*}", ";not_val=foo;not_val=bar", {/* not matched */}}
    )
);

INSTANTIATE_TEST_CASE_P(
    ProtoHostPath, TemplateNotMatch,
    ::testing::Values(
        TestParams{"{proto}://{www}tinkoff.ru/business{/node*}", "https://yandex.ru/business", {/* not matched */}},
        TestParams{"{proto}://{www}tinkoff.ru/business{/node*}", "https://tinkoff.ru/not_business", {/* not matched */}},
        TestParams{"{proto}://{www}tinkoff.ru/business{/node*}", "https://www.tinkoff.ru/not_business", {/* not matched */}},
        TestParams{"{proto}://{www}tinkoff.ru/business{/node*}", "https://www.tinkoff.ru/not_business/a/b/c/1/2/3", {/* not matched */}}
    )
);
// clang-format on

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
