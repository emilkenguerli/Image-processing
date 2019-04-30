// image.h: declaration of image class.
#ifndef IMAGE_H
#define IMAGE_H

#include <iterator>
#include <memory>

using namespace std;

namespace KNGEMI002 {

  class Image {

    private:
      int width, height;
      unique_ptr<unsigned char[]> data;
      string file_name;
      struct Filter {
        float x;
        float y;
      };

    public:
      void copy(const Image& rhs);
      bool load();
      bool save(string name); // make private later
      Image(); // default ctor
      Image(int w, int h, unsigned char* buffer); // ctor that takes in width, height and a buffer of values
      Image(string file_name); // constructor that takes in file name
      ~Image(); // dtor

      Image(const Image& orig_image); // copy ctor
      Image(Image&& orig_image); // move ctor

      Image operator+(const Image& rhs); // add images
      Image operator-(const Image& rhs); // substract images
      Image operator!(); // invert images
      Image operator/(const Image& rhs); // mask image
      Image operator*(int f); // threshold f
      bool operator==(const Image& rhs); 
      Image operator%(const Filter & g);

      friend ostream& operator<<(ostream& lhs, const Image& rhs); // out stream 
      friend ostream& operator>>(ifstream& lhs, Image& rhs); // out stream 

      int get_width() { return width; }
      int get_height() { return height; }
      const unique_ptr<unsigned char[]>& get_data() { return data; }

      class iterator {
        private:
          unsigned char *ptr;
          friend class Image; // to get access to Image class methods
          iterator(u_char *p) : ptr(p) {} // construct only via Image class (begin/end)
          ~iterator() { ptr = nullptr; }
          int i;
        public:  
        //copy construct is public
        iterator(const iterator & rhs) : ptr(rhs.ptr) {};
   
     // define overloaded ops: *, ++, --, =
        iterator& operator=(const iterator & rhs);
        iterator& operator=(iterator && rhs);
     // other methods for iterator
        unsigned char &  operator*();
        iterator& operator++();
        iterator operator++(const int);
        iterator& operator--();
        bool operator!=(const iterator& rhs);
        const iterator operator+(const iterator& rhs) const;
      };
      // define begin()/end() to get iterator to start and
      // "one-past" end.
      iterator begin(void) const;
      iterator end(void) const;
  };
}
#endif