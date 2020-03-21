#include <QtTest>
#include <QObject>

#include "interfacefilter.h"

typedef std::map<InternetExplorers::Constants::Filter::ValueFilters, QString> filtermap;
Q_DECLARE_METATYPE(filtermap)

using InternetExplorers::Constants::Filter::ValueFilters;

class InterfaceFilter : public QObject
{
    Q_OBJECT

public:
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

private:
    InternetExplorers::InterfaceFilter* m_filter;
};

InterfaceFilter::InterfaceFilter():
    m_filter(nullptr)
{

}

InterfaceFilter::~InterfaceFilter()
{

}

void InterfaceFilter::initTestCase()
{
    m_filter = new InternetExplorers::InterfaceFilter();
    QVERIFY(m_filter != nullptr);
}

void InterfaceFilter::cleanupTestCase()
{
    delete m_filter;
}

void InterfaceFilter::testOverallFilter()
{
    filtermap filter = filtermap({{ValueFilters::YEAR, "1996"},
                               {ValueFilters::YEAR_RANGE, "1996;2000"}});
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);

    filter = filtermap({});
    QCOMPARE(m_filter->validateFilter(filter), true);
}

void InterfaceFilter::testYearFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Lower bound") << filtermap(
            {{ValueFilters::YEAR, "1974"}});
    QTest::newRow("Upper bound") << filtermap(
            {{ValueFilters::YEAR, "2019"}});
    QTest::newRow("Normal year") << filtermap(
            {{ValueFilters::YEAR, "1996"}});
}

void InterfaceFilter::testYearFilter()
{
    QFETCH(filtermap, filter);
    QCOMPARE(m_filter->validateFilter(filter), true);
}

void InterfaceFilter::bencmarkYearFilter()
{
    filtermap fil = filtermap({{ValueFilters::YEAR, "1996"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InterfaceFilter::testYearFilterFail_data()
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

void InterfaceFilter::testYearFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InterfaceFilter::bencmarkYearFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::YEAR, "2420"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (InternetExplorers::FilterException) {

        }
    }
}

void InterfaceFilter::testYearRangeFilter_data()
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

void InterfaceFilter::testYearRangeFilter()
{
    QFETCH(filtermap, filter);
    QCOMPARE(m_filter->validateFilter(filter), true);
}

void InterfaceFilter::bencmarkYearRangeFilter()
{
    filtermap fil = filtermap({{ValueFilters::YEAR_RANGE, "1996;2002"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InterfaceFilter::testYearRangeFilterFail_data()
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

void InterfaceFilter::testYearRangeFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InterfaceFilter::bencmarkYearRangeFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::YEAR_RANGE, "1990;1980"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (InternetExplorers::FilterException) {

        }
    }
}

void InterfaceFilter::testDistanceFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Normal distance, P") << filtermap(
            {{ValueFilters::DISTANCE, "P50"}});
    QTest::newRow("Normal distance, V") << filtermap(
            {{ValueFilters::DISTANCE, "V50"}});
}

void InterfaceFilter::testDistanceFilter()
{
    QFETCH(filtermap, filter);
    QCOMPARE(m_filter->validateFilter(filter), true);
}

void InterfaceFilter::bencmarkDistanceFilter()
{
    filtermap fil = filtermap({{ValueFilters::DISTANCE, "P50"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InterfaceFilter::testDistanceFilterFail_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{ValueFilters::DISTANCE, ""}});
    QTest::newRow("White space") << filtermap(
            {{ValueFilters::DISTANCE, "    "}});
    QTest::newRow("Incorrect distance") << filtermap(
            {{ValueFilters::DISTANCE, "V200"}});
}

void InterfaceFilter::testDistanceFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InterfaceFilter::bencmarkDistanceFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::DISTANCE, "V200"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (InternetExplorers::FilterException) {

        }
    }
}

void InterfaceFilter::testNameFilter_data()
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

void InterfaceFilter::testNameFilter()
{
    QFETCH(filtermap, filter);
    QCOMPARE(m_filter->validateFilter(filter), true);
}

void InterfaceFilter::bencmarkNameFilter()
{
    filtermap fil = filtermap({{ValueFilters::NAME, "Mursu Esa"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InterfaceFilter::testNameFilterFail_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{ValueFilters::NAME, ""}});
    QTest::newRow("White space") << filtermap(
            {{ValueFilters::NAME, "    "}});
    QTest::newRow("Special characters") << filtermap(
            {{ValueFilters::NAME, "Mursu.; Esa"}});
}

void InterfaceFilter::testNameFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InterfaceFilter::bencmarkNameFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::NAME, "Mursu.; Esa"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (InternetExplorers::FilterException) {

        }
    }
}

void InterfaceFilter::testTimeRangeFilter_data()
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

void InterfaceFilter::testTimeRangeFilter()
{
    QFETCH(filtermap, filter);
    QCOMPARE(m_filter->validateFilter(filter), true);
}

void InterfaceFilter::bencmarkTimeRangeFilter()
{
    filtermap fil = filtermap({{ValueFilters::TIME_RANGE, "1:50:30;7:59:59"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InterfaceFilter::testTimeRangeFilterFail_data()
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

void InterfaceFilter::testTimeRangeFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InterfaceFilter::bencmarkTimeRangeFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::TIME_RANGE, "5:10:20;4:10:20"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (InternetExplorers::FilterException) {

        }
    }
}

void InterfaceFilter::testPlaceFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Normal place") << filtermap(
            {{ValueFilters::PLACE, "5"}});
    QTest::newRow("First place") << filtermap(
            {{ValueFilters::PLACE, "1"}});
    QTest::newRow("Very high place") << filtermap(
            {{ValueFilters::PLACE, "15000"}});
}

void InterfaceFilter::testPlaceFilter()
{
    QFETCH(filtermap, filter);
    QCOMPARE(m_filter->validateFilter(filter), true);
}

void InterfaceFilter::bencmarkPlaceFilter()
{
    filtermap fil = filtermap({{ValueFilters::PLACE, "5"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InterfaceFilter::testPlaceFilterFail_data()
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

void InterfaceFilter::testPlaceFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InterfaceFilter::bencmarkPlaceFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::PLACE, "-5"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (InternetExplorers::FilterException) {

        }
    }
}

void InterfaceFilter::testPlaceMenFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Normal place") << filtermap(
            {{ValueFilters::PLACE_MEN, "5"}});
    QTest::newRow("First place") << filtermap(
            {{ValueFilters::PLACE_MEN, "1"}});
    QTest::newRow("Very high place") << filtermap(
            {{ValueFilters::PLACE_MEN, "15000"}});
}

void InterfaceFilter::testPlaceMenFilter()
{
    QFETCH(filtermap, filter);
    QCOMPARE(m_filter->validateFilter(filter), true);
}

void InterfaceFilter::bencmarkPlaceMenFilter()
{
    filtermap fil = filtermap({{ValueFilters::PLACE_MEN, "5"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InterfaceFilter::testPlaceMenFilterFail_data()
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

void InterfaceFilter::testPlaceMenFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InterfaceFilter::bencmarkPlaceMenFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::PLACE_MEN, "-5"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (InternetExplorers::FilterException) {

        }
    }
}

void InterfaceFilter::testPlaceWomenFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Normal place") << filtermap(
            {{ValueFilters::PLACE_WOMEN, "5"}});
    QTest::newRow("First place") << filtermap(
            {{ValueFilters::PLACE_WOMEN, "1"}});
    QTest::newRow("Very high place") << filtermap(
            {{ValueFilters::PLACE_WOMEN, "15000"}});
}

void InterfaceFilter::testPlaceWomenFilter()
{
    QFETCH(filtermap, filter);
    QCOMPARE(m_filter->validateFilter(filter), true);
}

void InterfaceFilter::bencmarkPlaceWomenFilter()
{
    filtermap fil = filtermap({{ValueFilters::PLACE_WOMEN, "5"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InterfaceFilter::testPlaceWomenFilterFail_data()
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

void InterfaceFilter::testPlaceWomenFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InterfaceFilter::bencmarkPlaceWomenFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::PLACE_WOMEN, "-5"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (InternetExplorers::FilterException) {

        }
    }
}

void InterfaceFilter::testSexFilter_data()
{
    QTest::addColumn<std::map<ValueFilters, QString>>("filter");

    QTest::newRow("Man") << filtermap(
            {{ValueFilters::SEX, "M"}});
    QTest::newRow("Woman") << filtermap(
            {{ValueFilters::SEX, "N"}});
}

void InterfaceFilter::testSexFilter()
{
    QFETCH(filtermap, filter);
    QCOMPARE(m_filter->validateFilter(filter), true);
}

void InterfaceFilter::bencmarkSexFilter()
{
    filtermap fil = filtermap({{ValueFilters::SEX, "N"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InterfaceFilter::testSexFilterFail_data()
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

void InterfaceFilter::testSexFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InterfaceFilter::bencmarkSexFilterFail()
{
    filtermap fil = filtermap({{ValueFilters::SEX, "5"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (InternetExplorers::FilterException) {

        }
    }
}

QTEST_APPLESS_MAIN(InterfaceFilter)

#include "tst_interfacefilter.moc"
