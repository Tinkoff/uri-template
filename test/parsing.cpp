#include "fixtures.h"

TEST_P(TemplateNotParse, Test)
{
    ASSERT_TRUE(NotParsed(GetParam()));
}

// clang-format off
INSTANTIATE_TEST_CASE_P(
    Simple, TemplateNotParse,
    ::testing::Values(
        TestParams{"{}", "", {/* not parsed */}},
        TestParams{"{/id*", "", {/* not parsed */}},
        TestParams{"/id*}", "", {/* not parsed */}},
        TestParams{"{/?id}", "", {/* not parsed */}},
        TestParams{"{var:prefix}", "", {/* not parsed */}},
        TestParams{"{hello:2*}", "", {/* not parsed */}},
        TestParams{"{??hello}", "", {/* not parsed */}},
        TestParams{"{!hello}", "", {/* not parsed */}},
        TestParams{"{with space}", "", {/* not parsed */}},
        TestParams{"{ leading_space}", "", {/* not parsed */}},
        TestParams{"{trailing_space }", "", {/* not parsed */}},
        TestParams{"{=path}", "", {/* not parsed */}},
        TestParams{"{$var}", "", {/* not parsed */}},
        TestParams{"{|var*}", "", {/* not parsed */}},
        TestParams{"{*keys?}", "", {/* not parsed */}},
        TestParams{"{?empty=default,var}", "", {/* not parsed */}},
        TestParams{"{var}{-prefix|/-/|var}" , "", {/* not parsed */}},
        TestParams{"?q={searchTerms}&amp;c={example:color?}" , "", {/* not parsed */}},
        TestParams{"x{?empty|foo=none}" , "", {/* not parsed */}},
        TestParams{"/h{#hello+}" , "", {/* not parsed */}},
        TestParams{"/h#{hello+}" , "", {/* not parsed */}},
        TestParams{"{;keys:1*}", "", {/* not parsed */}},
        TestParams{"?{-join|&|var,list}" , "", {/* not parsed */}},
        TestParams{"/people/{~thing}", "", {/* not parsed */}},
        TestParams{"/{default-graph-uri}", "", {/* not parsed */}},
        TestParams{"/sparql{?query,default-graph-uri}", "", {/* not parsed */}},
        TestParams{"/sparql{?query){&default-graph-uri*}", "", {/* not parsed */}},
        TestParams{"/resolution{?x, y}" , "", {/* not parsed */}},
        TestParams{"{var:3000000000000}" , "", {/* not parsed */}}
    )
);
// clang-format on

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
