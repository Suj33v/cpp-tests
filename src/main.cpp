#define CATCH_CONFIG_MAIN // this define tells catch that the current file is the main test file.
#define _BSD_SOURCE       // this is required by cygwin to call gettimeofday() which is used by catch.
#include "catch.hpp"
#include <functional>
#include <memory>
#include <string>

TEST_CASE("bind test", "[misc]")
{
  class Number
  {
  public:
    Number(int num) : _num(num) {}
    int getNum() { return _num; }

  private:
    const int _num;
  };

  Number a(1);
  Number b(2);

  auto genericGetNum = std::bind(&Number::getNum, std::placeholders::_1);
  REQUIRE(genericGetNum(a) == 1);
  REQUIRE(genericGetNum(b) == 2);
}

TEST_CASE("bool compare test", "[misc]")
{
  REQUIRE((true > false));
}

TEST_CASE("bool to int implicit conversion test", "[misc]")
{
  const bool v1 = true, v2 = false, v3 = true;

  REQUIRE(v1 + v2 == 1);
  REQUIRE(2 * v1 == 2);
  REQUIRE(v1 + v3 == 2);
}

TEST_CASE("enum ordering test", "[misc]")
{
  SECTION("default-valued enum")
  {
    enum class Values
    {
      FIRST,
      SECOND
    };
    REQUIRE(Values::FIRST < Values::SECOND);
    REQUIRE(Values::SECOND > Values::FIRST);
  }

  SECTION("custom-valued enums")
  {
    enum class Values
    {
      FIRST = 1,
      SECOND = -1
    };
    REQUIRE(Values::FIRST > Values::SECOND);
    REQUIRE(Values::SECOND < Values::FIRST);
  }
}

TEST_CASE("inheritance test", "[misc]")
{
  class Base
  {
  public:
    virtual std::string f1() const { return "base"; }
    std::string f2() const { return "base"; }
  };

  class Derived : public Base
  {
  public:
    std::string f1() const { return "derived"; }
    std::string f2() const { return "derived"; }
  };

  class User
  {
  public:
    std::string g(const Base &a) const { return a.f2(); }
    std::string g(const Derived &a) const { return a.f2(); }
  };

  SECTION("testing virtual function binding")
  {
    std::unique_ptr<Base> b(new Derived);
    REQUIRE(b->f1() == "derived");
  }

  SECTION("testing choice of function for base pointer pointing to derived instance")
  {
    std::unique_ptr<Base> b(new Derived);
    User u;
    REQUIRE(u.g(*b) == "base");
  }
}

TEST_CASE("meyer's singleton test", "[misc]")
{
  class MeyerSingleton
  {
  public:
    static const MeyerSingleton &instance()
    {
      static MeyerSingleton s;
      return s;
    }

    int getCount() const { return _count; }
    void incrementCount() const { ++_count; }

  private:
    MeyerSingleton() : _count(0) {}
    ~MeyerSingleton() {}

    mutable int _count;
  };

  REQUIRE(MeyerSingleton::instance().getCount() == 0);

  MeyerSingleton::instance().incrementCount();

  REQUIRE(MeyerSingleton::instance().getCount() == 1);
}

TEST_CASE("test to use ternary operator as a statement", "[misc]")
{
  int a = 0;
  true ? a = 1 : a = 2;
  REQUIRE(a == 1);
  false ? a = 1 : a = 2;
  REQUIRE(a == 2);
}
