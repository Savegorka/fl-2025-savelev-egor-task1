#include "automatic.h"
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>

TEST(LongestPrefixTest, ConcatBasic) {
    NDA nfa;
    nfa.Build("ab.");
    EXPECT_EQ(nfa.LongestPrefix("abc"), 2);
    EXPECT_EQ(nfa.LongestPrefix("a"), 1);
    EXPECT_EQ(nfa.LongestPrefix("b"), 0);
    EXPECT_EQ(nfa.LongestPrefix(""), 0);
}

TEST(LongestPrefixTest, UnionBasic) {
    NDA nfa;
    nfa.Build("ab+");
    EXPECT_EQ(nfa.LongestPrefix("ba"), 1);
    EXPECT_EQ(nfa.LongestPrefix("a"), 1);
    EXPECT_EQ(nfa.LongestPrefix("c"), 0);
}

TEST(LongestPrefixTest, KleeneStar) {
    NDA nfa;
    nfa.Build("a*");
    EXPECT_EQ(nfa.LongestPrefix("aaab"), 3);
    EXPECT_EQ(nfa.LongestPrefix(""), 0);
    EXPECT_EQ(nfa.LongestPrefix("b"), 0);
}

TEST(LongestPrefixTest, EpsilonOnly) {
    NDA nfa;
    nfa.Build("1");
    EXPECT_EQ(nfa.LongestPrefix("abc"), 0);
    EXPECT_EQ(nfa.LongestPrefix(""), 0);
}

TEST(LongestPrefixTest, EpsilonConcat) {
    NDA nfa;
    nfa.Build("1a.");
    EXPECT_EQ(nfa.LongestPrefix("ab"), 1);
    EXPECT_EQ(nfa.LongestPrefix("a"), 1);
    EXPECT_EQ(nfa.LongestPrefix("b"), 0);
}

TEST(LongestPrefixTest, EarlyStop) {
    NDA nfa;
    nfa.Build("ab.");
    EXPECT_EQ(nfa.LongestPrefix("czzz"), 0);
    EXPECT_EQ(nfa.LongestPrefix("ac"), 1);
}

TEST(LongestPrefixTest, ManyEpsilons) {
    NDA nfa;
    nfa.Build("11+*");
    EXPECT_EQ(nfa.LongestPrefix("aaaa"), 0);
    EXPECT_EQ(nfa.LongestPrefix(""), 0);
}

TEST(LongestPrefixTest, InvalidWordSymbol) {
    NDA nfa;
    nfa.Build("a*");
    EXPECT_THROW(nfa.LongestPrefix("aZ"), std::runtime_error);
}

TEST(BuildErrors, PlusNeedsTwo) {
    NDA nfa;
    EXPECT_THROW(nfa.Build("+"), std::runtime_error);
    EXPECT_THROW(nfa.Build("a+*"), std::runtime_error);
}

TEST(BuildErrors, DotNeedsTwo) {
    NDA nfa;
    EXPECT_THROW(nfa.Build("a."), std::runtime_error);
    EXPECT_THROW(nfa.Build("."), std::runtime_error);
}

TEST(BuildErrors, StarNeedsOne) {
    NDA nfa;
    EXPECT_THROW(nfa.Build("*"), std::runtime_error);
}

TEST(BuildErrors, LeftoverOnStack) {
    NDA nfa;
    EXPECT_THROW(nfa.Build("aa"), std::runtime_error);
    EXPECT_THROW(nfa.Build("ab"), std::runtime_error);
}

TEST(BuildErrors, UnknownToken) {
    NDA nfa;
    EXPECT_THROW(nfa.Build("aZ"), std::runtime_error);
}

TEST(LongestPrefixTest, LargerSmoke) {
    NDA nfa;
    nfa.Build("ab+c.*a1+.");
    EXPECT_EQ(nfa.LongestPrefix("acacba"), 5);
    EXPECT_EQ(nfa.LongestPrefix(""), 0);
    EXPECT_EQ(nfa.LongestPrefix("c"), 0);
}

TEST(LongestPrefixTest, FullWordStillLive)
{
    NDA nfa;
    nfa.Build("ab.");
    EXPECT_EQ(nfa.LongestPrefix("ab"), 2);
}

TEST(LongestPrefixTest, NoEpsilonClosurePath)
{
    NDA nfa;
    nfa.Build("ab+");
    EXPECT_EQ(nfa.LongestPrefix("c"), 0);
    EXPECT_EQ(nfa.LongestPrefix("a"), 1);
}

TEST(LongestPrefixTest, EpsilonHeavyChain)
{
    NDA nfa;
    nfa.Build("11+1+1+*a.");
    EXPECT_EQ(nfa.LongestPrefix("a"), 1);
    EXPECT_EQ(nfa.LongestPrefix(""), 0);
}

TEST(LongestPrefixTest, MoveOnNoEdges)
{
    NDA nfa;
    nfa.Build("a");
    EXPECT_EQ(nfa.LongestPrefix("b"), 0);
}

TEST(LongestPrefixTest, StartWithoutBuild)
{
    NDA nfa;
    EXPECT_EQ(nfa.LongestPrefix("abc"), 0);
}

TEST(BuildErrors, LoneOperandThenOperator)
{
    NDA nfa;
    EXPECT_THROW(nfa.Build("a*+"), std::runtime_error);
}

TEST(BuildErrors, GarbageMix)
{
    NDA nfa;
    EXPECT_THROW(nfa.Build("aZ*."), std::runtime_error);
}

TEST(LongestPrefixTest, EntireUNotLiveAtEnd)
{
    NDA nfa;
    nfa.Build("ab.");
    EXPECT_EQ(nfa.LongestPrefix("a"), 1);
    EXPECT_EQ(nfa.LongestPrefix("aba"), 2);
}

TEST(LongestPrefixTest, LiteralC)
{
    NDA nfa;
    nfa.Build("c");
    EXPECT_EQ(nfa.LongestPrefix("c"), 1);
    EXPECT_EQ(nfa.LongestPrefix("ca"), 1);
    EXPECT_EQ(nfa.LongestPrefix("a"), 0);
}

TEST(LongestPrefixTest, ComplexStillLiveAtEnd)
{
    NDA nfa;
    nfa.Build("ab.c+*ab..");
    EXPECT_EQ(nfa.LongestPrefix("acac"), 1);
    EXPECT_EQ(nfa.LongestPrefix("cabab"), 5);
}

TEST(LongestPrefixTest, DeepEpsilonLayers)
{
    NDA nfa;
    nfa.Build("11+1+1+1+*a.");
    EXPECT_EQ(nfa.LongestPrefix("aaaa"), 1);
    EXPECT_EQ(nfa.LongestPrefix("a"), 1);
}

TEST(LongestPrefixTest, NoEpsilonAtAll)
{
    NDA nfa;
    nfa.Build("ab.");
    EXPECT_EQ(nfa.LongestPrefix("a"), 1);
    EXPECT_EQ(nfa.LongestPrefix("b"), 0);
}

TEST(LongestPrefixTest, NonLiveAfterFullRead)
{
    NDA nfa;
    nfa.Build("ab+");
    EXPECT_EQ(nfa.LongestPrefix("aa"), 1);
    EXPECT_EQ(nfa.LongestPrefix("bb"), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
