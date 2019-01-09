#include <iostream>
#include <string>
#include <thread>
#include <map>

#include "Colour.hpp"
#include "Utils.hpp"
#include "Picture.hpp"
#include "PicLibrary.hpp"


using namespace std;

static PicLibrary library;

static void interpreter(string action) {
  if (action == "liststore") {
    library.print_picturestore();
    return;
  }
  
  if (action == "exit") {
    return;
  }
  
  string filename, path;
  if (action == "save") {
    cin >> filename >> path;
    library.savepicture(filename, path);
  
  } else if (action == "load") {
    cin >> path >> filename;
    library.loadpicture(path, filename);
  
  } else if (action == "unload") {
    cin >> filename;
    library.unloadpicture(filename);
  
  } else if (action == "display") {
    cin >> filename;
    library.display(filename);
  
  } else if (action == "invert") {
    cin >> filename;
    library.invert(filename);
  
  } else if (action == "grayscale") {
    cin >> filename;
    library.grayscale(filename);
  
  } else if (action == "rotate") {
    int degree;
    cin >> degree >> filename;
    library.rotate(degree, filename);
  
  } else if (action == "flip") {
    char dir;
    cin >> dir >> filename;
    library.flipVH(dir, filename);
  
  } else if (action == "blur") {
    cin >> filename;
    library.blursequential(filename);
  } else {
    cout << "Invalid command entered" << endl;
  }
}

static string getfilename(string path) {
  size_t index = path.find_last_of("/\\");
  if (index == string::npos)
    return path;
  return path.substr(index + 1);
}

static void preloadpictures(int argc, char **argv) {
  for (int i = 1; i < argc; i++) {
    library.loadpicture(argv[i], getfilename(argv[i]));
  }
}

int main(int argc, char **argv) {
  preloadpictures(argc, argv);
  
  string action;
  do {
    cin >> action;
    interpreter(action);
  } while (action != "exit" && !cin.eof());
  
  return 0;
}


