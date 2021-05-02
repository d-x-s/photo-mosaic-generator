// File:        main.cpp
// Author:      Cinda
// Date:        2020-10-31
// Description: development of gold code


#include "rgbtree.h"
#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include "tileUtil.h"
#include <iostream>
#include <map>

#define TILESIZE 30

using namespace std;
using namespace cs221util;
using namespace tiler;


int main()
{

    // read directory and create map of average color -> file name (this function is given)
    map<RGBAPixel, string> photos = buildMap("imlib/");
    
    // build the kd tree given the photos map.  (you'll implement a rgbtree)
    rgbtree searchStructure(photos);

    // read a (small, 100x150 or so) target image into timage
    // tile(timage) returns a TILESIZExwidth by TILESIZExheight image corresponding
    // to the target.
    PNG timage; timage.readFromFile("targets/pyang25.png");

    // functionality of tile: for each pixel in the target image, find pixel's NN
    // in the kdtree, returning a photoID. Use the photoID to open the 
    // correct file, and use that file's pixels in the appropriate place
    // in the return image. You'll implement this function in __________________
    PNG mosaic = tile(timage, searchStructure, photos);

    mosaic.writeToFile("targets/mosaic.png");

  return 0;
}
