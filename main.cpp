#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <memory>
#include "Image.h"


using namespace std;


static void show_usage(string name)
{
  cerr << "Usage: " << name << " <option> OutputImageName" << endl
       << "Options:" << endl
       << "\t-h,--help\t\tshow this help message\n"
       << "\t-a I1 I2\t\tadd images I1 and I2" << endl
       << "\t-s I1 I2\t\tsubtract image I2 from I1" << endl
       << "\t-i I1\t\t\tinvert I1" << endl
       << "\t-l I1 I2\t\tmask I1 with I2" << endl
       << "\t-t I1 f\t\t\tthreshold I1 with integer value f" << endl
       << endl;
}

int main(int argc, char* argv[])
{
  if (argc < 3 || argc > 5) {
    show_usage(argv[0]);
    return 1;
  }

  string arg = argv[1];
  string InputImageName1;
  string InputImageName2;
  string OutputImageName;

  if (arg == "-a") { // add
    if (argc != 5) {
      show_usage(argv[0]);
      return 1;
    }
    InputImageName1 = argv[2];
    InputImageName2 = argv[3];
    OutputImageName = argv[4];
    KNGEMI002::Image im1(InputImageName1);
    if (!im1.load()) {
      cerr << "Couldn't load " << InputImageName1 << endl;
      return 0;
    }
    KNGEMI002::Image im2(InputImageName2);
    if (!im2.load()) {
      cerr << "Couldn't load " << InputImageName2 << endl;
      return 0;
    }
    KNGEMI002::Image sum = im1 + im2;
    sum.save(OutputImageName);
  }
  else if (arg == "-s") { // substract
    if (argc != 5) {
      show_usage(argv[0]);
      return 1;
    }
    InputImageName1 = argv[2];
    InputImageName2 = argv[3];
    OutputImageName = argv[4];
    KNGEMI002::Image im1(InputImageName1);
    if (!im1.load()) {
      cerr << "Couldn't load " << InputImageName1 << endl;
      return 0;
    }
    KNGEMI002::Image im2(InputImageName2);
    if (!im2.load()) {
      cerr << "Couldn't load " << InputImageName2 << endl;
      return 0;
    }
    KNGEMI002::Image diff = im1 - im2;
    diff.save(OutputImageName);
  }
  else if (arg == "-i") {  // invert
    if (argc != 4) {
      show_usage(argv[0]);
      return 1;
    }
    InputImageName1 = argv[2];
    OutputImageName = argv[3];
    KNGEMI002::Image im(InputImageName1);
    if (!im.load()) {
      cerr << "Couldn't load " << InputImageName1 << endl;
      return 0;
    }
    KNGEMI002::Image inv = !im;
    inv.save(OutputImageName);
  }
  else if (arg == "-l") { // mask
    if (argc != 5) {
      show_usage(argv[0]);
      return 1;
    }
    InputImageName1 = argv[2];
    InputImageName2 = argv[3];
    OutputImageName = argv[4];
    KNGEMI002::Image im1(InputImageName1);
    if (!im1.load()) {
      cerr << "Couldn't load " << InputImageName1 << endl;
      return 0;
    }
    KNGEMI002::Image im2(InputImageName2);
    if (!im2.load()) {
      cerr << "Couldn't load " << InputImageName2 << endl;
      return 0;
    }
    KNGEMI002::Image mask = im1 / im2;
    mask.save(OutputImageName);
  }
  else if (arg == "-t") {
    if (argc != 5) {
      show_usage(argv[0]);
      return 1;
    }
    InputImageName1 = argv[2];
    int f = std::stoi(argv[3]);
    OutputImageName = argv[4];
    KNGEMI002::Image im(InputImageName1);
    if (!im.load()) {
      cerr << "Couldn't load " << InputImageName1 << endl;
      return 0;
    }
    KNGEMI002::Image thres = im * f;
    thres.save(OutputImageName);  
  }
  else {
    show_usage(argv[0]);
    return 1;
  }


    return 0;
}

