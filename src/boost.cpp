#include "catch.hpp"
#include <boost/icl/interval.hpp>
#include <boost/icl/interval_set.hpp>
#include <boost/icl/map.hpp> // required only for boost::icl::partial_enricher.
#include <boost/icl/separate_interval_set.hpp>
#include <boost/icl/split_interval_set.hpp>
#include <boost/icl/interval_map.hpp>
#include <boost/optional.hpp>
#include <iostream>

TEST_CASE("interval tests", "[boost]")
{
    typedef boost::icl::interval<int> Interval;

    SECTION("closed intervals")
    {
        auto ival = Interval::closed(1, 3);
        REQUIRE(ival.lower() == 1);
        REQUIRE(ival.upper() == 3);
        REQUIRE(contains(ival, 1) == true);
        REQUIRE(contains(ival, 3) == true);
    }

    SECTION("open intervals")
    {
        auto ival = Interval::open(1, 3);
        REQUIRE(ival.lower() == 1);
        REQUIRE(ival.upper() == 3);
        REQUIRE(contains(ival, 1) == false);
        REQUIRE(contains(ival, 3) == false);
    }

    SECTION("right open intervals")
    {
        auto ival = Interval::right_open(1, 3);
        REQUIRE(ival.lower() == 1);
        REQUIRE(ival.upper() == 3);
        REQUIRE(contains(ival, 1) == true);
        REQUIRE(contains(ival, 3) == false);
    }

    SECTION("left open intervals")
    {
        auto ival = Interval::left_open(1, 3);
        REQUIRE(ival.lower() == 1);
        REQUIRE(ival.upper() == 3);
        REQUIRE(contains(ival, 1) == false);
        REQUIRE(contains(ival, 3) == true);
    }
}

TEST_CASE("interval set tests", "[boost]")
{
    typedef boost::icl::interval<int> Interval;

    SECTION("interval sets")
    {
        // intervals joined on overlap or touch.
        typedef boost::icl::interval_set<int> IntervalSet;
        auto iset = IntervalSet{};

        iset += Interval::closed(1, 3);
        REQUIRE((iset == IntervalSet(Interval::closed(1, 3))));

        iset += Interval::open(2, 5);
        REQUIRE((iset == IntervalSet(Interval::right_open(1, 5))));

        iset += Interval::closed(5, 7);
        REQUIRE((iset == IntervalSet(Interval::closed(1, 7))));

        iset += Interval::closed(10, 12);
        REQUIRE(contains(iset, Interval::closed(2, 6)));
        REQUIRE(contains(iset, Interval::closed(10, 12)));
        REQUIRE_FALSE(contains(iset, Interval::closed(8, 9)));
        REQUIRE_FALSE(contains(iset, Interval::closed(8, 11)));
    }

    SECTION("separate interval sets")
    {
        // intervals joined on overlap, not touch.
        typedef boost::icl::separate_interval_set<int> SeparateIntervalSet;
        auto se_iset = SeparateIntervalSet{};

        se_iset += Interval::right_open(1, 3);
        se_iset += Interval::closed(3, 5);
        REQUIRE_FALSE((se_iset == SeparateIntervalSet(Interval::closed(1, 5)))); // as se_iset is {[1, 3), [3, 5]}.

        se_iset += Interval::closed(2, 4);
        REQUIRE((se_iset == SeparateIntervalSet(Interval::closed(1, 5))));
    }

    SECTION("split interval sets")
    {
        // intervals split on overlap, borders preserved.
        typedef boost::icl::split_interval_set<int> SplitIntervalSet;
        auto sp_iset = SplitIntervalSet{};

        sp_iset += Interval::right_open(1, 3);
        sp_iset += Interval::right_open(2, 5);

        auto res = SplitIntervalSet{};
        res += Interval::right_open(1, 2);
        res += Interval::right_open(2, 3);
        res += Interval::right_open(3, 5);
        REQUIRE((sp_iset == res));
    }
}

TEST_CASE("interval map tests", "[boost]")
{
    // interval maps have the "identity absorber" property. This prevents storing
    // identities as values. For example, the following values cannot be stored:
    // - int values cannot store 0.
    // - std::string values cannot store "".
    // - set<T> values cannot store {}.
    typedef boost::icl::interval<int> Interval;
    typedef boost::icl::discrete_interval<int> DiscreteInterval;

    SECTION("interval maps")
    {
        typedef boost::icl::interval_map<int, int> IntervalMap;
        auto imap = IntervalMap{};

        imap += std::make_pair(Interval::closed(6, 8), 1);
        imap += std::make_pair(Interval::closed(7, 9), 2);

        std::vector<int> key_lowers, key_uppers, vals;
        std::vector<DiscreteInterval> keys;
        for (auto it = imap.begin(); it != imap.end(); ++it)
        {
            key_lowers.push_back(it->first.lower());
            key_uppers.push_back(it->first.upper());
            vals.push_back(it->second);
            keys.push_back(it->first);
        }

        REQUIRE((key_lowers == std::vector<int>({6, 7, 8})));
        REQUIRE((key_uppers == std::vector<int>({7, 8, 9})));
        REQUIRE((vals == std::vector<int>({1, 3, 2})));
        REQUIRE((keys == std::vector<DiscreteInterval>(
                             {DiscreteInterval::right_open(6, 7), DiscreteInterval::closed(7, 8), DiscreteInterval::left_open(8, 9)})));
    }

    SECTION("enriched interval maps")
    {
        typedef boost::icl::interval_map<int, int, boost::icl::partial_enricher> EIMap;

        // The third template above is a trait of the interval map. There are 4 possible traits:
        // - partial_absorber
        // - partial_enricher
        // - total absorber
        // - total enricher
        // "partial" means that the map is defined only for keys that have been inserted.
        // "total" means that the map is considered to have a "neutral" value for all keys not
        // stored in the map.
        // "absorber" means that the identity element with respect to "+" cannot be stored as a
        // value. For example, 0 for int types and "" for std::string types.
        // "enricher" allows storing the identity element.

        auto eimap = EIMap{};

        eimap += std::make_pair(Interval::closed(1, 10), 0);
        eimap += std::make_pair(Interval::closed(3, 5), 1);
        eimap += std::make_pair(Interval::closed(8, 10), 3);

        std::vector<DiscreteInterval> keys;
        std::vector<int> vals;
        for (auto it = eimap.begin(); it != eimap.end(); ++it)
        {
            keys.push_back(it->first);
            vals.push_back(it->second);
        }

        REQUIRE((keys == std::vector<DiscreteInterval>(
                             {DiscreteInterval::right_open(1, 3), DiscreteInterval::closed(3, 5), DiscreteInterval::open(5, 8), DiscreteInterval::closed(8, 10)})));
        REQUIRE((vals == std::vector<int>({0, 1, 0, 3})));
    }
}

TEST_CASE("optional test", "[boost]")
{
    SECTION("optional construction")
    {
        auto optInt = boost::optional<int>{};
        REQUIRE_FALSE(optInt.is_initialized());
        optInt = 1;
        REQUIRE(optInt.is_initialized());
        REQUIRE(optInt.get() == 1);
    }

    SECTION("optional returning function")
    {
        auto positivePart = [](int num) -> boost::optional<int> {
            return num > 0 ? num : boost::optional<int>{};
        };

        auto optVal = positivePart(5);
        REQUIRE(optVal.is_initialized());
        REQUIRE(optVal.get() == 5);
        REQUIRE_FALSE(positivePart(-5).is_initialized());
    }
}
