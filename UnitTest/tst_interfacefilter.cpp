#include <QtTest>
#include <QObject>

#include "interfacefilter.h"

typedef std::map<InternetExplorers::Constants::Filter::ValueFilters, QString> filtermap;
Q_DECLARE_METATYPE(filtermap)

class InterfaceFilter : public QObject
{
    Q_OBJECT

public:
    InterfaceFilter();
    ~InterfaceFilter();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    /* YEAR FILTER */
    void testYearFilter_data();
    void testYearFilter();
    void bencmarkYearFilter();

    void testYearFilterFail_data();
    void testYearFilterFail();
    void bencmarkYearFilterFail();
    /* !YEAR FILTER */

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

void InterfaceFilter::testYearFilter_data()
{
    QTest::addColumn<std::map<InternetExplorers::Constants::Filter::ValueFilters, QString>>("filter");

    QTest::newRow("Lower bound") << filtermap(
            {{InternetExplorers::Constants::Filter::ValueFilters::YEAR, "1974"}});
    QTest::newRow("Upper bound") << filtermap(
            {{InternetExplorers::Constants::Filter::ValueFilters::YEAR, "2019"}});
    QTest::newRow("Normal year") << filtermap(
            {{InternetExplorers::Constants::Filter::ValueFilters::YEAR, "1996"}});
}

void InterfaceFilter::testYearFilter()
{
    QFETCH(filtermap, filter);
    QCOMPARE(m_filter->validateFilter(filter), true);
}

void InterfaceFilter::bencmarkYearFilter()
{
    filtermap fil = filtermap({{InternetExplorers::Constants::Filter::ValueFilters::YEAR, "1996"}});
    QBENCHMARK{
        m_filter->validateFilter(fil);
    }
}

void InterfaceFilter::testYearFilterFail_data()
{
    QTest::addColumn<std::map<InternetExplorers::Constants::Filter::ValueFilters, QString>>("filter");

    QTest::newRow("Empty") << filtermap(
            {{InternetExplorers::Constants::Filter::ValueFilters::YEAR, ""}});
    QTest::newRow("Only space") << filtermap(
            {{InternetExplorers::Constants::Filter::ValueFilters::YEAR, "    "}});
    QTest::newRow("Kaikki vuodet") << filtermap(
            {{InternetExplorers::Constants::Filter::ValueFilters::YEAR, "Kaikki vuodet"}});
    QTest::newRow("Random string") << filtermap(
            {{InternetExplorers::Constants::Filter::ValueFilters::YEAR, "Justtesting"}});
    QTest::newRow("Too small year") << filtermap(
            {{InternetExplorers::Constants::Filter::ValueFilters::YEAR, "1917"}});
    QTest::newRow("Too big year") << filtermap(
            {{InternetExplorers::Constants::Filter::ValueFilters::YEAR, "2420"}});
}

void InterfaceFilter::testYearFilterFail()
{
    QFETCH(filtermap, filter);
    QVERIFY_EXCEPTION_THROWN(m_filter->validateFilter(filter), InternetExplorers::FilterException);
}

void InterfaceFilter::bencmarkYearFilterFail()
{
    filtermap fil = filtermap({{InternetExplorers::Constants::Filter::ValueFilters::YEAR, "2420"}});
    QBENCHMARK{
        try {
            m_filter->validateFilter(fil);
        } catch (InternetExplorers::FilterException) {

        }
    }
}

QTEST_APPLESS_MAIN(InterfaceFilter)

#include "tst_interfacefilter.moc"
