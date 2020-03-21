#include <QtTest>
#include <QObject>

#include "interfacefilter.h"

class InterfaceFilter : public QObject
{
    Q_OBJECT

public:
    InterfaceFilter();
    ~InterfaceFilter();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void test_case1();

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

QTEST_APPLESS_MAIN(InterfaceFilter)

#include "tst_interfacefilter.moc"
