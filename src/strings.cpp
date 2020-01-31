#include "catch.hpp"
#include <string>

TEST_CASE("sub-string test", "[string]") {
  const std::string fileName = "test.mps";
  auto pos = fileName.find(".");
  auto nameWithoutExt = fileName.substr(0, pos);

  REQUIRE(nameWithoutExt == "test");
}

TEST_CASE("partial string comparison test", "[string]") {
  std::string fullString = "abcdefg";
  std::string ending = "defg";

  REQUIRE(fullString.compare(fullString.length() - ending.length(), ending.length(), ending) == 0);
  REQUIRE(std::equal(ending.rbegin(), ending.rend(), fullString.rbegin()) == true);
}
