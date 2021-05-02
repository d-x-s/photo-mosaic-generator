#ifndef _TILER_
#define _TILER_

#include "rgbtree.h"
#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <map>
namespace fs = std::filesystem;


#define TILESIZE 30

namespace tiler {

/**
 * Function tile:
 * @param PNG & target: an image to use as base for the mosaic. it's pixels will be
 *                      be replaced by thumbnail images whose average color is close
 *                      to the pixel.
 * @param rgbtree & ss: a kd-tree of RGBAPixels, used as a query structure for
 *                      nearest neighbor search. 
 * @param map<RGBAPixel, string> & photos: a map that takes a color key and returns the
 *                      filename of an image whose average color is that key.
 *
 * returns: a PNG whose dimensions are TILESIZE times that of the target. Each
 * pixel in the target is used as a query to ss.findNearestNeighbor, and the response
 * is used as a key in photos. 
 */

PNG tile(PNG & target, const rgbtree & ss, map<RGBAPixel,string> & photos);

/* buildMap: function for building the map of <key, value> pairs, where the key is an
 * RGBAPixel representing the average color over an image, and the value is 
 * a string representing the path/filename.png of the TILESIZExTILESIZE image
 * whose average color is the key.
 * 
 * We've provided a bit of the C++ code that allows you to iterate over the files
 * in a directory. It is up to you to figure out what it means, and how to use it.
 * 
 * @param path is the subdirectory in which the tiles can be found. In our examples
 * this is imlib.
 *
*/
map<RGBAPixel, string> buildMap(string path);

//PNG renderThumbNailOntoMosaic(PNG & thumbnail, PNG & mosaic, int positionX, int positionY);
void render(int xPos, int yPos, PNG & mosaic, PNG & thumbnailTester);
}

#endif
// #ifndef _TILER_
// #define _TILER_

// #include "rgbtree.h"
// #include "cs221util/PNG.h"
// #include "cs221util/RGBAPixel.h"
// #include <filesystem>
// #include <iostream>
// #include <string>
// #include <map>
// namespace fs = std::filesystem;


// #define TILESIZE 30

// namespace tiler {

// /**
//  * Function tile:
//  * @param PNG & target: an image to use as base for the mosaic. it's pixels will be
//  *                      be replaced by thumbnail images whose average color is close
//  *                      to the pixel.
//  * @param rgbtree & ss: a kd-tree of RGBAPixels, used as a query structure for
//  *                      nearest neighbor search. 
//  * @param map<RGBAPixel, string> & photos: a map that takes a color key and returns the
//  *                      filename of an image whose average color is that key.
//  *
//  * returns: a PNG whose dimensions are TILESIZE times that of the target. Each
//  * pixel in the target is used as a query to ss.findNearestNeighbor, and the response
//  * is used as a key in photos. 
//  */

// PNG tile(PNG & target, const rgbtree & ss, map<RGBAPixel,string> & photos);

// /* buildMap: function for building the map of <key, value> pairs, where the key is an
//  * RGBAPixel representing the average color over an image, and the value is 
//  * a string representing the path/filename.png of the TILESIZExTILESIZE image
//  * whose average color is the key.
//  * 
//  * We've provided a bit of the C++ code that allows you to iterate over the files
//  * in a directory. It is up to you to figure out what it means, and how to use it.
//  * 
//  * @param path is the subdirectory in which the tiles can be found. In our examples
//  * this is imlib.
//  *
// */
// map<RGBAPixel, string> buildMap(string path);

// //PNG renderThumbNailOntoMosaic(PNG & thumbnail, PNG & mosaic, int positionX, int positionY);
// void render(int xPos, int yPos, PNG & mosaic, PNG & thumbnailTester);
// }

// #endif
