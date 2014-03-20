#include "../ImageStack.hpp"
#include "SampleImage.hpp"
#include <boost/test/unit_test.hpp>
using namespace hdrmerge;
using namespace std;


static const char * image1 = "test/sample1.dng";
static const char * image2 = "test/sample2.dng";
// Sample images with deviation
static const char * image3 = "test/sample1.png"; // (0, 0)
static const char * image4 = "test/sample2.png"; // (20, 32)
static const char * image5 = "test/sample3.png"; // (38, 26)
static const char * image6 = "test/sample4.png"; // (34, -4)


// BOOST_AUTO_TEST_CASE(testSampleImage) {
//     SampleImage si(image3);
//     save("sample1bis.png", si.pixelData, si.md.width, si.md.height);
// }


BOOST_AUTO_TEST_CASE( image_load ) {
    Image e1(image1);
    BOOST_REQUIRE(e1.good());
    Image e2(image2);
    BOOST_REQUIRE(e2.good());
    BOOST_REQUIRE(e2.isSameFormat(e1));
    Image e3(image3);
    BOOST_CHECK(!e3.good());
}


// BOOST_AUTO_TEST_CASE(image_scale) {
//     SampleImage si1(image3);
//     Image im(si1.pixelData, si1.md);
//     BOOST_REQUIRE(im.good());
//     char filename[] = "scaledx.png";
//     size_t w = im.getWidth(), h = im.getHeight();
//     for (int i = 0; i < Image::scaleSteps; ++i) {
//         filename[6] = '0' + i;
//         save(filename, im.getPixels(i), w, h);
//         w >>= 1;
//         h >>= 1;
//     }
// }

BOOST_AUTO_TEST_CASE(image_align) {
    SampleImage si1(image3);
    SampleImage si2(image4);
    SampleImage si3(image5);
    SampleImage si4(image6);
    Image e1(si1.pixelData, si1.md);
    Image e2(si2.pixelData, si2.md);
    Image e3(si3.pixelData, si3.md);
    Image e4(si4.pixelData, si4.md);
    BOOST_REQUIRE(e1.good());
    BOOST_REQUIRE(e2.good());
    BOOST_REQUIRE(e3.good());
    BOOST_REQUIRE(e4.good());
    e2.alignWith(e1, 0.5, 1.0/64);
    e3.alignWith(e1, 0.5, 1.0/64);
    e4.alignWith(e1, 0.5, 1.0/64);
    BOOST_CHECK_EQUAL(e1.getDeltaX(), 0);
    BOOST_CHECK_EQUAL(e1.getDeltaY(), 0);
    BOOST_CHECK_EQUAL(e2.getDeltaX(), 20);
    BOOST_CHECK_EQUAL(e2.getDeltaY(), 32);
    BOOST_CHECK_EQUAL(e3.getDeltaX(), 38);
    BOOST_CHECK_EQUAL(e3.getDeltaY(), 26);
    BOOST_CHECK_EQUAL(e4.getDeltaX(), 32);
    BOOST_CHECK_EQUAL(e4.getDeltaY(), -4);
    e3.alignWith(e2, 0.5, 1.0/64);
    e4.alignWith(e3, 0.5, 1.0/64);
    BOOST_CHECK_EQUAL(e3.getDeltaX(), 18);
    BOOST_CHECK_EQUAL(e3.getDeltaY(), -6);
    BOOST_CHECK_EQUAL(e4.getDeltaX(), -6);
    BOOST_CHECK_EQUAL(e4.getDeltaY(), -30);
}

BOOST_AUTO_TEST_CASE(stack_load) {
    ImageStack images;
    BOOST_CHECK_EQUAL(images.size(), 0);
    unique_ptr<Image> e1(new Image(image2)), e2(new Image(image1));
    Image & e1ref = *e1, & e2ref = *e2;
    BOOST_REQUIRE(e1->good());
    BOOST_REQUIRE(e2->good());
    BOOST_REQUIRE(images.addImage(e1));
    BOOST_CHECK(!e1.get());
    BOOST_CHECK(images.addImage(e2));
    BOOST_CHECK_EQUAL(images.size(), 2);
    BOOST_CHECK_EQUAL(&images.getImage(0), &e2ref);
    BOOST_CHECK_EQUAL(images.getImage(0).getWidth(), images.getWidth());
    BOOST_CHECK_EQUAL(images.getImage(0).getHeight(), images.getHeight());
}

BOOST_AUTO_TEST_CASE(stack_align) {
    ImageStack images;
    SampleImage si1(image3);
    SampleImage si2(image4);
    SampleImage si3(image5);
    SampleImage si4(image6);
    unique_ptr<Image> e1(new Image(si1.pixelData, si1.md)),
        e2(new Image(si2.pixelData, si2.md)),
        e3(new Image(si3.pixelData, si3.md)),
        e4(new Image(si4.pixelData, si4.md));
    Image & e1ref = *e1, & e2ref = *e2, & e3ref = *e3, & e4ref = *e4;
    BOOST_REQUIRE(e1->good());
    BOOST_REQUIRE(e2->good());
    BOOST_REQUIRE(e3->good());
    BOOST_REQUIRE(e4->good());
    BOOST_REQUIRE(images.addImage(e1));
    BOOST_REQUIRE(images.addImage(e2));
    BOOST_REQUIRE(images.addImage(e3));
    BOOST_REQUIRE(images.addImage(e4));
    images.align();
    BOOST_CHECK_EQUAL(images.getWidth(), 962);
    BOOST_CHECK_EQUAL(images.getHeight(), 564);
    BOOST_CHECK_EQUAL(e1ref.getDeltaX(), -38);
    BOOST_CHECK_EQUAL(e1ref.getDeltaY(), -32);
    BOOST_CHECK_EQUAL(e2ref.getDeltaX(), -18);
    BOOST_CHECK_EQUAL(e2ref.getDeltaY(), 0);
    BOOST_CHECK_EQUAL(e3ref.getDeltaX(), 0);
    BOOST_CHECK_EQUAL(e3ref.getDeltaY(), -6);
    BOOST_CHECK_EQUAL(e4ref.getDeltaX(), -6);
    BOOST_CHECK_EQUAL(e4ref.getDeltaY(), -36);
}
