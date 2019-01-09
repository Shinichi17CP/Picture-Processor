#ifndef PICLIB_H
#define PICLIB_H

#include <map>
#include <thread>
#include <mutex>

#include "Picture.hpp"
#include "Utils.hpp"

#define MAX_RGB 255
#define VERTICAL 'V'
#define HORIZONTAL 'H'
#define BLUR_NEIGHBOURS 2
#define TOTAL_NEIGHBOURS 9
#define NUM_THREADS 4
#define TILING_SIZE 8

class PicLibrary {

  private:
    Utils utils;
    map<string, Picture> library;
    mutex resultlock;
    
    void blurindividualaux(Picture picture, Picture& result, int i, int j);
    void blurbordersthreads(Picture picture, Picture& result);
    void blurrow(Picture picture, Picture& result, int start);
    void blurcolumn(Picture picture, Picture& result, int start);
    void blurtiling(Picture picture, Picture& result, int start);
    
  public:
    // defaiult constructor/deconstructor
    PicLibrary(){};
    ~PicLibrary(){};
  
    // command-line interpreter routines
    void print_picturestore();
    void loadpicture(string path, string filename);
    void unloadpicture(string filename);
    void savepicture(string filename, string path);
    void display(string filename);
    
    // picture transformation routines
    void invert(string filename);
    void grayscale(string filename);
    void rotate(int angle, string filename);
    void flipVH(char plane, string filename);
    
    // blur functions
    void blursequential(string filename);
    void blurindividual(string filename);
    void blur(string filename);
};

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////// SYNCHRONISATION PLAN ////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
/* TODO:
 * Assign each picture in the library's internal store a queue of operations
 * to be carried out using a map<string, queue<string>>. Each thread will
 * only be able to work on its assigned picture. The queue will ensure that
 * the image will undergo the transformations in the order that was given.
 * Thread is put to sleep if there's no task in the queue using a condition
 * variable. It is notified (woken up) when a new task is added.
 *
 * As for modifications to the library's internal storage (i.e. liststore,
 * load, unload), they will be done by a single thread as to preserve the
 * sequential consistency.
*/

#endif