// Image.cpp: definition of Image class.

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include "Image.h"

namespace KNGEMI002 {
Image::Image() 
{
  width = 0;
  height = 0;
}

Image::Image(int w, int h, unsigned char* buffer) 
{
  height = h;
  width = w;
  data.reset(buffer);
}

Image::Image(string name) 
{
  file_name = name;
}

// copy constructor
Image::Image(const Image& orig_image) 
{
  width = orig_image.width; 
  height = orig_image.height; 
  unsigned char* buffer  = new unsigned char[width*height];
  for(int i = 0; i < width*height; i++) 
    buffer[i] =  orig_image.data[i];
  data.reset(buffer);
}

// move constructor
Image::Image(Image&& orig_image) 
{
  width = orig_image.width; 
  height = orig_image.height; 
  unsigned char* buffer  = new unsigned char[width*height];
  for(int i = 0; i < width*height; i++) 
    buffer[i] =  orig_image.data[i];
  data.reset(buffer);
  orig_image.height = orig_image.width = 0;
  orig_image.data = nullptr;
}

Image::~Image() 
{
  data = nullptr;
  height = width = 0;
}

bool Image::load() 
{
  ifstream in_file;
  in_file.open(file_name, ios::binary|ios::in);
  if (!in_file) return false;
  in_file >> *this;
  return true;
}

bool Image::save(string file_name) 
{
  ofstream out_file(file_name, ios::binary|ios::out);
  if (!out_file) return false;
  out_file << *this;
  return true;
}

void Image::copy(const Image& rhs)
{
  Image::iterator beg = this->begin(), end = this->end();
  Image::iterator inStart = rhs.begin(), inEnd = rhs.end();
  while (beg != end) { *beg = *inStart; ++beg; ++inStart; }
}

Image::iterator Image::begin(void) const
{ 
  return iterator(data.get());
}

Image::iterator Image::end(void) const
{
   return iterator(data.get()+width*height);
}

// Add I1 I2 : add the pixel values of I1 to I2
Image Image::operator+(const Image& rhs)
{
  Image::iterator beg = this->begin(), end = this->end();
  Image::iterator inStart = rhs.begin(), inEnd = rhs.end();
  while (beg != end) { 
    if((*beg + *inStart) > 255) *beg = 255;
    ++beg;
    ++inStart;
  } 
  return *this;
}

// Subtract I1 I2 : subtract pixel values of I2 from I1
Image Image::operator-(const Image& rhs)
{
  Image::iterator beg = this->begin(), end = this->end();
  Image::iterator inStart = rhs.begin(), inEnd = rhs.end();
  while (beg != end) { 
    if((*beg - *inStart) < 0) *beg = 0;
    ++beg;
    ++inStart;
  } 
  return *this;
}

// Invert I1 : replace each pixel value p with (255 - p)
Image Image::operator!()
{
  Image::iterator beg = this->begin(), end = this->end();
  while (beg != end) { 
    *beg = 255 - *beg;
    ++beg;
  } 
  return *this;
}

// Mask I1 I2 : given I1 and an image I2, copy across values from I1 where I2
// has a value of 255. All other output values are set to 0.
Image Image::operator/(const Image& rhs) 
{
  Image::iterator beg = this->begin(), end = this->end();
  Image::iterator inStart = rhs.begin(), inEnd = rhs.end();
  while (beg != end) { 
    if(*inStart != 255) *beg = 0;
    ++beg;
    ++inStart;
  } 
  return *this;
}

// Threshold I1 f : for all pixels in I1 > f, set the result to the integer 255,
// otherwise set the value to 0. You can build a mask using this function.
Image Image::operator*(int f)
{
  Image::iterator beg = this->begin(), end = this->end();
  while (beg != end) { 
    if (*beg > f) *beg = 255;
    else *beg = 0;
    ++beg;
  } 
  return *this;
}

bool Image::operator==(const Image& rhs) 
{
  for(int i = 0; i < sizeof(data.get()); i++) {
      if(data[i]!= rhs.data[i])
         return false;
  }
  return true;
}

// Implement I/O operators for Image: <<
ostream& operator<<(ostream& lhs, const Image& rhs) 
{
  lhs << "P5" << endl;
  lhs << "# Output image" << endl;
  lhs << rhs.height << " " << rhs.width << endl;
  lhs << "255" << endl;
  lhs.write(reinterpret_cast<char*>(rhs.data.get()), rhs.height*rhs.width);
}

//Implement I/O operators for Image: >>
ostream& operator>>(ifstream& lhs, Image& rhs)
{
  for (std::string line;  std::getline(lhs, line);) {
    if (line.compare("255") == 0) break;
    if(line.at(0)=='#' || line.compare("P5") == 0) continue;
    istringstream istr(line);
    istr >> ws >> rhs.height;
    istr >> ws >> rhs.width;
  }
  unsigned long int size = rhs.height*rhs.width;
  rhs.data.reset(new unsigned char[size]);
  skipws(lhs);
  lhs.read(reinterpret_cast<char*>(rhs.data.get()), size);
} 

// Implement a further “filter” operator (%) for your image which will apply a filter g to the image
Image Image::operator%(const Image::Filter & g)
{
}

// iterator class definitions follow

Image::iterator & Image::iterator::operator=(const Image::iterator & rhs)
{ 
  if(this == &rhs) return *this;
  ptr = rhs.ptr;
  return *this;
}

Image::iterator&  Image::iterator::operator=(iterator && rhs)
{
  if(this != &rhs) ptr = rhs.ptr;
  rhs = nullptr;
  return *this;
}

unsigned char & Image::iterator::operator*() {
  return *ptr; 
}

Image::iterator & Image::iterator::operator++()
{ 
  ptr++;
  return *this; 
}

Image::iterator Image::iterator::operator++(const int)
{
  iterator old(*this); 
  ++ptr;
  return old; 
}

Image::iterator & Image::iterator::operator--() 
{ 
  return --*this; 
}

bool Image::iterator::operator!=(const iterator& rhs)
{
  return (ptr != rhs.ptr);
}

const Image::iterator Image::iterator::operator+(const iterator& rhs) const
{
  iterator tmp(*this);
  tmp = tmp + rhs;
  return tmp;
}

}
