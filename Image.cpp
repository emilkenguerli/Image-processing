// Image.cpp: definition of Image class.

#include <fstream>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <memory>
#include <limits>
#include <utility>
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
  data.reset(buffer);
}

Image::Image(string file_name) 
{
  fileName = file_name;
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

bool Image::load(string file_name) 
{
  ifstream in_file;
  in_file.open(file_name, ios::binary|ios::in);
  if (!in_file) return false;

  for (std::string line;  std::getline(in_file, line);) {
    if (line.compare("255") == 0) break;
    if(line.at(0)=='#' || line.compare("P5") == 0) continue;
//    cout << line << std::endl;
    istringstream istr(line);
    istr >> ws >> height;
    istr >> ws >> width;
  }
  unsigned long int size = height*width;
  data.reset(new unsigned char[size]);
  skipws(in_file);
  in_file.read(reinterpret_cast<char*>(data.get()), size);
  return true;
}

bool Image::save(string file_name) 
{
  ofstream out_file(file_name, ios::binary|ios::out);
  if (!out_file) return false;

  out_file <<"P5"<< endl;
  out_file <<"# Output image" << endl;
  out_file << height<< " " << width << endl;
  out_file <<"255" << endl;
  out_file.write(reinterpret_cast<char*>(data.get()), height*width);
  return true;
}



}
