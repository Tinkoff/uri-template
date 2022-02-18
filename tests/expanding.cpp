#include "fixtures.h"

TEST_P(TemplateExpand, Test)
{
    ASSERT_TRUE(Expanded(GetParam()));
}

// clang-format off
INSTANTIATE_TEST_CASE_P(
    Level1, TemplateExpand,
    ::testing::Values(
        TestParams{"{var}", "value",
                   {{"var", URI::Template::VarValue("value")}}
        },
        TestParams{"{undef}", "",
                   {{"undef", URI::Template::VarValue()}}
        },
        TestParams{"{undef}", "", {}},
        TestParams{"O{undef}X", "OX",
                   {{"undef", URI::Template::VarValue()}}
        },
        TestParams{"O{empty}X", "OX",
                   {{"empty", URI::Template::VarValue("")}}
        },
        TestParams{"asd{var}asd", "asdvalueasd",
                   {{"var", URI::Template::VarValue("value")}}
        },
        TestParams{"asd", "asd", {}
        },
        TestParams{"{hello}", "Hello%20World%21",
                   {{"hello", URI::Template::VarValue("Hello World!")}}
        }
    )
);

INSTANTIATE_TEST_CASE_P(
    Level2, TemplateExpand,
    ::testing::Values(
        TestParams{"{+var}", "value",
                   {{"var", URI::Template::VarValue("value")}}
        },
        TestParams{"{+hello}", "Hello%20World!",
                   {{"hello", URI::Template::VarValue("Hello World!")}}
        },
        TestParams{"{+path}/here", "/foo/bar/here",
                   {{"path", URI::Template::VarValue("/foo/bar")}}
        },
        TestParams{"here?ref={+path}", "here?ref=/foo/bar",
                   {{"path", URI::Template::VarValue("/foo/bar")}}
        }
    )
);

INSTANTIATE_TEST_CASE_P(
    Level3, TemplateExpand,
    ::testing::Values(
        TestParams{"{+var}", "value",
                   {{"var", URI::Template::VarValue("value")}}
        },
        TestParams{"map?{x,y}", "map?1024,768",
                   {{"x", URI::Template::VarValue("1024")},
                    {"y", URI::Template::VarValue("768")}}
        },
        TestParams{"{x,hello,y}", "1024,Hello%20World%21,768",
                   {{"x", URI::Template::VarValue("1024")},
                    {"hello", URI::Template::VarValue("Hello World!")},
                    {"y", URI::Template::VarValue("768")}}
        },
        TestParams{"{+x,hello,y}", "1024,Hello%20World!,768",
                   {{"x", URI::Template::VarValue("1024")},
                    {"hello", URI::Template::VarValue("Hello World!")},
                    {"y", URI::Template::VarValue("768")}}
        },
        TestParams{"{+path,x}/here", "/foo/bar,1024/here",
                   {{"path", URI::Template::VarValue("/foo/bar")},
                    {"x", URI::Template::VarValue("1024")}}
        },
        TestParams{"{#x,hello,y}", "#1024,Hello%20World!,768",
                   {{"x", URI::Template::VarValue("1024")},
                    {"hello", URI::Template::VarValue("Hello World!")},
                    {"y", URI::Template::VarValue("768")}}
        },
        TestParams{"{#path,x}/here", "#/foo/bar,1024/here",
                   {{"path", URI::Template::VarValue("/foo/bar")},
                    {"x", URI::Template::VarValue("1024")}}
        },
        TestParams{"X{.var}", "X.value",
                   {{"var", URI::Template::VarValue("value")}}
        },
        TestParams{"X{.x,y}", "X.1024.768",
                   {{"x", URI::Template::VarValue("1024")},
                    {"y", URI::Template::VarValue("768")}}
        },
        TestParams{"{/var}", "/value",
                   {{"var", URI::Template::VarValue("value")}}
        },
        TestParams{"{/var,x}/here", "/value/1024/here",
                   {{"var", URI::Template::VarValue("value")},
                    {"x", URI::Template::VarValue("1024")}}
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
        TestParams{"{?x,y}", "?x=1024&y=768",
                   {{"x", URI::Template::VarValue("1024")},
                    {"y", URI::Template::VarValue("768")}}
        },
        TestParams{"{?x,y,empty}", "?x=1024&y=768&empty=",
                   {{"x", URI::Template::VarValue("1024")},
                    {"y", URI::Template::VarValue("768")},
                    {"empty", URI::Template::VarValue("")}}
        },
        TestParams{"?fixed=yes{&x}", "?fixed=yes&x=1024",
                   {{"x", URI::Template::VarValue("1024")}}
        },
        TestParams{"{&x,y,empty}", "&x=1024&y=768&empty=",
                   {{"x", URI::Template::VarValue("1024")},
                    {"y", URI::Template::VarValue("768")},
                    {"empty", URI::Template::VarValue("")}}
        }
    )
);

INSTANTIATE_TEST_CASE_P(
    Level4, TemplateExpand,
    ::testing::Values(
        TestParams{"{var:3}", "val",
                   {{"var", URI::Template::VarValue("value")}}
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
        TestParams{"{keys}", "comma,%2C,dot,.,semi,%3B",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"semi", ";"},
                                                                                                  {"dot", "."},
                                                                                                  {"comma", ","}})}}
        },
        TestParams{"{keys*}", "comma=%2C,dot=.,semi=%3B",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"semi", ";"},
                                                                                                  {"dot", "."},
                                                                                                  {"comma", ","}})}}
        },
        TestParams{"{base}index", "http%3A%2F%2Fexample.com%2Fhome%2Findex",
                   {{"base", URI::Template::VarValue("http://example.com/home/")}}
        },
        TestParams{"{+base}index", "http://example.com/home/index",
                   {{"base", URI::Template::VarValue("http://example.com/home/")}}
        },
        TestParams{"{+path:6}/here", "/foo/b/here",
                   {{"path", URI::Template::VarValue("/foo/bar")}}
        },
        TestParams{"{+list}", "red,green,blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{+list*}", "red,green,blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{+keys}", "comma,,,dot,.,semi,;",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"semi", ";"},
                                                                                                  {"dot", "."},
                                                                                                  {"comma", ","}})}}
        },
        TestParams{"{+keys*}", "comma=,,dot=.,semi=;",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"semi", ";"},
                                                                                                  {"dot", "."},
                                                                                                  {"comma", ","}})}}
        },
        TestParams{"{#var}", "#value",
                   {{"var", URI::Template::VarValue("value")}}
        },
        TestParams{"{#hello}", "#Hello%20World!",
                   {{"hello", URI::Template::VarValue("Hello World!")}}
        },
        TestParams{"foo{#empty}", "foo#",
                   {{"empty", URI::Template::VarValue("")}}
        },
        TestParams{"foo{#undef}", "foo",
                   {{"undef", URI::Template::VarValue()}}
        },
        TestParams{"{#path:6}/here", "#/foo/b/here",
                   {{"path", URI::Template::VarValue("/foo/bar")}}
        },
        TestParams{"{#list}", "#red,green,blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{#list*}", "#red,green,blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{#keys}", "#comma,,,dot,.,semi,;",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"semi", ";"},
                                                                                                  {"dot", "."},
                                                                                                  {"comma", ","}})}}
        },
        TestParams{"{#keys*}", "#comma=,,dot=.,semi=;",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"semi", ";"},
                                                                                                  {"dot", "."},
                                                                                                  {"comma", ","}})}}
        },
        TestParams{"X{.var:3}", "X.val",
                   {{"var", URI::Template::VarValue("value")}}
        },
        TestParams{"X{.empty}", "X.",
                   {{"empty", URI::Template::VarValue("")}}
        },
        TestParams{"X{.undef}", "X",
                   {{"undef", URI::Template::VarValue()}}
        },
        TestParams{"X{.list}", "X.red,green,blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"X{.list*}", "X.red.green.blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"X{.keys}", "X.comma,%2C,dot,.,semi,%3B",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"semi", ";"},
                                                                                                  {"dot", "."},
                                                                                                  {"comma", ","}})}}
        },
        TestParams{"{/var:1,var}", "/v/value",
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
        TestParams{"{/list}", "/red,green,blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{/list*}", "/red/green/blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{/list*,path:4}", "/red/green/blue/%2Ffoo",
                   {{"path", URI::Template::VarValue("/foo/bar")},
                    {"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{/keys}", "/comma,%2C,dot,.,semi,%3B",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"semi", ";"},
                                                                                                  {"dot", "."},
                                                                                                  {"comma", ","}})}}
        },
        TestParams{"{/keys*}", "/comma=%2C/dot=./semi=%3B",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"semi", ";"},
                                                                                                  {"dot", "."},
                                                                                                  {"comma", ","}})}}
        },
        TestParams{"{;hello:5}", ";hello=Hello",
                   {{"hello", URI::Template::VarValue("Hello World!")}}
        },
        TestParams{"{;list}", ";list=red,green,blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{;list*}", ";list=red;list=green;list=blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{;keys}", ";keys=comma,%2C,dot,.,semi,%3B",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"semi", ";"},
                                                                                                  {"dot", "."},
                                                                                                  {"comma", ","}})}}
        },
        TestParams{"{;keys*}", ";comma=%2C;dot=.;semi=%3B",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"semi", ";"},
                                                                                                  {"dot", "."},
                                                                                                  {"comma", ","}})}}
        },
        TestParams{"{?var:3}", "?var=val",
                   {{"var", URI::Template::VarValue("value")}}
        },
        TestParams{"{?list}", "?list=red,green,blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{?list*}", "?list=red&list=green&list=blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{?keys}", "?keys=comma,%2C,dot,.,semi,%3B",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"semi", ";"},
                                                                                                  {"dot", "."},
                                                                                                  {"comma", ","}})}}
        },
        TestParams{"{?keys*}", "?comma=%2C&dot=.&semi=%3B",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"semi", ";"},
                                                                                                  {"dot", "."},
                                                                                                  {"comma", ","}})}}
        },
        TestParams{"{&var:3}", "&var=val",
                   {{"var", URI::Template::VarValue("value")}}
        },
        TestParams{"{&list}", "&list=red,green,blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{&list*}", "&list=red&list=green&list=blue",
                   {{"list", URI::Template::VarValue(std::vector<std::string>{"red", "green", "blue"})}}
        },
        TestParams{"{&keys}", "&keys=comma,%2C,dot,.,semi,%3B",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"semi", ";"},
                                                                                                  {"dot", "."},
                                                                                                  {"comma", ","}})}}
        },
        TestParams{"{&keys*}", "&comma=%2C&dot=.&semi=%3B",
                   {{"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"semi", ";"},
                                                                                                  {"dot", "."},
                                                                                                  {"comma", ","}})}}
        }
    )
);

INSTANTIATE_TEST_CASE_P(
    ExtraLevel4, TemplateExpand,
    ::testing::Values(
        TestParams{"{/id*}{?fields,first_name,last.name,token}", "/person?fields=id,name,picture&first_name=John&last.name=Doe&token=12345",
                   {{"id", URI::Template::VarValue("person")},
                    {"fields", URI::Template::VarValue(std::vector<std::string>{"id", "name", "picture"})},
                    {"first_name", URI::Template::VarValue("John")},
                    {"last.name", URI::Template::VarValue("Doe")},
                    {"token", URI::Template::VarValue("12345")}}
        },
        TestParams{"/search.{format}{?q,geo,lang,locale,page,result_type}", "/search.json?q=URI%20Templates&geo=37.76,-122.427&lang=en&page=5",
                   {{"format", URI::Template::VarValue("json")},
                    {"q", URI::Template::VarValue("URI Templates")},
                    {"geo", URI::Template::VarValue(std::vector<std::string>{"37.76", "-122.427"})},
                    {"lang", URI::Template::VarValue("en")},
                    {"page", URI::Template::VarValue("5")}}
        },
        TestParams{"/test{/Some%20Thing}", "/test/foo",
                   {{"Some%20Thing", URI::Template::VarValue("foo")}}
        },
        TestParams{"/base{/group_id,first_name}/pages{/page,lang}{?format,q}", "/base/12345/John/pages/5/en?format=json&q=URI%20Templates",
                   {{"group_id", URI::Template::VarValue("12345")},
                    {"first_name", URI::Template::VarValue("John")},
                    {"format", URI::Template::VarValue("json")},
                    {"q", URI::Template::VarValue("URI Templates")},
                    {"lang", URI::Template::VarValue("en")},
                    {"page", URI::Template::VarValue("5")}}
        },
        TestParams{"/sparql{?query}", "/sparql?query=PREFIX%20dc%3A%20%3Chttp%3A%2F%2Fpurl.org%2Fdc%2Felements%2F1.1%2F%3E%20SELECT%20%3Fbook%20%3Fwho%20WHERE%20%7B%20%3Fbook%20dc%3Acreator%20%3Fwho%20%7D",
                   {{"query", URI::Template::VarValue("PREFIX dc: <http://purl.org/dc/elements/1.1/> SELECT ?book ?who WHERE { ?book dc:creator ?who }")}}
        },
        TestParams{"/go{?uri}", "/go?uri=http%3A%2F%2Fexample.org%2F%3Furi%3Dhttp%3A%2F%2Fexample.org%2F",
                   {{"uri", URI::Template::VarValue("http://example.org/?uri=http%3A%2F%2Fexample.org%2F")}}
        },
        TestParams{"/service{?word}", "/service?word=dr%C3%BCcken",
                   {{"word", URI::Template::VarValue("drücken")}}
        },

        TestParams{"/lookup{?Stra%C3%9Fe}", "/lookup?Stra%C3%9Fe=Gr%C3%BCner%20Weg",
                   {{"Stra%C3%9Fe", URI::Template::VarValue("Grüner Weg")}}
        },

        TestParams{"{random}", "%C5%A1%C3%B6%C3%A4%C5%B8%C5%93%C3%B1%C3%AA%E2%82%AC%C2%A3%C2%A5%E2%80%A1%C3%91%C3%92%C3%93%C3%94%C3%95%C3%96%C3%97%C3%98%C3%99%C3%9A%C3%A0%C3%A1%C3%A2%C3%A3%C3%A4%C3%A5%C3%A6%C3%A7%C3%BF",
                   {{"random", URI::Template::VarValue("šöäŸœñê€£¥‡ÑÒÓÔÕÖ×ØÙÚàáâãäåæçÿ")}}
        },
        TestParams{"{?assoc_special_chars*}", "?%C5%A1%C3%B6%C3%A4%C5%B8%C5%93%C3%B1%C3%AA%E2%82%AC%C2%A3%C2%A5%E2%80%A1%C3%91%C3%92%C3%93%C3%94%C3%95=%C3%96%C3%97%C3%98%C3%99%C3%9A%C3%A0%C3%A1%C3%A2%C3%A3%C3%A4%C3%A5%C3%A6%C3%A7%C3%BF",
                   {{"assoc_special_chars", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"šöäŸœñê€£¥‡ÑÒÓÔÕ", "Ö×ØÙÚàáâãäåæçÿ"}})}}
        }
    )
);

INSTANTIATE_TEST_CASE_P(
    ExtraExplode, TemplateExpand,
    ::testing::Values(
        TestParams{"{?id,token,keys*}", "?id=admin&token=12345&key1=val1&key2=val2",
                   {{"id", URI::Template::VarValue("admin")},
                    {"token", URI::Template::VarValue("12345")},
                    {"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"key2", "val2"},
                                                                                                  {"key1", "val1"}})}}
        },
        TestParams{"{/id}{?token,keys*}", "/admin?token=12345&key1=val1&key2=val2",
                   {{"id", URI::Template::VarValue("admin")},
                    {"token", URI::Template::VarValue("12345")},
                    {"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"key2", "val2"},
                                                                                                  {"key1", "val1"}})}}
        },
        TestParams{"{?id,token}{&keys*}", "?id=admin&token=12345&key1=val1&key2=val2",
                   {{"id", URI::Template::VarValue("admin")},
                    {"token", URI::Template::VarValue("12345")},
                    {"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"key2", "val2"},
                                                                                                  {"key1", "val1"}})}}
        },
        TestParams{"/user{/id}{?token,tab}{&keys*}", "/user/admin?token=12345&tab=overview&key1=val1&key2=val2",
                   {{"id", URI::Template::VarValue("admin")},
                    {"token", URI::Template::VarValue("12345")},
                    {"tab", URI::Template::VarValue("overview")},
                    {"keys", URI::Template::VarValue(std::unordered_map<std::string, std::string>{{"key2", "val2"},
                                                                                                  {"key1", "val1"}})}}
        }
    )
);
// clang-format on

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
