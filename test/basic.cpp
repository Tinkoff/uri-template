#include "fixtures.h"

TEST(IsTemplated, Test)
{
    ASSERT_FALSE(URI::Template::ParseTemplate("").IsTemplated());
    ASSERT_FALSE(URI::Template::ParseTemplate("foobar").IsTemplated());

    ASSERT_TRUE(URI::Template::ParseTemplate("{var}").IsTemplated());
    ASSERT_TRUE(URI::Template::ParseTemplate("{var*}").IsTemplated());
    ASSERT_TRUE(URI::Template::ParseTemplate("{.val}").IsTemplated());
    ASSERT_TRUE(URI::Template::ParseTemplate("{.val*}").IsTemplated());
    ASSERT_TRUE(URI::Template::ParseTemplate("{/val}").IsTemplated());
    ASSERT_TRUE(URI::Template::ParseTemplate("{/val*}").IsTemplated());
    ASSERT_TRUE(URI::Template::ParseTemplate("{;val}").IsTemplated());
    ASSERT_TRUE(URI::Template::ParseTemplate("{;val*}").IsTemplated());
    ASSERT_TRUE(URI::Template::ParseTemplate("{?val}").IsTemplated());
    ASSERT_TRUE(URI::Template::ParseTemplate("{?val*}").IsTemplated());
    ASSERT_TRUE(URI::Template::ParseTemplate("{&val}").IsTemplated());
    ASSERT_TRUE(URI::Template::ParseTemplate("{&val*}").IsTemplated());
}

TEST(LiteralCompare, Test)
{
    const auto literal1 = URI::Template::ParseTemplate("foo")[0].Get<URI::Template::Literal>();
    const auto literal2 = URI::Template::ParseTemplate("foo")[0].Get<URI::Template::Literal>();
    const auto literal3 = URI::Template::ParseTemplate("bar")[0].Get<URI::Template::Literal>();

    ASSERT_TRUE(literal1 == literal2);
    ASSERT_TRUE(literal1 != literal3);
}

TEST(ExpressionCompare, Test)
{
    const auto expression1 = URI::Template::ParseTemplate("{var}")[0].Get<URI::Template::Expression>();
    const auto expression2 = URI::Template::ParseTemplate("{var}")[0].Get<URI::Template::Expression>();
    const auto expression3 = URI::Template::ParseTemplate("{val}")[0].Get<URI::Template::Expression>();
    const auto expression4 = URI::Template::ParseTemplate("{/val}")[0].Get<URI::Template::Expression>();
    const auto expression5 = URI::Template::ParseTemplate("{+val}")[0].Get<URI::Template::Expression>();
    const auto expression6 = URI::Template::ParseTemplate("{.val}")[0].Get<URI::Template::Expression>();
    const auto expression7 = URI::Template::ParseTemplate("{;val}")[0].Get<URI::Template::Expression>();
    const auto expression8 = URI::Template::ParseTemplate("{?val}")[0].Get<URI::Template::Expression>();
    const auto expression9 = URI::Template::ParseTemplate("{&val}")[0].Get<URI::Template::Expression>();
    const auto expression10 = URI::Template::ParseTemplate("{#val}")[0].Get<URI::Template::Expression>();

    ASSERT_TRUE(expression1 == expression2);
    ASSERT_TRUE(expression1 != expression3);
    ASSERT_TRUE(expression1 != expression4);
    ASSERT_TRUE(expression1 != expression5);
    ASSERT_TRUE(expression1 != expression6);
    ASSERT_TRUE(expression1 != expression7);
    ASSERT_TRUE(expression1 != expression8);
    ASSERT_TRUE(expression1 != expression9);
    ASSERT_TRUE(expression1 != expression10);
}

TEST(PartsSize, Test)
{
    const auto uri_template = URI::Template::ParseTemplate("foo{var}bar");
    ASSERT_EQ(uri_template.Size(), 3);
    ASSERT_EQ(uri_template.Parts().size(), uri_template.Size());
}

TEST(PartsCompare, Test)
{
    auto uri_template1 = URI::Template::ParseTemplate("foo{var}bar");
    const auto const_uri_template = uri_template1;
    for (std::size_t i = 0; i < uri_template1.Size(); ++i) {
        ASSERT_EQ(const_uri_template[i], uri_template1[i]);
    }
    ASSERT_EQ(const_uri_template.Parts(), uri_template1.Parts());

    const auto uri_template2 = URI::Template::ParseTemplate("foo{var}bar");
    ASSERT_EQ(uri_template1.Parts(), uri_template1.Parts());

    const auto uri_template3 = URI::Template::ParseTemplate("bar{var}bar");
    const auto uri_template4 = URI::Template::ParseTemplate("foo{var}foo");
    const auto uri_template5 = URI::Template::ParseTemplate("foo{var1}bar");
    const auto uri_template6 = URI::Template::ParseTemplate("foo{/var}bar");
    ASSERT_NE(uri_template1.Parts(), uri_template3.Parts());
    ASSERT_NE(uri_template1.Parts(), uri_template4.Parts());
    ASSERT_NE(uri_template1.Parts(), uri_template5.Parts());
    ASSERT_NE(uri_template1.Parts(), uri_template6.Parts());

    const auto uri_template7 = URI::Template::ParseTemplate("asd{/variable}qwe");
    ASSERT_EQ(uri_template1.Size(), uri_template7.Size());
    for (std::size_t i = 0; i < uri_template1.Size(); ++i) {
        ASSERT_TRUE(uri_template1[i] != uri_template7[i]);
    }
}

TEST(OperatorDefinition, Test)
{
    const auto op_noop = URI::Template::OpNoop();
    ASSERT_EQ(op_noop.Type(), URI::Template::OperatorType::NONE);
    ASSERT_THROW(op_noop.Start(), std::runtime_error);
    ASSERT_EQ(op_noop.First(), URI::Template::Operator::kNoCharacter);
    ASSERT_EQ(op_noop.Separator(), ',');
    ASSERT_EQ(op_noop.Named(), false);
    ASSERT_EQ(op_noop.EmptyEq(), false);
    ASSERT_EQ(op_noop.Reserved(), false);
    ASSERT_EQ(op_noop.StartExpanded(), false);

    const auto op_reserved_chars = URI::Template::OpReservedChars();
    ASSERT_EQ(op_reserved_chars.Type(), URI::Template::OperatorType::RESERVED_CHARS);
    ASSERT_EQ(op_reserved_chars.Start(), '+');
    ASSERT_EQ(op_reserved_chars.First(), URI::Template::Operator::kNoCharacter);
    ASSERT_EQ(op_reserved_chars.Separator(), ',');
    ASSERT_EQ(op_reserved_chars.Named(), false);
    ASSERT_EQ(op_reserved_chars.EmptyEq(), false);
    ASSERT_EQ(op_reserved_chars.Reserved(), true);
    ASSERT_EQ(op_reserved_chars.StartExpanded(), false);

    const auto op_fragment = URI::Template::OpFragment();
    ASSERT_EQ(op_fragment.Type(), URI::Template::OperatorType::FRAGMENT);
    ASSERT_EQ(op_fragment.Start(), '#');
    ASSERT_EQ(op_fragment.First(), '#');
    ASSERT_EQ(op_fragment.Separator(), ',');
    ASSERT_EQ(op_fragment.Named(), false);
    ASSERT_EQ(op_fragment.EmptyEq(), false);
    ASSERT_EQ(op_fragment.Reserved(), true);
    ASSERT_EQ(op_fragment.StartExpanded(), true);

    const auto op_label = URI::Template::OpLabel();
    ASSERT_EQ(op_label.Type(), URI::Template::OperatorType::LABEL);
    ASSERT_EQ(op_label.Start(), '.');
    ASSERT_EQ(op_label.First(), '.');
    ASSERT_EQ(op_label.Separator(), '.');
    ASSERT_EQ(op_label.Named(), false);
    ASSERT_EQ(op_label.EmptyEq(), false);
    ASSERT_EQ(op_label.Reserved(), false);
    ASSERT_EQ(op_label.StartExpanded(), true);

    const auto op_path = URI::Template::OpPath();
    ASSERT_EQ(op_path.Type(), URI::Template::OperatorType::PATH);
    ASSERT_EQ(op_path.Start(), '/');
    ASSERT_EQ(op_path.First(), '/');
    ASSERT_EQ(op_path.Separator(), '/');
    ASSERT_EQ(op_path.Named(), false);
    ASSERT_EQ(op_path.EmptyEq(), false);
    ASSERT_EQ(op_path.Reserved(), false);
    ASSERT_EQ(op_path.StartExpanded(), true);

    const auto op_path_param = URI::Template::OpPathParam();
    ASSERT_EQ(op_path_param.Type(), URI::Template::OperatorType::PATH_PARAMETER);
    ASSERT_EQ(op_path_param.Start(), ';');
    ASSERT_EQ(op_path_param.First(), ';');
    ASSERT_EQ(op_path_param.Separator(), ';');
    ASSERT_EQ(op_path_param.Named(), true);
    ASSERT_EQ(op_path_param.EmptyEq(), false);
    ASSERT_EQ(op_path_param.Reserved(), false);
    ASSERT_EQ(op_path_param.StartExpanded(), true);

    const auto op_query = URI::Template::OpQuery();
    ASSERT_EQ(op_query.Type(), URI::Template::OperatorType::QUERY);
    ASSERT_EQ(op_query.Start(), '?');
    ASSERT_EQ(op_query.First(), '?');
    ASSERT_EQ(op_query.Separator(), '&');
    ASSERT_EQ(op_query.Named(), true);
    ASSERT_EQ(op_query.EmptyEq(), true);
    ASSERT_EQ(op_query.Reserved(), false);
    ASSERT_EQ(op_query.StartExpanded(), true);

    const auto op_query_continue = URI::Template::OpQueryContinue();
    ASSERT_EQ(op_query_continue.Type(), URI::Template::OperatorType::QUERY_CONTINUE);
    ASSERT_EQ(op_query_continue.Start(), '&');
    ASSERT_EQ(op_query_continue.First(), '&');
    ASSERT_EQ(op_query_continue.Separator(), '&');
    ASSERT_EQ(op_query_continue.Named(), true);
    ASSERT_EQ(op_query_continue.EmptyEq(), true);
    ASSERT_EQ(op_query_continue.Reserved(), false);
    ASSERT_EQ(op_query_continue.StartExpanded(), true);
}

TEST(ModifierDefinition, Test)
{
    const auto mod_noop = URI::Template::ModNoop();
    ASSERT_EQ(mod_noop.Type(), URI::Template::ModifierType::NONE);
    ASSERT_THROW(mod_noop.Start(), std::runtime_error);

    const auto mod_length = URI::Template::ModLength();
    ASSERT_EQ(mod_length.Type(), URI::Template::ModifierType::LENGTH);
    ASSERT_EQ(mod_length.Start(), ':');

    const auto mod_explode = URI::Template::ModExplode();
    ASSERT_EQ(mod_explode.Type(), URI::Template::ModifierType::EXPLODE);
    ASSERT_EQ(mod_explode.Start(), '*');
}

TEST(VariableType, Test)
{
    const auto var = URI::Template::Variable("var", std::make_shared<URI::Template::ModNoop>(), 0);

    ASSERT_EQ(var.IsPrefixed(), false);
    ASSERT_EQ(var.IsExploded(), false);
    ASSERT_EQ(var.Name(), "var");

    const auto& mod = var.Mod();
    ASSERT_EQ(mod.Type(), URI::Template::ModifierType::NONE);
}

TEST(VariableCompare, Test)
{
    const auto var1 = URI::Template::Variable("var", nullptr, 0);
    const auto var2 = URI::Template::Variable("var", nullptr, 0);
    const auto var3 = URI::Template::Variable("var3", nullptr, 0);
    const auto var4 = URI::Template::Variable("var", std::make_shared<URI::Template::ModLength>(), 0);
    const auto var5 = URI::Template::Variable("var", std::make_shared<URI::Template::ModLength>(), 100);
    const auto var6 = URI::Template::Variable("var", std::make_shared<URI::Template::ModExplode>(), 0);

    ASSERT_TRUE(var1 == var2);
    ASSERT_TRUE(var1 != var3);
    ASSERT_TRUE(var1 != var4);
    ASSERT_TRUE(var1 != var5);
    ASSERT_TRUE(var1 != var6);
}

TEST(ValueString, Test)
{
    auto value1 = URI::Template::VarValue(URI::Template::VarType::STRING);
    ASSERT_EQ(value1.Type(), URI::Template::VarType::STRING);
    ASSERT_EQ(value1.Get<std::string>(), "");

    auto value2 = URI::Template::VarValue("");
    ASSERT_EQ(value2.Type(), URI::Template::VarType::STRING);
    ASSERT_EQ(value2.Get<std::string>(), "");

    ASSERT_EQ(value1, value2);

    ASSERT_THROW(value2.Get<std::vector<std::string>>(), std::bad_variant_access);
    ASSERT_THROW((value2.Get<std::unordered_map<std::string, std::string>>()), std::bad_variant_access);
}

TEST(ValueList, Test)
{
    auto value1 = URI::Template::VarValue(URI::Template::VarType::LIST);
    ASSERT_EQ(value1.Type(), URI::Template::VarType::LIST);
    ASSERT_EQ(value1.Get<std::vector<std::string>>(), std::vector<std::string>());

    auto value2 = URI::Template::VarValue(std::vector<std::string>());
    ASSERT_EQ(value2.Type(), URI::Template::VarType::LIST);
    ASSERT_EQ(value1.Get<std::vector<std::string>>(), std::vector<std::string>());

    ASSERT_EQ(value1, value2);

    ASSERT_THROW(value2.Get<std::string>(), std::bad_variant_access);
    ASSERT_THROW((value2.Get<std::unordered_map<std::string, std::string>>()), std::bad_variant_access);
}

TEST(ValueDict, Test)
{
    auto value1 = URI::Template::VarValue(URI::Template::VarType::DICT);
    ASSERT_EQ(value1.Type(), URI::Template::VarType::DICT);
    ASSERT_EQ((value1.Get<std::unordered_map<std::string, std::string>>()),
              (std::unordered_map<std::string, std::string>()));

    auto value2 = URI::Template::VarValue(std::unordered_map<std::string, std::string>());
    ASSERT_EQ(value2.Type(), URI::Template::VarType::DICT);
    ASSERT_EQ((value1.Get<std::unordered_map<std::string, std::string>>()),
              (std::unordered_map<std::string, std::string>()));

    ASSERT_EQ(value1, value2);

    ASSERT_THROW(value2.Get<std::string>(), std::bad_variant_access);
    ASSERT_THROW(value2.Get<std::vector<std::string>>(), std::bad_variant_access);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
