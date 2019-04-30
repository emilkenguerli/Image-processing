// image.h: declaration of image class.
#ifndef IMAGE_H
#define IMAGE_H

#include <iterator>

using namespace std;

namespace KNGEMI002 {

  class Image {

    private:
      int width, height;
      unique_ptr<unsigned char[]> data;
      string fileName;

    public:
      void copy(const Image& rhs);
      bool load(string fileName);
      bool save(string fileName); // make private later
      Image(); // default ctor
      Image(int w, int h, unsigned char* buffer); // ctor that takes in width, height and a buffer of values
      Image(string file_name); // constructor that takes in file name
      ~Image(); // dtor

      Image(const Image& orig_image); // copy ctor
      Image(Image&& orig_image); // move ctor

      Image operator+(const Image& rhs); // add images
      Image operator-(const Image& rhs); // substract images
      Image operator!(); // invert images



}
#endif