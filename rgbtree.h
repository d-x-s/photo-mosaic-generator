/**
 *
 * RGBTree for pa3
 *
 */

#ifndef _RGBTREE_H_
#define _RGBTREE_H_

#include <utility>
#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include <vector>
#include <map>
using namespace std;
using namespace cs221util;

/**
 * RGB Tree: A kd tree is a generalization of a binary search tree
 * for keys of multiple dimensions. Since our keys are RGBAPixels,
 * we will refer to our kd tree as an RGBTree. We typically consider 
 * a kd tree to be a spatial structure whose keys are k-dimensional 
 * points in space. Its core functionality is to allow for nearest 
 * neighbor search within the space. That is, we don't necessarily 
 * search for particular keys, but rather, we search for a key that is 
 * "close" to the query key.
 * 
 * You should not remove anything from this class definition, but
 * you will find it helpful to add your own private helper functions 
 * to it.
 *
 * This file will be submitted for grading, so you are welcome to add 
 * helper functions and auxiliary data.
 */

class rgbtree {

//private:
public:

    vector<RGBAPixel> tree; //vector representing the KDTree
    int rootOfTree; 

public:

  /* =============== public PA3 FUNCTIONS =========================*/

  /**
   * Constructor that builds a rgbtree out of a given 
   * map<rgbapixel,string>.
   * 
    * KDTree structure:
    * We represent the tree as a nearly perfect binary tree, whose "nodes"
    * are the entries of an array. Given n keys, the root is in array
    * position n/2, root->left is in approx position n/4, root->right is 
    * in approx position 3n/4, etc. (Integer division is your friend.)
    *
    * KDTree order:
    * A KDtree is a generalization of a binary search tree applied to 
    * k dimensional data (a bst is a 1d tree). For each level of the
    * tree we designate a splitting dimension, and for each node on
    * that level, we partition the keys in that node's subtree about the 
    * median value in the splitting dimension -- smaller keys to the left, 
    * and larger keys to the right. The splitting dimension is chosen by 
    * rotating through the k dimensions: in general, the splitting 
    * dimension is "level mod k". 
    *
    * RGBTree:
    * Our kdtree applies specifically to the RGB color space, and as such,
    * it's a 3d tree. We arbitrarily designate ['r', 'g', 'b'] as the order
    * in which we cycle through the dimensions. This means that at level 0, 
    * the root is the median red value, and on the left, all points have  
    * red <= that median, and on the right all points have red >= that median.
    * At the next level, the root's left subtree will consist of approximately 
    * n/2 points, and we partition those according to the green dimension, and
    * similarly for the right subtree.
    *
    * Median finding:
    * If someone magically handed you the median at each level of the tree, 
    * your work would be done. "quickSelect(k)" is an algorithm 
    * based on the partition function of quickSort that returns the kth largest element
    * in a collection (we want the median): Choose an arbitrary element to be 
    * the pivot, and complete the partition. If the pivot turned out to be the 
    * median, then stop and celebrate. If it wasn't, then you have at least 
    * narrowed the search space for the median, because it must be in the larger 
    * of the two partitions.  Use quickSelect, recursively, on the appropriate 
    * segment of the array, searching for the median element. The precise
    * parameter to use for the recursive call is for you to figure out
    * when you complete your implementation.
    */

    rgbtree( const map< RGBAPixel, string> & photos);
    
    /**
     * Finds the closest point to the parameter (query) point in the RGBTree.
     *
     * This function takes a reference to a RGBAPixel and
     * returns the  RGBAPixel closest to it in the tree. We are defining
     * closest here to be the minimum Euclidean distance between pixels.
     * If there are ties (this time in distance), they must be
     * decided using RGBAPixel::operator<().
     *
    * You will almost certainly want to create a recursive helper function
    * for which this public interface is merely a wrapper.
    * 
    * findNearestNeighbor(queryPixel) works as follows:
    * Throughout the computation, we maintain the nearest known neighbor, and
    * its distance from the query pixel. At every node, the nearest known 
    * neighbor is the best of A) the nearest known neighbor in the subtree 
    * containing the queryPixel, B) the pixel at the node, and C) the nearest
    * neighbor in the other subtree *if it should be searched*. (We hope we 
    * don't have to pursue C.)  
    * 
    * Pruning the search:
    * We can argue that a subtree need not be searched if all of its points
    * must be farther from the queryPixel than the nearest-known-neighbor.
    * To evaluate this, we note that the current node partitions space in one
    * dimension and one of those partitions *doesn't* contain the queryPoint. If the 
    * queryPoint is farther than the current-best-distance from the root *in 
    * only that dimension*, then it is farther than the current-best-distance 
    * from all points in that subtree, and the subtree need not be searched.
    * 
    * We leave the design decision on how to handle empty subtrees to you!
     *
     * @see Here is a reference we found quite useful in writing our kd-tree:
     *  [Andrew Moore's KD-Tree Tutorial]
     * (https://www.ri.cmu.edu/pub_files/pub1/moore_andrew_1991_1/moore_andrew_1991_1.pdf)
     *
     * @param query The point we wish to find the closest neighbor to in the
     *  tree.
     * @return The closest point to query in the rgbtree
     */
    RGBAPixel findNearestNeighbor(const RGBAPixel & query) const;

  /* =============== end of public PA3 FUNCTIONS =========================*/

    // Errata fix: changing private to public 
    public:
  /* =============== private PA3 FUNCTIONS =========================*/
  /* most of the files in this section will be helper functions for */
  /* the rgbtree constructor, and for the nearest neighbor function.*/
  /* You will want to add your own helper functions, too, especially*/
  /* some recursive functions that drive the constructor and the    */
  /* nearest neighbor function.                                     */


    /**
     * smallerByDim
     *
     * Determines if pixel a is smaller than pixel b in a given dimension d.
     * If there is a tie, break it with RGBAPixel::operator<().
     *
     * For example:
     *
     *     RGBAPixel a(1, 2, 3);
     *     RGBAPixel b(3, 2, 1);
     *     cout << smallerByDim(a, b, 0) << endl; // should print true
     *     cout << smallerByDim(a, b, 2) << endl; // should print false
     *     cout << smallerByDim(a, b, 1) << endl; // based on operator<, this should be true
     *
     * @return A boolean value indicating whether the first RGBAPixel is smaller
     *  than the second RGBAPixel in the curDim dimension.
     */
    bool smallerByDim(const RGBAPixel& first, const RGBAPixel& second,
                       int curDim) const;


    /** Helper function to do quick select. 
    * rearranges the member variable tree[start..end] as follows:
    * tree[start..start+k-1], tree[start+k], tree[start+k+1..end], so that, with 
    * respect to dimension d, everything in tree[start..start+k-1] is less
    * than or equal to tree[start + k], and everything in tree[start+k+1..end]
    * is greater than or equal to tree[start + k].
    * 
    * The algorithm for accomplishing this "semi-sort" of the data involves 
    * repeated application of the partition function, as described in the kdtree
    * constructor comments, above.
    */
    void quickSelect(int start, int end, int k, int d);

    /** Helper function to do partition for quick select. */
    /* You should adapt the code you were given in HW2 for this one */
    int partition(int lo, int hi, int d);


    /* distToSplit 
     *  This function just returns the distance between the query point, and the
     *  splitting plane through point curr, in the given dimension.
     */
    int distToSplit(const RGBAPixel& query, const RGBAPixel& curr, int dimension ) const;

    ////////////////////////////////////////////////////////////////////////////// my own helpers

    void buildTree(int start, int end, int median, int dimension);

    //RGBAPixel findNearestNeighbor_RecursiveHelper(const RGBAPixel & query, int start, int end, int dimension, int bestDistance, RGBAPixel closest) const;
    //void fNN_recursive(const RGBAPixel & query, int start, int end, int dimension, const RGBAPixel & closest) const;
    // void fNN_recursive(const RGBAPixel & query, int start, int end, int dimension, RGBAPixel & closest) const;
    RGBAPixel fNN_recursive(const RGBAPixel & query, int start, int end, int dimension, RGBAPixel & bestPixel, int bestDistance) const;


    int distanceBetweenPointsInKDimensions(const RGBAPixel & first, const RGBAPixel & second) const;

    int bestOf(const RGBAPixel & temp, const RGBAPixel & curr, const RGBAPixel & query) const;

    int distance3D(const RGBAPixel & first, const RGBAPixel & second) const;

};
#endif




// /**
//  *
//  * RGBTree for pa3
//  *
//  */

// #ifndef _RGBTREE_H_
// #define _RGBTREE_H_

// #include <utility>
// #include "cs221util/PNG.h"
// #include "cs221util/RGBAPixel.h"
// #include <vector>
// #include <map>
// using namespace std;
// using namespace cs221util;

// /**
//  * RGB Tree: A kd tree is a generalization of a binary search tree
//  * for keys of multiple dimensions. Since our keys are RGBAPixels,
//  * we will refer to our kd tree as an RGBTree. We typically consider 
//  * a kd tree to be a spatial structure whose keys are k-dimensional 
//  * points in space. Its core functionality is to allow for nearest 
//  * neighbor search within the space. That is, we don't necessarily 
//  * search for particular keys, but rather, we search for a key that is 
//  * "close" to the query key.
//  * 
//  * You should not remove anything from this class definition, but
//  * you will find it helpful to add your own private helper functions 
//  * to it.
//  *
//  * This file will be submitted for grading, so you are welcome to add 
//  * helper functions and auxiliary data.
//  */

// class rgbtree {

// //private:
// public:

//     vector<RGBAPixel> tree; //vector representing the KDTree
//     int rootOfTree; 

// public:

//   /* =============== public PA3 FUNCTIONS =========================*/

//   /**
//    * Constructor that builds a rgbtree out of a given 
//    * map<rgbapixel,string>.
//    * 
//     * KDTree structure:
//     * We represent the tree as a nearly perfect binary tree, whose "nodes"
//     * are the entries of an array. Given n keys, the root is in array
//     * position n/2, root->left is in approx position n/4, root->right is 
//     * in approx position 3n/4, etc. (Integer division is your friend.)
//     *
//     * KDTree order:
//     * A KDtree is a generalization of a binary search tree applied to 
//     * k dimensional data (a bst is a 1d tree). For each level of the
//     * tree we designate a splitting dimension, and for each node on
//     * that level, we partition the keys in that node's subtree about the 
//     * median value in the splitting dimension -- smaller keys to the left, 
//     * and larger keys to the right. The splitting dimension is chosen by 
//     * rotating through the k dimensions: in general, the splitting 
//     * dimension is "level mod k". 
//     *
//     * RGBTree:
//     * Our kdtree applies specifically to the RGB color space, and as such,
//     * it's a 3d tree. We arbitrarily designate ['r', 'g', 'b'] as the order
//     * in which we cycle through the dimensions. This means that at level 0, 
//     * the root is the median red value, and on the left, all points have  
//     * red <= that median, and on the right all points have red >= that median.
//     * At the next level, the root's left subtree will consist of approximately 
//     * n/2 points, and we partition those according to the green dimension, and
//     * similarly for the right subtree.
//     *
//     * Median finding:
//     * If someone magically handed you the median at each level of the tree, 
//     * your work would be done. "quickSelect(k)" is an algorithm 
//     * based on the partition function of quickSort that returns the kth largest element
//     * in a collection (we want the median): Choose an arbitrary element to be 
//     * the pivot, and complete the partition. If the pivot turned out to be the 
//     * median, then stop and celebrate. If it wasn't, then you have at least 
//     * narrowed the search space for the median, because it must be in the larger 
//     * of the two partitions.  Use quickSelect, recursively, on the appropriate 
//     * segment of the array, searching for the median element. The precise
//     * parameter to use for the recursive call is for you to figure out
//     * when you complete your implementation.
//     */

//     rgbtree( const map< RGBAPixel, string> & photos);
    
//     /**
//      * Finds the closest point to the parameter (query) point in the RGBTree.
//      *
//      * This function takes a reference to a RGBAPixel and
//      * returns the  RGBAPixel closest to it in the tree. We are defining
//      * closest here to be the minimum Euclidean distance between pixels.
//      * If there are ties (this time in distance), they must be
//      * decided using RGBAPixel::operator<().
//      *
//     * You will almost certainly want to create a recursive helper function
//     * for which this public interface is merely a wrapper.
//     * 
//     * findNearestNeighbor(queryPixel) works as follows:
//     * Throughout the computation, we maintain the nearest known neighbor, and
//     * its distance from the query pixel. At every node, the nearest known 
//     * neighbor is the best of A) the nearest known neighbor in the subtree 
//     * containing the queryPixel, B) the pixel at the node, and C) the nearest
//     * neighbor in the other subtree *if it should be searched*. (We hope we 
//     * don't have to pursue C.)  
//     * 
//     * Pruning the search:
//     * We can argue that a subtree need not be searched if all of its points
//     * must be farther from the queryPixel than the nearest-known-neighbor.
//     * To evaluate this, we note that the current node partitions space in one
//     * dimension and one of those partitions *doesn't* contain the queryPoint. If the 
//     * queryPoint is farther than the current-best-distance from the root *in 
//     * only that dimension*, then it is farther than the current-best-distance 
//     * from all points in that subtree, and the subtree need not be searched.
//     * 
//     * We leave the design decision on how to handle empty subtrees to you!
//      *
//      * @see Here is a reference we found quite useful in writing our kd-tree:
//      *  [Andrew Moore's KD-Tree Tutorial]
//      * (https://www.ri.cmu.edu/pub_files/pub1/moore_andrew_1991_1/moore_andrew_1991_1.pdf)
//      *
//      * @param query The point we wish to find the closest neighbor to in the
//      *  tree.
//      * @return The closest point to query in the rgbtree
//      */
//     RGBAPixel findNearestNeighbor(const RGBAPixel & query) const;

//   /* =============== end of public PA3 FUNCTIONS =========================*/

//     // Errata fix: changing private to public 
//     public:
//   /* =============== private PA3 FUNCTIONS =========================*/
//   /* most of the files in this section will be helper functions for */
//   /* the rgbtree constructor, and for the nearest neighbor function.*/
//   /* You will want to add your own helper functions, too, especially*/
//   /* some recursive functions that drive the constructor and the    */
//   /* nearest neighbor function.                                     */


//     /**
//      * smallerByDim
//      *
//      * Determines if pixel a is smaller than pixel b in a given dimension d.
//      * If there is a tie, break it with RGBAPixel::operator<().
//      *
//      * For example:
//      *
//      *     RGBAPixel a(1, 2, 3);
//      *     RGBAPixel b(3, 2, 1);
//      *     cout << smallerByDim(a, b, 0) << endl; // should print true
//      *     cout << smallerByDim(a, b, 2) << endl; // should print false
//      *     cout << smallerByDim(a, b, 1) << endl; // based on operator<, this should be true
//      *
//      * @return A boolean value indicating whether the first RGBAPixel is smaller
//      *  than the second RGBAPixel in the curDim dimension.
//      */
//     bool smallerByDim(const RGBAPixel& first, const RGBAPixel& second,
//                        int curDim) const;


//     /** Helper function to do quick select. 
//     * rearranges the member variable tree[start..end] as follows:
//     * tree[start..start+k-1], tree[start+k], tree[start+k+1..end], so that, with 
//     * respect to dimension d, everything in tree[start..start+k-1] is less
//     * than or equal to tree[start + k], and everything in tree[start+k+1..end]
//     * is greater than or equal to tree[start + k].
//     * 
//     * The algorithm for accomplishing this "semi-sort" of the data involves 
//     * repeated application of the partition function, as described in the kdtree
//     * constructor comments, above.
//     */
//     void quickSelect(int start, int end, int k, int d);

//     /** Helper function to do partition for quick select. */
//     /* You should adapt the code you were given in HW2 for this one */
//     int partition(int lo, int hi, int d);


//     /* distToSplit 
//      *  This function just returns the distance between the query point, and the
//      *  splitting plane through point curr, in the given dimension.
//      */
//     int distToSplit(const RGBAPixel& query, const RGBAPixel& curr, int dimension ) const;

//     ////////////////////////////////////////////////////////////////////////////// my own helpers

//     void buildTree(int start, int end, int median, int dimension);

//     //RGBAPixel findNearestNeighbor_RecursiveHelper(const RGBAPixel & query, int start, int end, int dimension, int bestDistance, RGBAPixel closest) const;
//     //void fNN_recursive(const RGBAPixel & query, int start, int end, int dimension, const RGBAPixel & closest) const;
//     // void fNN_recursive(const RGBAPixel & query, int start, int end, int dimension, RGBAPixel & closest) const;
//     void fNN_recursive(const RGBAPixel & query, int start, int end, int dimension, RGBAPixel & nearest_neighbor_so_far, int nearest_neighbor_so_far_distance) const;


//     int distanceBetweenPointsInKDimensions(const RGBAPixel & first, const RGBAPixel & second) const;

//     int bestOf(const RGBAPixel & temp, const RGBAPixel & curr, const RGBAPixel & query) const;

//     int distance3D(const RGBAPixel & first, const RGBAPixel & second) const;

// };
// #endif

