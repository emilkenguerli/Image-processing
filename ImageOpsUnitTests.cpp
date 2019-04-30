#include <iostream>
#include "Image.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"


TEST_CASE("Test Load and Save", "[LOAD/SAVE OPERATION]")
{
  // buffer for test image
  unsigned char* buffer = new unsigned char[16];
  for(int i = 0; i < 16; i++) {
    buffer[i] = 128 + 4*i;
  }
  KNGEMI002::Image im_test(4,4, buffer); // test image
  im_test.save("Test.pgm");
  KNGEMI002::Image im1("Test.pgm");
  if (!im1.load()) {
    cerr << "Couldn't load Test.pgm" << endl;
  }

// saved image identical to loaded image
  REQUIRE((im_test == im1));
}

TEST_CASE("Test Addition", "[ADD OPERATION]")
{
  // buffer for test image
  unsigned char* buffer1 = new unsigned char[4];
  for(int i = 0; i < 4; i++) {
    buffer1[i] = 100;
  }
  // buffer for white image
  unsigned char* buffer0 = new unsigned char[4];
  for(int i = 0; i < 4; i++) {
    buffer0[i] = 255;
  }
  KNGEMI002::Image im_white(2,2, buffer0); // white image
  KNGEMI002::Image im_test(2,2, buffer1); // test image

  KNGEMI002::Image invert = !im_test; // inverted image
  KNGEMI002::Image sum = im_test + invert; // sum of test and inverted image
  // sum of test and inverted image should give white image
  REQUIRE((sum == im_white));
}

TEST_CASE("Test Invertion", "[INVERT OPERATION]")
{
  // buffer for black image
  unsigned char* buffer1 = new unsigned char[4];
  for(int i = 0; i < 4; i++) {
    buffer1[i] = 0;
  }
  // buffer for white image
  unsigned char* buffer0 = new unsigned char[4];
  for(int i = 0; i < 4; i++) {
    buffer0[i] = 255;
  }
  KNGEMI002::Image im_white(2,2, buffer0); // white image
  KNGEMI002::Image im_black(2,2, buffer1); // test image

  KNGEMI002::Image invert = !im_black; // inverted image
  // invert of black image should give white image
  REQUIRE((invert == im_white));
}

TEST_CASE("Test Subtraction", "[SUBTRACT OPERATION]")
{
  // buffer for black image
  unsigned char* buffer1 = new unsigned char[4];
  for(int i = 0; i < 1; i++) {
    buffer1[i] = 0;
  }
  // buffer for white image
  unsigned char* buffer0 = new unsigned char[4];
  for(int i = 0; i < 1; i++) {
    buffer0[i] = 255;
  }
  KNGEMI002::Image im_white(1,1, buffer0); // white image
  KNGEMI002::Image im_black(1,1, buffer1); // black image
  KNGEMI002::Image sub1 = im_white - im_black;

// subtract black image from white image should give white image
  REQUIRE((sub1 == im_white));
}

TEST_CASE("Test Mask", "[MASK OPERATION]")
{
  // buffer for black image
  unsigned char* buffer1 = new unsigned char[4];
  for(int i = 0; i < 4; i++) {
    buffer1[i] = 0;
  }
  // buffer for test image
  unsigned char* buffer2 = new unsigned char[4];
  for(int i = 0; i < 4; i++) {
    buffer1[i] = 128;
  }
  KNGEMI002::Image im_test(2,2, buffer2); // test image
  KNGEMI002::Image im_black(2,2, buffer1); // black image
  KNGEMI002::Image mask2 = im_test / im_black;
  // test image masked by black image should produce the original image
  REQUIRE((mask2 == im_test));
}

TEST_CASE("A simple qualitative test", "[QUALITATIVE TEST]")
{
  unsigned char* buffer = new unsigned char[169];
  int k = 0;
  int val = 15;
  for(int i = 0; i < 13; i++) {
    for(int j = 0; j < 13; j++) {
      buffer[k] = val; 
    }
    val += 16;
  }
  KNGEMI002::Image m(13,13, buffer); // test image m

// create an image mask M1 (via thresholding)
  KNGEMI002::Image m1 = m*128; 
  REQUIRE(m1.get_width() == m.get_width());
  REQUIRE(m1.get_height() == m.get_height());

// create an inverted mask of M1 called M2 (by inverting M1)
  KNGEMI002::Image m2 = !m1; 
  REQUIRE(m1.get_width() == m2.get_width());
  REQUIRE(m1.get_height() == m2.get_height());

// use M1 and an Image U1 to create a masked image
  unsigned char* buffer1 = new unsigned char[169];
  int kw = 0;
  int valw = 255;
  for(int i = 0; i < 13; i++) {
    for(int j = 0; j < 13; j++) {
      buffer1[k] = val; 
    }
    val -= 16;
  }
  KNGEMI002::Image u1(13,13, buffer1); // test image u1
  KNGEMI002::Image mask1 = m1 / u1;
  REQUIRE(mask1.get_width() == u1.get_width());
  REQUIRE(mask1.get_height() == u1.get_height());

// use M2 and an Image U2 to create a second masked image
  unsigned char* buffer2 = new unsigned char[4];
  for(int i = 0; i < 4; i++) {
    buffer2[i] = 255;
  }
  KNGEMI002::Image u2(13,13, buffer2); // test image u2
  KNGEMI002::Image mask2 = m2 / u2;
  REQUIRE(mask2.get_width() == u2.get_width());
  REQUIRE(mask2.get_height() == u2.get_height());

// add the two masked images together to create a new image with pieces of both U1 and U2!
  KNGEMI002::Image new_image = mask1 + mask2;
  REQUIRE(new_image.get_width() == mask2.get_width());
  REQUIRE(new_image.get_height() == mask2.get_height());
}
