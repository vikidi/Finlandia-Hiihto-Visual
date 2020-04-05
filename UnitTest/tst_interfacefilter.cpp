#include <QtTest>
#include <QObject>

#include "interfacefilter.h"

// Typedef so that Qt macros understand the map
typedef std::map<InternetExplorers::Constants::Filter::ValueFilters, QString> filtermap;
Q_DECLARE_METATYPE(filtermap)

// Just to make code more readable
using InternetExplorers::Constants::Filter::ValueFilters;

/*!
 * \brief
 * This is for all of the tests that test our own code.
 */
namespace InternetExplorersTests {

/*!
 * \brief
 * This class is for InterfaceFilter classes tests
 */
class InterfaceFilter : public QObject
{
    Q_OBJECT

public:
    /*!
     * \brief
     * Initializes tests
     */
    InterfaceFilter();
    ~InterfaceFilter();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    /* OVERALL FILTER */
    void testOverallFilter();
    /* !OVERALL FILTER */

    /* YEAR FILTER */
    void testYearFilter_data();
    void testYearFilter();
    void bencmarkYearFilter();

    void testYearFilterFail_data();
    void testYearFilterFail();
    void bencmarkYearFilterFail();
    /* !YEAR FILTER */

    /* YEAR RANGE FILTER */
    void testYearRangeFilter_data();
    void testYearRangeFilter();
    void bencmarkYearRangeFilter();

    void testYearRangeFilterFail_data();
    void testYearRangeFilterFail();
    void bencmarkYearRangeFilterFail();
    /* !YEAR RANGE FILTER */

    /* DISTANCE FILTER */
    void testDistanceFilter_data();
    void testDistanceFilter();
    void bencmarkDistanceFilter();

    void testDistanceFilterFail_data();
    void testDistanceFilterFail();
    void bencmarkDistanceFilterFail();
    /* !DISTANCE FILTER */

    /* NAME FILTER */
    void testNameFilter_data();
    void testNameFilter();
    void bencmarkNameFilter();

    void testNameFilterFail_data();
    void testNameFilterFail();
    void bencmarkNameFilterFail();
    /* !NAME FILTER */

    /* TIME RANGE FILTER */
    void testTimeRangeFilter_data();
    void testTimeRangeFilter();
    void bencmarkTimeRangeFilter();

    void testTimeRangeFilterFail_data();
    void testTimeRangeFilterFail();
    void bencmarkTimeRangeFilterFail();
    /* !TIME RANGE FILTER */

    /* PLACE FILTER */
    void testPlaceFilter_data();
    void testPlaceFilter();
    void bencmarkPlaceFilter();

    void testPlaceFilterFail_data();
    void testPlaceFilterFail();
    void bencmarkPlaceFilterFail();
    /* !PLACE FILTER */

    /* PLACE RANGE FILTER */
    void testPlaceRangeFilter_data();
    void testPlaceRangeFilter();
    void bencmarkPlaceRangeFilter();

    void testPlaceRangeFilterFail_data();
    void testPlaceRangeFilterFail();
    void bencmarkPlaceRangeFilterFail();
    /* !PLACE RANGE FILTER */

    /* PLACE MEN FILTER */
    void testPlaceMenFilter_data();
    void testPlaceMenFilter();
    void bencmarkPlaceMenFilter();

    void testPlaceMenFilterFail_data();
    void testPlaceMenFilterFail();
    void bencmarkPlaceMenFilterFail();
    /* !PLACE MEN FILTER */

    /* PLACE WOMEN FILTER */
    void testPlaceWomenFilter_data();
    void testPlaceWomenFilter();
    void bencmarkPlaceWomenFilter();

    void testPlaceWomenFilterFail_data();
    void testPlaceWomenFilterFail();
    void bencmarkPlaceWomenFilterFail();
    /* !PLACE WOMEN FILTER */

    /* SEX FILTER */
    void testSexFilter_data();
    void testSexFilter();
    void bencmarkSexFilter();

    void testSexFilterFail_data();
    void testSexFilterFail();
    void bencmarkSexFilterFail();
    /* !SEX FILTER */

    /* CITY FILTER */
    void testCityFilter_data();
    void testCityFilter();
    void bencmarkCityFilter();

    void testCityFilterFail_data();
    void testCityFilterFail();
    void bencmarkCityFilterFail();
    /* !CITY FILTER */

    /* NATIONALITY FILTER */
    void testNationalityFilter_data();
    void testNationalityFilter();
    void bencmarkNationalityFilter();

    void testNationalityFilterFail_data();
    void testNationalityFilterFail();
    void bencmarkNationalityFilterFail();
    /* !NATIONALITY FILTER */

    /* BIRTH YEAR FILTER */
    void testBirthYearFilter_data();
    void testBirthYearFilter();
    void bencmarkBirthYearFilter();

    void testBirthYearFilterFail_data();
    void testBirthYearFilterFail();
    void bencmarkBirthYearFilterFail();
    /* !BIRTH YEAR FILTER */

    /* TEAM FILTER */
    void testTeamFilter_data();
    void testTeamFilter();
    void bencmarkTeamFilter();

    void testTeamFilterFail_data();
    void testTeamFilterFail();
    void bencmarkTeamFilterFail();
    /* !TEAM FILTER */

private:
    InternetExplorers::InterfaceFilter* m_filter;
};

}

InternetExplorersTests::InterfaceFilter::InterfaceFilter():
    m_filter(nullptr)
{

}

InternetExplorersTests::InterfaceFilter::~InterfaceFilter()
{

}

void InternetExplorersTests::InterfaceFilter::initTestCase()
{
    m_filter = new InternetExplorers::InterfaceFilter();
    QVERIFY(m_filter != nullptr);
}

void InternetExplorersTests::InterfaceFilter::cleanupTestCase()
{
    delete m_filter;
}

void InternetExplorersTests::InterfaceFilter::testOverallFilter()
{
    filtermap filter = filtermap({{ValueFilters::YEAR, "1996"},
                                  {ValueFilters::YEAR_RANGE, "1996;2000"}});
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);

    filter = filtermap({{ValueFilters::PLACE, "5"},
                        {ValueFilters::PLACE_RANGE, "5;8"}});
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);

    filter = filtermap({});
    try {
        QCOMPARE(m_filter->validateFilter(filter), true);
    } catch (const InternetExplorers::FilterException&) {
        QFAIL("Threw filter exception when shouldnt have");
    }

}

void InternetExplorersTests::InterfaceFilter::testYearFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Lower bound") << filtermap(
            {{ValueFilters::YEAR, "1974"}});
    QTest::newRow("Upper bound") << filtermap(
            {{ValueFilters::YEAR, "2019"}});
    QTest::newRow("Normal year") << filtermap(
            {{ValueFilters::YEAR, "1996"}});
}

void InternetExplorersTests::InterfaceFilter::testYearFilter()
{
    QFETCH(filtermap, filter);
    try {
        QCOMPARE(m_filter->validateFilter(filter), true);
    } catch (const InternetExplorers::FilterException&) {
        QFAIL("Threw filter exception when shouldnt have");
    }
}

void InternetExplorersTests::InterfaceFilter::bencmarkYearFilter()
{
    filtermap fil = filtermap({{ValueFilters::YEAR, "1996"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InternetExplorersTests::InterfaceFilter::testYearFilterFail_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{ValueFilters::YEAR, ""}});
    QTest::newRow("Only space") << filtermap(
            {{ValueFilters::YEAR, "    "}});
    QTest::newRow("Kaikki vuodet") << filtermap(
            {{ValueFilters::YEAR, "Kaikki vuodet"}});
    QTest::newRow("Random string") << filtermap(
            {{ValueFilters::YEAR, "Justtesting"}});
    QTest::newRow("Too small year") << filtermap(
            {{ValueFilters::YEAR, "1917"}});
    QTest::newRow("Too big year") << filtermap(
            {{ValueFilters::YEAR, "2420"}});
}

void InternetExplorersTests::InterfaceFilter::testYearFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InternetExplorersTests::InterfaceFilter::bencmarkYearFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::YEAR, "2420"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (const InternetExplorers::FilterException&) {

        }
    }
}

void InternetExplorersTests::InterfaceFilter::testYearRangeFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Normal range") << filtermap(
            {{ValueFilters::YEAR_RANGE, "1987;2002"}});
    QTest::newRow("Lower year on lower bound") << filtermap(
            {{ValueFilters::YEAR_RANGE, "1974;1996"}});
    QTest::newRow("Upper year on upper bound") << filtermap(
            {{ValueFilters::YEAR_RANGE, "1996;2019"}});
    QTest::newRow("Both values on bounds") << filtermap(
            {{ValueFilters::YEAR_RANGE, "1974;2019"}});
    QTest::newRow("One year difference") << filtermap(
            {{ValueFilters::YEAR_RANGE, "1980;1981"}});
}

void InternetExplorersTests::InterfaceFilter::testYearRangeFilter()
{
    QFETCH(filtermap, filter);
    try {
        QCOMPARE(m_filter->validateFilter(filter), true);
    } catch (const InternetExplorers::FilterException&) {
        QFAIL("Threw filter exception when shouldnt have");
    }
}

void InternetExplorersTests::InterfaceFilter::bencmarkYearRangeFilter()
{
    filtermap fil = filtermap({{ValueFilters::YEAR_RANGE, "1996;2002"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InternetExplorersTests::InterfaceFilter::testYearRangeFilterFail_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{ValueFilters::YEAR_RANGE, ""}});
    QTest::newRow("White space") << filtermap(
            {{ValueFilters::YEAR_RANGE, "    "}});
    QTest::newRow("Only separator") << filtermap(
            {{ValueFilters::YEAR_RANGE, ";"}});
    QTest::newRow("Only one year, no separator") << filtermap(
            {{ValueFilters::YEAR_RANGE, "1996"}});
    QTest::newRow("Only lower year, with separator") << filtermap(
            {{ValueFilters::YEAR_RANGE, "1996;"}});
    QTest::newRow("Only upper year, with separator") << filtermap(
            {{ValueFilters::YEAR_RANGE, ";1996"}});
    QTest::newRow("Wrong separator") << filtermap(
            {{ValueFilters::YEAR_RANGE, "1980:1990"}});
    QTest::newRow("Lower bound too low") << filtermap(
            {{ValueFilters::YEAR_RANGE, "1970;1990"}});
    QTest::newRow("Upper bound too high") << filtermap(
            {{ValueFilters::YEAR_RANGE, "1990;2420"}});
    QTest::newRow("Same lower and upper years") << filtermap(
            {{ValueFilters::YEAR_RANGE, "1990;1990"}});
    QTest::newRow("Lower year higher than upper") << filtermap(
            {{ValueFilters::YEAR_RANGE, "1990;1980"}});
}

void InternetExplorersTests::InterfaceFilter::testYearRangeFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InternetExplorersTests::InterfaceFilter::bencmarkYearRangeFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::YEAR_RANGE, "1990;1980"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (const InternetExplorers::FilterException&) {

        }
    }
}

void InternetExplorersTests::InterfaceFilter::testDistanceFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Normal distance, P") << filtermap(
            {{ValueFilters::DISTANCE, "P50"}});
    QTest::newRow("Normal distance, V") << filtermap(
            {{ValueFilters::DISTANCE, "V50"}});
}

void InternetExplorersTests::InterfaceFilter::testDistanceFilter()
{
    QFETCH(filtermap, filter);
    try {
        QCOMPARE(m_filter->validateFilter(filter), true);
    } catch (const InternetExplorers::FilterException&) {
        QFAIL("Threw filter exception when shouldnt have");
    }
}

void InternetExplorersTests::InterfaceFilter::bencmarkDistanceFilter()
{
    filtermap fil = filtermap({{ValueFilters::DISTANCE, "P50"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InternetExplorersTests::InterfaceFilter::testDistanceFilterFail_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{ValueFilters::DISTANCE, ""}});
    QTest::newRow("White space") << filtermap(
            {{ValueFilters::DISTANCE, "    "}});
    QTest::newRow("Incorrect distance") << filtermap(
            {{ValueFilters::DISTANCE, "V200"}});
}

void InternetExplorersTests::InterfaceFilter::testDistanceFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InternetExplorersTests::InterfaceFilter::bencmarkDistanceFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::DISTANCE, "V200"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (const InternetExplorers::FilterException&) {

        }
    }
}

void InternetExplorersTests::InterfaceFilter::testNameFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Normal name") << filtermap(
            {{ValueFilters::NAME, "Mursu Esa"}});
    QTest::newRow("Upper case name") << filtermap(
            {{ValueFilters::NAME, "MURSU ESA"}});
    QTest::newRow("Lower case name") << filtermap(
            {{ValueFilters::NAME, "mursu esa"}});
    QTest::newRow("Only one name") << filtermap(
            {{ValueFilters::NAME, "Mursu"}});
}

void InternetExplorersTests::InterfaceFilter::testNameFilter()
{
    QFETCH(filtermap, filter);
    try {
        QCOMPARE(m_filter->validateFilter(filter), true);
    } catch (const InternetExplorers::FilterException&) {
        QFAIL("Threw filter exception when shouldnt have");
    }
}

void InternetExplorersTests::InterfaceFilter::bencmarkNameFilter()
{
    filtermap fil = filtermap({{ValueFilters::NAME, "Mursu Esa"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InternetExplorersTests::InterfaceFilter::testNameFilterFail_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{ValueFilters::NAME, ""}});
    QTest::newRow("White space") << filtermap(
            {{ValueFilters::NAME, "    "}});
    QTest::newRow("Special characters") << filtermap(
            {{ValueFilters::NAME, "Mursu.; Esa"}});
}

void InternetExplorersTests::InterfaceFilter::testNameFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InternetExplorersTests::InterfaceFilter::bencmarkNameFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::NAME, "Mursu.; Esa"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (const InternetExplorers::FilterException&) {

        }
    }
}

void InternetExplorersTests::InterfaceFilter::testTimeRangeFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Normal time range") << filtermap(
            {{ValueFilters::TIME_RANGE, "5:10:20;6:30:40"}});
    QTest::newRow("Over 10h") << filtermap(
            {{ValueFilters::TIME_RANGE, "5:10:20;16:30:40"}});
    QTest::newRow("One second apart") << filtermap(
            {{ValueFilters::TIME_RANGE, "5:10:20;5:10:21"}});
    QTest::newRow("Same lower and upper times") << filtermap(
            {{ValueFilters::TIME_RANGE, "5:10:20;5:10:20"}});
    QTest::newRow("Only seconds") << filtermap(
            {{ValueFilters::TIME_RANGE, "0:00:20;0:00:40"}});
    QTest::newRow("Only minutes") << filtermap(
            {{ValueFilters::TIME_RANGE, "0:10:00;0:15:00"}});
    QTest::newRow("Only hours") << filtermap(
            {{ValueFilters::TIME_RANGE, "5:00:00;7:00:00"}});
    QTest::newRow("Minimum boundaries, all") << filtermap(
            {{ValueFilters::TIME_RANGE, "0:00:00;7:00:00"}});
    QTest::newRow("Maximum boundaries, min/sec") << filtermap(
            {{ValueFilters::TIME_RANGE, "1:50:30;7:59:59"}});
}

void InternetExplorersTests::InterfaceFilter::testTimeRangeFilter()
{
    QFETCH(filtermap, filter);
    try {
        QCOMPARE(m_filter->validateFilter(filter), true);
    } catch (const InternetExplorers::FilterException&) {
        QFAIL("Threw filter exception when shouldnt have");
    }
}

void InternetExplorersTests::InterfaceFilter::bencmarkTimeRangeFilter()
{
    filtermap fil = filtermap({{ValueFilters::TIME_RANGE, "1:50:30;7:59:59"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InternetExplorersTests::InterfaceFilter::testTimeRangeFilterFail_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{ValueFilters::TIME_RANGE, ""}});
    QTest::newRow("White space") << filtermap(
            {{ValueFilters::TIME_RANGE, "    "}});
    QTest::newRow("Only separator") << filtermap(
            {{ValueFilters::TIME_RANGE, ";"}});
    QTest::newRow("Only one time, no separator") << filtermap(
            {{ValueFilters::TIME_RANGE, "5:10:20"}});
    QTest::newRow("Only lower time, with separator") << filtermap(
            {{ValueFilters::TIME_RANGE, "5:10:20;"}});
    QTest::newRow("Only upper time, with separator") << filtermap(
            {{ValueFilters::TIME_RANGE, ";5:10:20"}});
    QTest::newRow("Wrong separator between times") << filtermap(
            {{ValueFilters::TIME_RANGE, "5:10:20:6:10:20"}});
    QTest::newRow("Wrong separator inside times") << filtermap(
            {{ValueFilters::TIME_RANGE, "5.10.20;6.10.20"}});
    QTest::newRow("Lower bound too low") << filtermap(
            {{ValueFilters::TIME_RANGE, "-4:10:20;5:10:20"}});
    QTest::newRow("Seconds too high") << filtermap(
            {{ValueFilters::TIME_RANGE, "5:10:99;6:10:20"}});
    QTest::newRow("Minutes too high") << filtermap(
            {{ValueFilters::TIME_RANGE, "5:99:20;6:10:20"}});
    QTest::newRow("Lower time higher than upper") << filtermap(
            {{ValueFilters::TIME_RANGE, "5:10:20;4:10:20"}});
    QTest::newRow("Tenths of second") << filtermap(
            {{ValueFilters::TIME_RANGE, "5:10:20.4;6:10:20.5"}});
}

void InternetExplorersTests::InterfaceFilter::testTimeRangeFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InternetExplorersTests::InterfaceFilter::bencmarkTimeRangeFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::TIME_RANGE, "5:10:20;4:10:20"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (const InternetExplorers::FilterException&) {

        }
    }
}

void InternetExplorersTests::InterfaceFilter::testPlaceFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Normal place") << filtermap(
            {{ValueFilters::PLACE, "5"}});
    QTest::newRow("First place") << filtermap(
            {{ValueFilters::PLACE, "1"}});
    QTest::newRow("Very high place") << filtermap(
            {{ValueFilters::PLACE, "15000"}});
}

void InternetExplorersTests::InterfaceFilter::testPlaceFilter()
{
    QFETCH(filtermap, filter);
    try {
        QCOMPARE(m_filter->validateFilter(filter), true);
    } catch (const InternetExplorers::FilterException&) {
        QFAIL("Threw filter exception when shouldnt have");
    }
}

void InternetExplorersTests::InterfaceFilter::bencmarkPlaceFilter()
{
    filtermap fil = filtermap({{ValueFilters::PLACE, "5"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InternetExplorersTests::InterfaceFilter::testPlaceFilterFail_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{ValueFilters::PLACE, ""}});
    QTest::newRow("White space") << filtermap(
            {{ValueFilters::PLACE, "    "}});
    QTest::newRow("Zero plaxe") << filtermap(
            {{ValueFilters::PLACE, "0"}});
    QTest::newRow("Negative place") << filtermap(
            {{ValueFilters::PLACE, "-5"}});
    QTest::newRow("Text without numbers") << filtermap(
            {{ValueFilters::PLACE, "Justtesting"}});
}

void InternetExplorersTests::InterfaceFilter::testPlaceFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InternetExplorersTests::InterfaceFilter::bencmarkPlaceFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::PLACE, "-5"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (const InternetExplorers::FilterException&) {

        }
    }
}

void InternetExplorersTests::InterfaceFilter::testPlaceMenFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Normal place") << filtermap(
            {{ValueFilters::PLACE_MEN, "5"}});
    QTest::newRow("First place") << filtermap(
            {{ValueFilters::PLACE_MEN, "1"}});
    QTest::newRow("Very high place") << filtermap(
            {{ValueFilters::PLACE_MEN, "15000"}});
}

void InternetExplorersTests::InterfaceFilter::testPlaceMenFilter()
{
    QFETCH(filtermap, filter);
    try {
        QCOMPARE(m_filter->validateFilter(filter), true);
    } catch (const InternetExplorers::FilterException&) {
        QFAIL("Threw filter exception when shouldnt have");
    }
}

void InternetExplorersTests::InterfaceFilter::bencmarkPlaceMenFilter()
{
    filtermap fil = filtermap({{ValueFilters::PLACE_MEN, "5"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InternetExplorersTests::InterfaceFilter::testPlaceMenFilterFail_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{ValueFilters::PLACE_MEN, ""}});
    QTest::newRow("White space") << filtermap(
            {{ValueFilters::PLACE_MEN, "    "}});
    QTest::newRow("Zero plaxe") << filtermap(
            {{ValueFilters::PLACE_MEN, "0"}});
    QTest::newRow("Negative place") << filtermap(
            {{ValueFilters::PLACE_MEN, "-5"}});
    QTest::newRow("Text without numbers") << filtermap(
            {{ValueFilters::PLACE_MEN, "Justtesting"}});
}

void InternetExplorersTests::InterfaceFilter::testPlaceMenFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InternetExplorersTests::InterfaceFilter::bencmarkPlaceMenFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::PLACE_MEN, "-5"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (const InternetExplorers::FilterException&) {

        }
    }
}

void InternetExplorersTests::InterfaceFilter::testPlaceWomenFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Normal place") << filtermap(
            {{ValueFilters::PLACE_WOMEN, "5"}});
    QTest::newRow("First place") << filtermap(
            {{ValueFilters::PLACE_WOMEN, "1"}});
    QTest::newRow("Very high place") << filtermap(
            {{ValueFilters::PLACE_WOMEN, "15000"}});
}

void InternetExplorersTests::InterfaceFilter::testPlaceWomenFilter()
{
    QFETCH(filtermap, filter);
    try {
        QCOMPARE(m_filter->validateFilter(filter), true);
    } catch (const InternetExplorers::FilterException&) {
        QFAIL("Threw filter exception when shouldnt have");
    }
}

void InternetExplorersTests::InterfaceFilter::bencmarkPlaceWomenFilter()
{
    filtermap fil = filtermap({{ValueFilters::PLACE_WOMEN, "5"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InternetExplorersTests::InterfaceFilter::testPlaceWomenFilterFail_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{ValueFilters::PLACE_WOMEN, ""}});
    QTest::newRow("White space") << filtermap(
            {{ValueFilters::PLACE_WOMEN, "    "}});
    QTest::newRow("Zero plaxe") << filtermap(
            {{ValueFilters::PLACE_WOMEN, "0"}});
    QTest::newRow("Negative place") << filtermap(
            {{ValueFilters::PLACE_WOMEN, "-5"}});
    QTest::newRow("Text without numbers") << filtermap(
            {{ValueFilters::PLACE_WOMEN, "Justtesting"}});
}

void InternetExplorersTests::InterfaceFilter::testPlaceWomenFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InternetExplorersTests::InterfaceFilter::bencmarkPlaceWomenFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::PLACE_WOMEN, "-5"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (const InternetExplorers::FilterException&) {

        }
    }
}

void InternetExplorersTests::InterfaceFilter::testSexFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Man") << filtermap(
            {{ValueFilters::SEX, "M"}});
    QTest::newRow("Woman") << filtermap(
            {{ValueFilters::SEX, "N"}});
}

void InternetExplorersTests::InterfaceFilter::testSexFilter()
{
    QFETCH(filtermap, filter);
    try {
        QCOMPARE(m_filter->validateFilter(filter), true);
    } catch (const InternetExplorers::FilterException&) {
        QFAIL("Threw filter exception when shouldnt have");
    }
}

void InternetExplorersTests::InterfaceFilter::bencmarkSexFilter()
{
    filtermap fil = filtermap({{ValueFilters::SEX, "N"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InternetExplorersTests::InterfaceFilter::testSexFilterFail_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{ValueFilters::SEX, ""}});
    QTest::newRow("White space") << filtermap(
            {{ValueFilters::SEX, "    "}});
    QTest::newRow("Random text") << filtermap(
            {{ValueFilters::SEX, "Normal test"}});
    QTest::newRow("Number") << filtermap(
            {{ValueFilters::SEX, "5"}});
}

void InternetExplorersTests::InterfaceFilter::testSexFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InternetExplorersTests::InterfaceFilter::bencmarkSexFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::SEX, "5"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (const InternetExplorers::FilterException&) {

        }
    }
}

void InternetExplorersTests::InterfaceFilter::testPlaceRangeFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Normal range") << filtermap(
            {{ValueFilters::PLACE_RANGE, "3;8"}});
    QTest::newRow("Lower place on lower bound") << filtermap(
            {{ValueFilters::PLACE_RANGE, "1;8"}});
    QTest::newRow("One place difference") << filtermap(
            {{ValueFilters::PLACE_RANGE, "3;4"}});
}

void InternetExplorersTests::InterfaceFilter::testPlaceRangeFilter()
{
    QFETCH(filtermap, filter);
    try {
        QCOMPARE(m_filter->validateFilter(filter), true);
    } catch (const InternetExplorers::FilterException&) {
        QFAIL("Threw filter exception when shouldnt have");
    }
}

void InternetExplorersTests::InterfaceFilter::bencmarkPlaceRangeFilter()
{
    filtermap fil = filtermap({{ValueFilters::PLACE_RANGE, "3;8"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InternetExplorersTests::InterfaceFilter::testPlaceRangeFilterFail_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{ValueFilters::PLACE_RANGE, ""}});
    QTest::newRow("White space") << filtermap(
            {{ValueFilters::PLACE_RANGE, "    "}});
    QTest::newRow("Only separator") << filtermap(
            {{ValueFilters::PLACE_RANGE, ";"}});
    QTest::newRow("Only one place, no separator") << filtermap(
            {{ValueFilters::PLACE_RANGE, "3"}});
    QTest::newRow("Only lower place, with separator") << filtermap(
            {{ValueFilters::PLACE_RANGE, "3;"}});
    QTest::newRow("Wrong separator") << filtermap(
            {{ValueFilters::PLACE_RANGE, "3:8"}});
    QTest::newRow("Lower bound too low") << filtermap(
            {{ValueFilters::PLACE_RANGE, "-1;8"}});
    QTest::newRow("Same lower and upper places") << filtermap(
            {{ValueFilters::PLACE_RANGE, "3;3"}});
    QTest::newRow("Lower place higher than upper") << filtermap(
            {{ValueFilters::PLACE_RANGE, "6;3"}});
}

void InternetExplorersTests::InterfaceFilter::testPlaceRangeFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InternetExplorersTests::InterfaceFilter::bencmarkPlaceRangeFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::PLACE_RANGE, "6;3"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (const InternetExplorers::FilterException&) {

        }
    }
}

void InternetExplorersTests::InterfaceFilter::testCityFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Normal city name") << filtermap(
            {{ValueFilters::CITY, "Raisio"}});
    QTest::newRow("All upper case") << filtermap(
            {{ValueFilters::CITY, "RAISIO"}});
    QTest::newRow("All lower case") << filtermap(
            {{ValueFilters::CITY, "raisio"}});
}

void InternetExplorersTests::InterfaceFilter::testCityFilter()
{
    QFETCH(filtermap, filter);
    try {
        QCOMPARE(m_filter->validateFilter(filter), true);
    } catch (const InternetExplorers::FilterException&) {
        QFAIL("Threw filter exception when shouldnt have");
    }
}

void InternetExplorersTests::InterfaceFilter::bencmarkCityFilter()
{
    filtermap fil = filtermap({{ValueFilters::CITY, "Raisio"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InternetExplorersTests::InterfaceFilter::testCityFilterFail_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{ValueFilters::CITY, ""}});
    QTest::newRow("White space") << filtermap(
            {{ValueFilters::CITY, "    "}});
    QTest::newRow("Special characters") << filtermap(
            {{ValueFilters::CITY, "Rai.;sio"}});
}

void InternetExplorersTests::InterfaceFilter::testCityFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InternetExplorersTests::InterfaceFilter::bencmarkCityFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::CITY, "Rai.;sio"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (const InternetExplorers::FilterException&) {

        }
    }
}

void InternetExplorersTests::InterfaceFilter::testNationalityFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Upper case") << filtermap(
            {{ValueFilters::NATIONALITY, "FI"}});
    QTest::newRow("Lower case") << filtermap(
            {{ValueFilters::NATIONALITY, "fi"}});
    QTest::newRow("Mixed case") << filtermap(
            {{ValueFilters::NATIONALITY, "Fi"}});
}

void InternetExplorersTests::InterfaceFilter::testNationalityFilter()
{
    QFETCH(filtermap, filter);
    try {
        QCOMPARE(m_filter->validateFilter(filter), true);
    } catch (const InternetExplorers::FilterException&) {
        QFAIL("Threw filter exception when shouldnt have");
    }
}

void InternetExplorersTests::InterfaceFilter::bencmarkNationalityFilter()
{
    filtermap fil = filtermap({{ValueFilters::NATIONALITY, "FI"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InternetExplorersTests::InterfaceFilter::testNationalityFilterFail_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{ValueFilters::NATIONALITY, ""}});
    QTest::newRow("White space") << filtermap(
            {{ValueFilters::NATIONALITY, "    "}});
    QTest::newRow("Under two characters") << filtermap(
            {{ValueFilters::NATIONALITY, "F"}});
    QTest::newRow("Over two characters") << filtermap(
            {{ValueFilters::NATIONALITY, "FIN"}});
    QTest::newRow("Special characters") << filtermap(
            {{ValueFilters::NATIONALITY, "F;"}});
}

void InternetExplorersTests::InterfaceFilter::testNationalityFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InternetExplorersTests::InterfaceFilter::bencmarkNationalityFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::NATIONALITY, "F;"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (const InternetExplorers::FilterException&) {

        }
    }
}

void InternetExplorersTests::InterfaceFilter::testBirthYearFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Normal birth year") << filtermap(
            {{ValueFilters::BIRTH_YEAR, "89"}});
}

void InternetExplorersTests::InterfaceFilter::testBirthYearFilter()
{
    QFETCH(filtermap, filter);
    try {
        QCOMPARE(m_filter->validateFilter(filter), true);
    } catch (const InternetExplorers::FilterException&) {
        QFAIL("Threw filter exception when shouldnt have");
    }
}

void InternetExplorersTests::InterfaceFilter::bencmarkBirthYearFilter()
{
    filtermap fil = filtermap({{ValueFilters::BIRTH_YEAR, "89"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InternetExplorersTests::InterfaceFilter::testBirthYearFilterFail_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{ValueFilters::BIRTH_YEAR, ""}});
    QTest::newRow("White space") << filtermap(
            {{ValueFilters::BIRTH_YEAR, "    "}});
    QTest::newRow("Under two numbers") << filtermap(
            {{ValueFilters::BIRTH_YEAR, "8"}});
    QTest::newRow("Over two numbers") << filtermap(
            {{ValueFilters::BIRTH_YEAR, "560"}});
    QTest::newRow("Random two characters") << filtermap(
            {{ValueFilters::BIRTH_YEAR, "Mo"}});
}

void InternetExplorersTests::InterfaceFilter::testBirthYearFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InternetExplorersTests::InterfaceFilter::bencmarkBirthYearFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::BIRTH_YEAR, "Mo"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (const InternetExplorers::FilterException&) {

        }
    }
}

void InternetExplorersTests::InterfaceFilter::testTeamFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Normal team") << filtermap(
            {{ValueFilters::TEAM, "Vantaan Hiihtoseura"}});
    QTest::newRow("Upper case team") << filtermap(
            {{ValueFilters::TEAM, "VANTAAN HIIHTOSEURA"}});
    QTest::newRow("Lower case team") << filtermap(
            {{ValueFilters::TEAM, "vantaan hiihtoseura"}});
    QTest::newRow("Only one word in team") << filtermap(
            {{ValueFilters::TEAM, "Vantaan"}});
}

void InternetExplorersTests::InterfaceFilter::testTeamFilter()
{
    QFETCH(filtermap, filter);
    try {
        QCOMPARE(m_filter->validateFilter(filter), true);
    } catch (const InternetExplorers::FilterException&) {
        QFAIL("Threw filter exception when shouldnt have");
    }
}

void InternetExplorersTests::InterfaceFilter::bencmarkTeamFilter()
{
    filtermap fil = filtermap({{ValueFilters::TEAM, "Vantaan Hiihtoseura"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InternetExplorersTests::InterfaceFilter::testTeamFilterFail_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{ValueFilters::TEAM, ""}});
    QTest::newRow("White space") << filtermap(
            {{ValueFilters::TEAM, "    "}});
    QTest::newRow("Special characters") << filtermap(
            {{ValueFilters::TEAM, "Vantaan. Hiihtoseura;"}});
}

void InternetExplorersTests::InterfaceFilter::testTeamFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InternetExplorersTests::InterfaceFilter::bencmarkTeamFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::TEAM, "Vantaan. Hiihtoseura;"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (const InternetExplorers::FilterException&) {

        }
    }
}

QTEST_APPLESS_MAIN(InternetExplorersTests::InterfaceFilter)

#include "tst_interfacefilter.moc"
