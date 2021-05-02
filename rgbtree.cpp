/**
 * @file rgbtree.cpp
 * Implementation of rgbtree class.
 */

#include <utility>
#include <algorithm>
#include "rgbtree.h"

#include <limits.h>

using namespace std;

//  1)     partition                        [ o ]
//  2)     quickSelect                      [ o ]
//  3)     buildMap                         [ o ]
//  4)     rgbtree                          [ o ]
//  5)     distToSplit,                     [ o ]
//  6)     smallerByDim                     [ o ]
//  7)     findNearestNeighbour             [ o ]
//  8)     tile                             [ o ]

////////////////////////////////////// CONSTRUCTOR

rgbtree::rgbtree(const map<RGBAPixel,string>& photos)
{
  //build the vector "tree" of RGBAPixels from the keys in map "photos"
  for (auto const& x : photos)
  {
    tree.push_back(x.first);
  }

  //setup
  int initial_start = 0;
  int initial_end = tree.size()-1;
  int initial_dimension = 0;
  int initial_median = (initial_start+initial_end)/2;

  buildTree(initial_start, initial_end, initial_median, initial_dimension);
}

void rgbtree::buildTree(int start, int end, int median, int dimension)
{
  //base case: single elements will be in order vacuosuly
  if (start >= end)
  { return; }

  //quickSelect will put the median element into place
  quickSelect(start, end, median, dimension);

  //tick to next dimension
  dimension++;
  int nextDimension = (dimension)%3;

  //do the left side of median
  int leftSubTree_Median = (start + (median-1))/2;
  buildTree(start, median-1, leftSubTree_Median, nextDimension);

  //do the right side of median
  int rightSubTree_Median = ((median+1) + end)/2;
  buildTree(median+1, end, rightSubTree_Median, nextDimension);
}

//////////////////////////////////////

////////////////////////////////////// NEAREST NEIGHBOR + HELPERS

RGBAPixel rgbtree::findNearestNeighbor(const RGBAPixel & query) const
{
  int start = 0;
  int end = tree.size()-1;
  int index_rootMin = (start+end)/2;
  int dimension = 0;
  RGBAPixel rootMin = tree[index_rootMin];

  int bestDistance = distance3D(query, rootMin);

  return fNN_recursive(query, start, end, dimension, rootMin, bestDistance);
}



RGBAPixel rgbtree::fNN_recursive(const RGBAPixel & query, int start, int end, int dimension, RGBAPixel & bestPixel, int bestDistance) const
{
  //BASE CASE:
  if (start > end) { return bestPixel; }

  //CASE 1: 
  int index_rootMin = (start+end)/2;
  RGBAPixel rootMin = tree[index_rootMin];
  int distance_root_to_query = distance3D(query, rootMin);

  //check if the distance of current node is smaller than bestDistance; if so, replace bestDistance
  if (distance_root_to_query < bestDistance)
  {
    bestPixel = rootMin;
    bestDistance = distance_root_to_query;
  }

  //CASE 2: 
  if (smallerByDim(query, rootMin, dimension)) //true: go left
  { 
    //update dimensions
    int currDimension = dimension; 
    dimension++; 
    int nextDimension = dimension%3;

    //search for inMin (left subtree)
    RGBAPixel inMin = fNN_recursive(query, start, index_rootMin-1, nextDimension, bestPixel, bestDistance);
    int distance_inMin_to_query = distance3D(query, inMin);

    if (distance_inMin_to_query < bestDistance)
    {
      bestPixel = inMin;
      bestDistance = distance_inMin_to_query;
    }

    //CASE 3:
    //when the radius crosses over the splitting plane
    int radius = distance3D(query, bestPixel);
    int plane  = distToSplit(query, rootMin, currDimension);

    if (radius >= plane) //search for outMin (right subtree)
    {
      RGBAPixel outMin = fNN_recursive(query, index_rootMin+1, end, nextDimension, bestPixel, bestDistance);
      int distance_outMin_to_query = distance3D(query, outMin);

      if (distance_outMin_to_query < bestDistance)
      {
        bestPixel = outMin;
        bestDistance = distance_outMin_to_query; 
      }
    }
    
    
  }

  else //false: go right
  { 
    //update dimensions
    int currDimension = dimension;
    dimension++;
    int nextDimension = dimension%3;

    //search for inMin (right subtree)
    RGBAPixel inMin = fNN_recursive(query, index_rootMin+1, end, nextDimension, bestPixel, bestDistance);
    int distance_inMin_to_query = distance3D(query, inMin);

    if (distance_inMin_to_query < bestDistance)
    {
      bestPixel = inMin;
      bestDistance = distance_inMin_to_query;
    }

    //CASE 3: 
    //when the radius crosses over the splitting plane
    int radius = distance3D(query, bestPixel);
    int plane = distToSplit(query, rootMin, currDimension);

    if (radius >= plane) //serach for outMin (left subtree)
    {
      RGBAPixel outMin = fNN_recursive(query, start, index_rootMin-1, nextDimension, bestPixel, bestDistance);
      int distance_outMin_to_query = distance3D(query, outMin);

      if (distance_outMin_to_query < bestDistance)
      {
        bestPixel = outMin;
        bestDistance = distance_outMin_to_query;
      }
    }
  }

  return bestPixel;
  
}

// {
//   if (tree.size() == 1) //one node tree, the root is also the nearest neighbor
//   { return; }

//   if (start > end) //empty subtree
//   {
//     return;
//   }

//   //case 1: rootMin, the root of the KDTree
//   int index_root = (start+end)/2;
//   RGBAPixel curr_root = tree[index_root];
//   int distance_query_to_root = distance3D(query, curr_root);

//   if (distance_query_to_root < nearest_neighbor_so_far_distance)
//   { 
//     int nearest_neighbor_so_far_distance = distance_query_to_root; 
//     nearest_neighbor_so_far = curr_root;
//   }


//   //case 2: inMin, the nearest neighbor of QUERY in the subtree containing QUERY
//   if (smallerByDim(query, curr_root, dimension)) 
//   //true, go left
//   {
//     int currDimension = dimension; dimension++; int nextDimension = dimension%3;
//     fNN_recursive(query, start, index_root-1, nextDimension, nearest_neighbor_so_far, nearest_neighbor_so_far_distance);
    
//     //case 3: outMin, worst case search other subtree
//     int distance_splitting_plane = distToSplit(query, curr_root, currDimension);
//     if (distance_query_to_root >= distance_splitting_plane)
//     {
//       fNN_recursive(query, index_root+1, end, nextDimension, nearest_neighbor_so_far, nearest_neighbor_so_far_distance);
//     }

//   }

//   else 
//   //false, go right
//   {
//     int currDimension = dimension; dimension++; int nextDimension = dimension%3;
//     fNN_recursive(query, index_root+1, end, nextDimension, nearest_neighbor_so_far, nearest_neighbor_so_far_distance);

//     //case 3: outMin, worst case search other subtree
//     int distance_splitting_plane = distToSplit(query, curr_root, currDimension);
//     if (distance_query_to_root >= distance_splitting_plane)
//     {
//       fNN_recursive(query, start, index_root-1, nextDimension, nearest_neighbor_so_far, nearest_neighbor_so_far_distance);
//     }
//   }
// }

// {

//   distToSplit(query, RGBAPixel(), dimension);
//   return RGBAPixel();
//   if (start >= end)
//   { 
//     return RGBAPixel(); 
//   }

//   int index_curr = (start+end)/2;
//   RGBAPixel curr = tree[index_curr];

//   if (smallerByDim(query, curr, dimension)) //true, go left
//   {
//     dimension++; int nextDimension = (dimension)%3;
//     RGBAPixel temp = fNN_recursive(query, start, index_curr-1, nextDimension);
//     RGBAPixel closest = bestOf(temp, curr, query);

//     int radiusSquared = distance3D(query, closest); 
//     int distSquared = distToSplit(query, curr, dimension);

//     if (radiusSquared >= distSquared) //if circle radius crosses over splitting plane
//     { 
//       temp = fNN_recursive(query, index_curr+1, end, nextDimension); //a candidate point returned by searching the other subtree (worst case)
//       RGBAPixel closest = bestOf(temp, closest, query); //select the closer distance to query between "temp" and "closest"
//     }
//     return closest;
//   }

//   else //false, go right
//   {
//     dimension++; int nextDimension = (dimension)%3;
//     RGBAPixel temp = fNN_recursive(query, index_curr+1, end, nextDimension);
//     RGBAPixel closest = bestOf(temp, curr, query);

//     int radiusSquared = distance3D(query, closest);
//     int distSquared = distToSplit(query, curr, dimension);

//     if (radiusSquared >= distSquared)
//     {
//       temp = fNN_recursive(query, start, index_curr-1, nextDimension);
//       RGBAPixel closest = bestOf(temp, closest, query);
//     }
//     return closest;
//   }
// }



int rgbtree::bestOf(const RGBAPixel & temp, const RGBAPixel & curr, const RGBAPixel & query) const
{
  int d1 = distance3D(temp, query);
  int d2 = distance3D(curr, query);

  if (d1 < d2)
    return d1;
  else
    return d2;
}




int rgbtree::distance3D(const RGBAPixel & first, const RGBAPixel & second) const
{
  int first_red = (int)first.r;
  int first_gre = (int)first.g;
  int first_blu = (int)first.b;

  int second_red = (int)second.r;
  int second_gre = (int)second.g;
  int second_blu = (int)second.b;

  return (first_red-second_red)*(first_red-second_red)+
         (first_gre-second_gre)*(first_gre-second_gre)+
         (first_blu-second_blu)*(first_blu-second_blu);
}


// determine if pixel a is smaller than pixel b in dimension curDim
bool rgbtree::smallerByDim(const RGBAPixel & first,
                                const RGBAPixel & second, int curDim) const
{
  if (curDim == 0)
  {
    if (first.r < second.r)
    return true;

    if (first.r == second.r)
    return (first < second);

    else {return false;}
  }

  if (curDim == 1)
  {
    if (first.g < second.g)
    return true;

    if (first.g == second.g)
    return (first < second);

    else {return false;}
  }

  if (curDim == 2)
  {
    if (first.b < second.b)
    return true;

    if (first.b == second.b)
    return (first < second);

    else {return false;}
  }
  cout << "Error in smallerByDim, this line of code should not be reached." << endl; 
}



/**
 * Helper function to help determine if the nearest neighbor could 
 * be on the other side of the KD tree.
 */
int rgbtree::distToSplit(const RGBAPixel& query, const RGBAPixel& curr, int currDim) const
{
  if (currDim == 0)
  { 
    int currentR = (int)curr.r; 
    int queryR = (int)query.r;
    return (currentR-queryR)*(currentR-queryR); 
  }

  if (currDim == 1)
  { 
    int currentG = (int)curr.g; 
    int queryG = (int)query.g;
    return (currentG-queryG)*(currentG-queryG); 
  }

  if (currDim == 2)
  { 
    int currentB = (int)curr.b;
    int queryB = (int)query.b;
    return (currentB-queryB)*(currentB-queryB); 
  }
}

//////////////////////////////////////

////////////////////////////////////// QUICKSELECT + HELPERS

/**
 * This function splits the trees[start..end] subarray at position start k
 */
void rgbtree::quickSelect(int start, int end, int k, int d)
{
  //stop. A[k] contains the kth ordered element.
  if (start >= end)
  { return; }

  int p = partition(start, end, d);

  if(k < p)
  //search left of pivot.
  { quickSelect(start, p-1, k, d); }

  if (k > p)
  //search right of pivot.
  { quickSelect(p+1, end, k, d); }

  if (k == p)
  //stop, k is now in the right place.
  { return; }
}


/**
 * This method does a partition around pivot and will be used 
 * in quick select. It uses tree[lo] as the default pivot.
 * It returns the index of the pivot in the updated vector.
 * You will likely need to modify and complete this code.
 */
int rgbtree::partition(int lo, int hi, int d) 

{ 
    if (d == 0)
    {
    RGBAPixel x = tree[hi];
    int idx = lo; 
    for (int j = lo; j <= hi - 1; j++) { 
        if (tree[j].r <= x.r) { 
            swap(tree[idx], tree[j]); 
            idx++; 
        } 
    } 
    swap(tree[idx], tree[hi]); 
    return idx;
    } 

    if (d == 1)
    {
    RGBAPixel x = tree[hi];
    int idx = lo; 
    for (int j = lo; j <= hi - 1; j++) { 
        if (tree[j].g <= x.g) { 
            swap(tree[idx], tree[j]); 
            idx++; 
        } 
    } 
    swap(tree[idx], tree[hi]); 
    return idx;
    } 

    if (d == 2)
    {
    RGBAPixel x = tree[hi];
    int idx = lo; 
    for (int j = lo; j <= hi - 1; j++) { 
        if (tree[j].b <= x.b) { 
            swap(tree[idx], tree[j]); 
            idx++; 
        } 
    } 
    swap(tree[idx], tree[hi]); 
    return idx;
    } 
}

//////////////////////////////////////

//////////////////////////////////////


// /**
//  * @file rgbtree.cpp
//  * Implementation of rgbtree class.
//  */

// #include <utility>
// #include <algorithm>
// #include "rgbtree.h"

// #include <limits.h>

// using namespace std;

// //  1)     partition                        [ o ]
// //  2)     quickSelect                      [ o ]
// //  3)     buildMap                         [ o ]
// //  4)     rgbtree                          [ o ]
// //  5)     distToSplit,                     [ o ]
// //  6)     smallerByDim                     [ o ]
// //  7)     findNearestNeighbour             []
// //  8)     tile                             [ o ]

// ////////////////////////////////////// CONSTRUCTOR

// rgbtree::rgbtree(const map<RGBAPixel,string>& photos)
// {
//   //build the vector "tree" of RGBAPixels from the keys in map "photos"
//   for (auto const& x : photos)
//   {
//     tree.push_back(x.first);
//   }

//   //setup
//   int initial_start = 0;
//   int initial_end = tree.size()-1;
//   int initial_dimension = 0;
//   int initial_median = (initial_start+initial_end)/2;

//   buildTree(initial_start, initial_end, initial_median, initial_dimension);
// }

// void rgbtree::buildTree(int start, int end, int median, int dimension)
// {
//   //base case: single elements will be in order vacuosuly
//   if (start >= end)
//   { return; }

//   //quickSelect will put the median element into place
//   quickSelect(start, end, median, dimension);

//   //tick to next dimension
//   dimension++;
//   int nextDimension = (dimension)%3;

//   //do the left side of median
//   int leftSubTree_Median = (start + (median-1))/2;
//   buildTree(start, median-1, leftSubTree_Median, nextDimension);

//   //do the right side of median
//   int rightSubTree_Median = ((median+1) + end)/2;
//   buildTree(median+1, end, rightSubTree_Median, nextDimension);
// }

// //////////////////////////////////////

// ////////////////////////////////////// NEAREST NEIGHBOR + HELPERS

// RGBAPixel rgbtree::findNearestNeighbor(const RGBAPixel & query) const
// {
//   int start = 0;
//   int end = tree.size()-1;
//   int dimension = 0;
//   int nearest_neighbor_so_far_distance = 10000000;

//   int index_root = (start+end)/2;
//   RGBAPixel getRoot = tree[index_root];
//   RGBAPixel nearest_neighbor_so_far = RGBAPixel(getRoot); //copy the root

//   fNN_recursive(query, start, end, dimension, nearest_neighbor_so_far, nearest_neighbor_so_far_distance); 

//   return nearest_neighbor_so_far;

// }



// void rgbtree::fNN_recursive(const RGBAPixel & query, int start, int end, int dimension, RGBAPixel & nearest_neighbor_so_far, int nearest_neighbor_so_far_distance) const
// {
//   if (tree.size() == 1) //one node tree, the root is also the nearest neighbor
//   { return; }

//   if (start > end) //empty subtree
//   {
//     return;
//   }

//   //case 1: rootMin, the root of the KDTree
//   int index_root = (start+end)/2;
//   RGBAPixel curr_root = tree[index_root];
//   int distance_query_to_root = distance3D(query, curr_root);

//   if (distance_query_to_root < nearest_neighbor_so_far_distance)
//   { 
//     int nearest_neighbor_so_far_distance = distance_query_to_root; 
//     RGBAPixel nearest_neighbor_so_far = curr_root;
//   }


//   //case 2: inMin, the nearest neighbor of QUERY in the subtree containing QUERY
//   if (smallerByDim(query, curr_root, dimension)) 
//   //true, go left
//   {
//     int currDimension = dimension; dimension++; int nextDimension = dimension%3;
//     fNN_recursive(query, start, index_root-1, nextDimension, nearest_neighbor_so_far, nearest_neighbor_so_far_distance);
    
//     //case 3: outMin, worst case search other subtree
//     int distance_splitting_plane = distToSplit(query, curr_root, currDimension);
//     if (distance_query_to_root >= distance_splitting_plane)
//     {
//       fNN_recursive(query, index_root+1, end, nextDimension, nearest_neighbor_so_far, nearest_neighbor_so_far_distance);
//     }

//   }

//   else 
//   //false, go right
//   {
//     int currDimension = dimension; dimension++; int nextDimension = dimension%3;
//     fNN_recursive(query, index_root+1, end, nextDimension, nearest_neighbor_so_far, nearest_neighbor_so_far_distance);

//     //case 3: outMin, worst case search other subtree
//     int distance_splitting_plane = distToSplit(query, curr_root, currDimension);
//     if (distance_query_to_root >= distance_splitting_plane)
//     {
//       fNN_recursive(query, start, index_root-1, nextDimension, nearest_neighbor_so_far, nearest_neighbor_so_far_distance);
//     }
//   }
// }

// // {

// //   distToSplit(query, RGBAPixel(), dimension);
// //   return RGBAPixel();
// //   if (start >= end)
// //   { 
// //     return RGBAPixel(); 
// //   }

// //   int index_curr = (start+end)/2;
// //   RGBAPixel curr = tree[index_curr];

// //   if (smallerByDim(query, curr, dimension)) //true, go left
// //   {
// //     dimension++; int nextDimension = (dimension)%3;
// //     RGBAPixel temp = fNN_recursive(query, start, index_curr-1, nextDimension);
// //     RGBAPixel closest = bestOf(temp, curr, query);

// //     int radiusSquared = distance3D(query, closest); 
// //     int distSquared = distToSplit(query, curr, dimension);

// //     if (radiusSquared >= distSquared) //if circle radius crosses over splitting plane
// //     { 
// //       temp = fNN_recursive(query, index_curr+1, end, nextDimension); //a candidate point returned by searching the other subtree (worst case)
// //       RGBAPixel closest = bestOf(temp, closest, query); //select the closer distance to query between "temp" and "closest"
// //     }
// //     return closest;
// //   }

// //   else //false, go right
// //   {
// //     dimension++; int nextDimension = (dimension)%3;
// //     RGBAPixel temp = fNN_recursive(query, index_curr+1, end, nextDimension);
// //     RGBAPixel closest = bestOf(temp, curr, query);

// //     int radiusSquared = distance3D(query, closest);
// //     int distSquared = distToSplit(query, curr, dimension);

// //     if (radiusSquared >= distSquared)
// //     {
// //       temp = fNN_recursive(query, start, index_curr-1, nextDimension);
// //       RGBAPixel closest = bestOf(temp, closest, query);
// //     }
// //     return closest;
// //   }
// // }



// int rgbtree::bestOf(const RGBAPixel & temp, const RGBAPixel & curr, const RGBAPixel & query) const
// {
//   int d1 = distance3D(temp, query);
//   int d2 = distance3D(curr, query);

//   if (d1 < d2)
//     return d1;
//   else
//     return d2;
// }




// int rgbtree::distance3D(const RGBAPixel & first, const RGBAPixel & second) const
// {
//   int first_red = (int)first.r;
//   int first_gre = (int)first.g;
//   int first_blu = (int)first.b;

//   int secnd_red = (int)second.r;
//   int secnd_gre = (int)second.g;
//   int secnd_blu = (int)second.b;

//   return (first_red-secnd_red)*(first_red-secnd_red)+(first_gre-secnd_gre)*(first_gre-secnd_gre)+(first_blu-secnd_blu)*(first_blu-secnd_blu);
// }


// // determine if pixel a is smaller than pixel b in dimension curDim
// bool rgbtree::smallerByDim(const RGBAPixel & first,
//                                 const RGBAPixel & second, int curDim) const
// {
//   if (curDim == 0)
//   {
//     if (first.r < second.r)
//     return true;

//     if (first.r == second.r)
//     return (first < second);

//     else {return false;}
//   }

//   if (curDim == 1)
//   {
//     if (first.g < second.g)
//     return true;

//     if (first.g == second.g)
//     return (first < second);

//     else {return false;}
//   }

//   if (curDim == 2)
//   {
//     if (first.b < second.b)
//     return true;

//     if (first.b == second.b)
//     return (first < second);

//     else {return false;}
//   }
//   cout << "Error in smallerByDim, this line of code should not be reached." << endl; 
// }



// /**
//  * Helper function to help determine if the nearest neighbor could 
//  * be on the other side of the KD tree.
//  */
// int rgbtree::distToSplit(const RGBAPixel& query, const RGBAPixel& curr, int currDim) const
// {
//   if (currDim == 0)
//   { 
//     int currentR = (int)curr.r; 
//     int queryR = (int)query.r;
//     return (currentR-queryR)*(currentR-queryR); 
//   }

//   if (currDim == 1)
//   { 
//     int currentG = (int)curr.g; 
//     int queryG = (int)query.g;
//     return (currentG-queryG)*(currentG-queryG); 
//   }

//   if (currDim == 2)
//   { 
//     int currentB = (int)curr.b;
//     int queryB = (int)query.b;
//     return (currentB-queryB)*(currentB-queryB); 
//   }
// }

// //////////////////////////////////////

// ////////////////////////////////////// QUICKSELECT + HELPERS

// /**
//  * This function splits the trees[start..end] subarray at position start k
//  */
// void rgbtree::quickSelect(int start, int end, int k, int d)
// {
//   //stop. A[k] contains the kth ordered element.
//   if (start >= end)
//   { return; }

//   int p = partition(start, end, d);

//   if(k < p)
//   //search left of pivot.
//   { quickSelect(start, p-1, k, d); }

//   if (k > p)
//   //search right of pivot.
//   { quickSelect(p+1, end, k, d); }

//   if (k == p)
//   //stop, k is now in the right place.
//   { return; }
// }


// /**
//  * This method does a partition around pivot and will be used 
//  * in quick select. It uses tree[lo] as the default pivot.
//  * It returns the index of the pivot in the updated vector.
//  * You will likely need to modify and complete this code.
//  */
// int rgbtree::partition(int lo, int hi, int d) 

// { 
//     if (d == 0)
//     {
//     RGBAPixel x = tree[hi];
//     int idx = lo; 
//     for (int j = lo; j <= hi - 1; j++) { 
//         if (tree[j].r <= x.r) { 
//             swap(tree[idx], tree[j]); 
//             idx++; 
//         } 
//     } 
//     swap(tree[idx], tree[hi]); 
//     return idx;
//     } 

//     if (d == 1)
//     {
//     RGBAPixel x = tree[hi];
//     int idx = lo; 
//     for (int j = lo; j <= hi - 1; j++) { 
//         if (tree[j].g <= x.g) { 
//             swap(tree[idx], tree[j]); 
//             idx++; 
//         } 
//     } 
//     swap(tree[idx], tree[hi]); 
//     return idx;
//     } 

//     if (d == 2)
//     {
//     RGBAPixel x = tree[hi];
//     int idx = lo; 
//     for (int j = lo; j <= hi - 1; j++) { 
//         if (tree[j].b <= x.b) { 
//             swap(tree[idx], tree[j]); 
//             idx++; 
//         } 
//     } 
//     swap(tree[idx], tree[hi]); 
//     return idx;
//     } 
// }

// //////////////////////////////////////

// //////////////////////////////////////


