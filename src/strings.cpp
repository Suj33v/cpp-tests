#include "catch.hpp"
#include <string>

TEST_CASE("sub-string test", "[string]")
{
  const std::string fileName = "test.mps";
  auto pos = fileName.find(".");
  auto nameWithoutExt = fileName.substr(0, pos);

  REQUIRE(nameWithoutExt == "test");
}

TEST_CASE("partial string comparison test", "[string]")
{
  std::string all = "abcdefg";
  std::string end = "defg";

  REQUIRE(all.compare(all.length() - end.length(), end.length(), end) == 0);
  REQUIRE(std::equal(end.rbegin(), end.rend(), all.rbegin()) == true);
}
