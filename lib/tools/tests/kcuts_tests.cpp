#include <cppunit/extensions/HelperMacros.h>

#include <TApplication.h>

#include "KCutInside.h"
#include "KCutOutside.h"
#include "KCutSmaller.h"
#include "KCutBigger.h"

class BasicCase : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE( BasicCase );
    CPPUNIT_TEST( CutTests );
    CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    void tearDown();

protected:
    void CutTests();
};

CPPUNIT_TEST_SUITE_REGISTRATION( BasicCase );

void BasicCase::setUp()
{
}

void BasicCase::tearDown()
{
}

void BasicCase::CutTests()
{
    KCutInside<Int_t> ci_w(10, 20, KT::WEAK, KT::WEAK);
    KCutInside<Int_t> ci_s(10, 20, KT::STRONG, KT::STRONG);
    KCutOutside<Int_t> co_w(10, 20, KT::WEAK, KT::WEAK);
    KCutOutside<Int_t> co_s(10, 20, KT::STRONG, KT::STRONG);
    KCutSmaller<Int_t> cs_w(20, KT::WEAK);
    KCutSmaller<Int_t> cs_s(20, KT::STRONG);
    KCutBigger<Int_t> cb_w(10, KT::WEAK);
    KCutBigger<Int_t> cb_s(10, KT::STRONG);

    CPPUNIT_ASSERT(ci_w.test(15) == true);
    CPPUNIT_ASSERT(ci_w.test(10) == true);
    CPPUNIT_ASSERT(ci_w.test(20) == true);

    CPPUNIT_ASSERT(ci_s.test(15) == true);
    CPPUNIT_ASSERT(ci_s.test(10) == false);
    CPPUNIT_ASSERT(ci_s.test(20) == false);

    CPPUNIT_ASSERT(co_w.test(5) == true);
    CPPUNIT_ASSERT(co_w.test(10) == true);
    CPPUNIT_ASSERT(co_w.test(20) == true);
    CPPUNIT_ASSERT(co_w.test(25) == true);

    CPPUNIT_ASSERT(co_s.test(5) == true);
    CPPUNIT_ASSERT(co_s.test(10) == false);
    CPPUNIT_ASSERT(co_s.test(20) == false);
    CPPUNIT_ASSERT(co_s.test(25) == true);

    CPPUNIT_ASSERT(cs_w.test(15) == true);
    CPPUNIT_ASSERT(cs_w.test(20) == true);
    CPPUNIT_ASSERT(cs_w.test(25) == false);

    CPPUNIT_ASSERT(cs_s.test(15) == true);
    CPPUNIT_ASSERT(cs_s.test(20) == false);
    CPPUNIT_ASSERT(cs_s.test(25) == false);

    CPPUNIT_ASSERT(cb_w.test(5) == false);
    CPPUNIT_ASSERT(cb_w.test(10) == true);
    CPPUNIT_ASSERT(cb_w.test(15) == true);

    CPPUNIT_ASSERT(cb_s.test(5) == false);
    CPPUNIT_ASSERT(cb_s.test(10) == false);
    CPPUNIT_ASSERT(cb_s.test(15) == true);
}
