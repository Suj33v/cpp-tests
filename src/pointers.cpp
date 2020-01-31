#include "catch.hpp"
#include <memory>
#include <string>

TEST_CASE("shared pointer tests", "[ptrs]") {
  auto num = std::shared_ptr<int>{ new int{ 1 } };

  auto num2 = num;
  REQUIRE(*num2 == 1);

  auto fn = [num](std::shared_ptr<int>& localNum) {
    localNum = num;
  };
  std::shared_ptr<int> num3(nullptr);
  fn(num3);

  REQUIRE(*num == 1);
  REQUIRE(*num2 == 1);
  REQUIRE(*num3 == 1);
}

TEST_CASE("unique pointer construction test", "[ptrs]") {
  auto upv = std::unique_ptr<std::vector<int>>{ nullptr };
  if(!upv) {
    upv.reset(new std::vector<int>);
    upv->push_back(1);
  }
  REQUIRE(upv->at(0) == 1);
}

TEST_CASE("unique pointer passing test", "[ptrs]") {
  auto num = std::unique_ptr<int>{ new int{ 1 } };
  auto fn = [](int& v) { return v; };
  REQUIRE(fn(*num) == 1);
}

TEST_CASE("unique pointer destruction test", "[ptrs]") {
  class Notifier {
  public:
    Notifier(std::string& str): _str(str) { _str = "constructed"; }
    ~Notifier() { _str = "destructed"; }
  private:
    std::string& _str;
  };

  class PtrHolder {
  public:
    PtrHolder(std::string& str1, std::string& str2):
      _uPtr(new Notifier(str1)),
      _rPtr(new Notifier(str2)) {}
    ~PtrHolder() { delete _rPtr; }
  private:
    const std::unique_ptr<Notifier> _uPtr;
    const Notifier * _rPtr;
  };

  std::string a = "";
  std::string b = "";
  PtrHolder * ph = new PtrHolder(a, b);
  REQUIRE(a == "constructed");
  REQUIRE(b == "constructed");
  delete ph;
  REQUIRE(a == "destructed");
  REQUIRE(b == "destructed");


}
