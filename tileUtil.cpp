
// File:        tileUtil.cpp
// Author:      Cinda
// Date:        2020-10-31
// Description: file handling functions that prepare data for the tiling algorithm


#include "tileUtil.h"

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

PNG tiler::tile(PNG & target, const rgbtree & ss, map<RGBAPixel,string> & photos)
{   
    PNG mosaic = PNG(target);

    //since each pixel is replaced by a 30x30 thumbnail, we expand each dimension by a factor of 30
    unsigned int newHeight = target.height() * 30;
    unsigned int newWidth = target.width() * 30;
    mosaic.resize(newWidth, newHeight);

    //for each pixel in target, do NN search with it on rgbtree ss
    //NN search will return a pixel (closest point), which will be a key in photos map
    //plug the key into photos map to get a string representing filepath to a thumbnail
    //put the thumbnail onto mosaic
    for (unsigned x = 0; x < target.width(); x++) {
        for (unsigned y = 0; y < target.height(); y++) {

            RGBAPixel * query = target.getPixel(x, y);
            RGBAPixel querySub = RGBAPixel();
            querySub.r = query->r;
            querySub.g = query->g;
            querySub.b = query->b;
            // RGBAPixel querySub = RGBAPixel();
            // querySub.r = 200;
            // querySub.g = 200;
            // querySub.b = 200;
            RGBAPixel closest = ss.findNearestNeighbor(querySub);
            string filePath = photos[closest];
            PNG thumbnail; thumbnail.readFromFile(filePath);

            //PNG thumbnail; thumbnail.readFromFile("imlib/99359743_aa94427a3f_s.png");

            render(30*x, 30*y, mosaic, thumbnail);
                  
        }
    }
    return mosaic;
}

void tiler::render(int xPos, int yPos, PNG & mosaic, PNG & thumbnailTester)
{
    for (unsigned i = 0; i < thumbnailTester.width(); i++) {
        for (unsigned j = 0; j < thumbnailTester.height(); j++) {

            RGBAPixel * thumbnailTemp = thumbnailTester.getPixel(i, j);
            RGBAPixel * mosaicTemp = mosaic.getPixel(xPos+i, yPos+j);
            mosaicTemp->r = thumbnailTemp->r;
            mosaicTemp->g = thumbnailTemp->g;
            mosaicTemp->b = thumbnailTemp->b;

        }
    }
}

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
map<RGBAPixel, string> tiler::buildMap(string path) 
{

    map <RGBAPixel, string> thumbs;

    for (const auto & entry : fs::directory_iterator(path)) 
    
    {
    PNG curr; curr.readFromFile(entry.path());

    // find average color by adding up RGB values of all the pixels, then dividing by # pixels
    // using PNG curr, iterate through every single pixel, adding up the r, g, b channels succesivvely, then divide the sums by # of pixels
    // afterwards, construct an RGBAPixel with those averaged values
    // map the newly constructed average RGBAPixel with its corresponding picture, the value being the filePath

    // one image corresponds to one <K,V> pair in the result map, namely <RGBAPixel,String>

        int currHeight = curr.height();
        int currWidth  = curr.width(); 
        int currArea   = currHeight * currWidth; //# pixels in an image 

        int sumR = 0;
        int sumG = 0;
        int sumB = 0; 

        for (unsigned x = 0; x < curr.width(); x++) {
            for (unsigned y = 0; y < curr.height(); y++) {
                
                RGBAPixel *pixel = curr.getPixel(x, y);
                sumR = sumR + pixel->r;
                sumG = sumG + pixel->g;
                sumB = sumB + pixel->b;
        
            }
        }
    
        //calculate the average R, G, B values 
        int averageR = sumR/currArea;
        int averageG = sumG/currArea;
        int averageB = sumB/currArea;
        
        //construct the new RGBAPixel object representing the average color of the image
        RGBAPixel averagePixel = RGBAPixel(averageR, averageG, averageB, 255);
        //RGBAPixel * averagePixel = new RGBAPixel(averageR, averageG, averageB, 255);

        //update our map with key (averagePixel) and corresponding value (file path)
        thumbs[averagePixel] = entry.path();
        
    }
    return thumbs;
}


// // File:        tileUtil.cpp
// // Author:      Cinda
// // Date:        2020-10-31
// // Description: file handling functions that prepare data for the tiling algorithm


// #include "tileUtil.h"

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

// PNG tiler::tile(PNG & target, const rgbtree & ss, map<RGBAPixel,string> & photos)
// {   
//     PNG mosaic = PNG(target);

//     //since each pixel is replaced by a 30x30 thumbnail, we expand each dimension by a factor of 30
//     unsigned int newHeight = target.height() * 30;
//     unsigned int newWidth = target.width() * 30;
//     mosaic.resize(newWidth, newHeight);

//     //for each pixel in target, do NN search with it on rgbtree ss
//     //NN search will return a pixel (closest point), which will be a key in photos map
//     //plug the key into photos map to get a string representing filepath to a thumbnail
//     //put the thumbnail onto mosaic
//     for (unsigned x = 0; x < target.width(); x++) {
//         for (unsigned y = 0; y < target.height(); y++) {

//             RGBAPixel * query = target.getPixel(x, y);
//             RGBAPixel querySub = RGBAPixel();
//             querySub.r = query->r;
//             querySub.g = query->g;
//             querySub.b = query->b;

//             RGBAPixel closest = ss.findNearestNeighbor(querySub);
//             string filePath = photos[closest];
//             PNG thumbnail; thumbnail.readFromFile(filePath);

//             //PNG thumbnail; thumbnail.readFromFile("imlib/99359743_aa94427a3f_s.png");

//             cout << "rendering" <<endl;
//             render(30*x, 30*y, mosaic, thumbnail);
                  
//         }
//     }
//     return mosaic;
// }

// void tiler::render(int xPos, int yPos, PNG & mosaic, PNG & thumbnailTester)
// {
//     for (unsigned i = 0; i < thumbnailTester.width(); i++) {
//         for (unsigned j = 0; j < thumbnailTester.height(); j++) {

//             RGBAPixel * thumbnailTemp = thumbnailTester.getPixel(i, j);
//             RGBAPixel * mosaicTemp = mosaic.getPixel(xPos+i, yPos+j);
//             mosaicTemp->r = thumbnailTemp->r;
//             mosaicTemp->g = thumbnailTemp->g;
//             mosaicTemp->b = thumbnailTemp->b;

//         }
//     }
// }

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
// map<RGBAPixel, string> tiler::buildMap(string path) 
// {

//     map <RGBAPixel, string> thumbs;

//     for (const auto & entry : fs::directory_iterator(path)) 
    
//     {
//     PNG curr; curr.readFromFile(entry.path());

//     // find average color by adding up RGB values of all the pixels, then dividing by # pixels
//     // using PNG curr, iterate through every single pixel, adding up the r, g, b channels succesivvely, then divide the sums by # of pixels
//     // afterwards, construct an RGBAPixel with those averaged values
//     // map the newly constructed average RGBAPixel with its corresponding picture, the value being the filePath

//     // one image corresponds to one <K,V> pair in the result map, namely <RGBAPixel,String>

//         int currHeight = curr.height();
//         int currWidth  = curr.width(); 
//         int currArea   = currHeight * currWidth; //# pixels in an image 

//         int sumR = 0;
//         int sumG = 0;
//         int sumB = 0; 

//         for (unsigned x = 0; x < curr.width(); x++) {
//             for (unsigned y = 0; y < curr.height(); y++) {
                
//                 RGBAPixel *pixel = curr.getPixel(x, y);
//                 sumR = sumR + pixel->r;
//                 sumG = sumG + pixel->g;
//                 sumB = sumB + pixel->b;
        
//             }
//         }
    
//         //calculate the average R, G, B values 
//         int averageR = sumR/currArea;
//         int averageG = sumG/currArea;
//         int averageB = sumB/currArea;
        
//         //construct the new RGBAPixel object representing the average color of the image
//         RGBAPixel averagePixel = RGBAPixel(averageR, averageG, averageB, 255);
//         //RGBAPixel * averagePixel = new RGBAPixel(averageR, averageG, averageB, 255);

//         //update our map with key (averagePixel) and corresponding value (file path)
//         thumbs[averagePixel] = entry.path();
        
//     }
//     return thumbs;
// }


