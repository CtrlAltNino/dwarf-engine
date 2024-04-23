#include <Application/DwarfEngine.h>
#include <gtest/gtest.h>

TEST(DummyTests, One_Is_One)
{
  const auto expected = 1;
  //   const auto actual = multiply(1, 1);
  ASSERT_EQ(expected, 1);
}

int
main(int argc, char** argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}