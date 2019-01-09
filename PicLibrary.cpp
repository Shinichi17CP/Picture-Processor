#include "PicLibrary.hpp"
#include "Colour.hpp"

// Command-line interpreter routines
void PicLibrary::print_picturestore() {
  for (auto elem = library.begin(); elem != library.end(); elem++)
    cout << elem->first << endl;
}

void PicLibrary::loadpicture(string path, string filename) {
  Picture picture(path);
  if (library.find(filename) != library.end())
    cerr << "ERROR: Picture " << filename << " already exists in the library"
    << endl;
  else if (!picture.getimage().data)
      cerr << "ERROR: Loading picture \"" << filename << "\" unsuccessful."
      << endl;
  else if ((library.insert(pair<string, Picture>(filename, picture))).second)
    cout << "Image " << filename << " loaded successfully" << endl;
  else
    cerr << "ERROR: Loading picture \"" << filename << "\" into library "
                                                       "unsuccessful." << endl;
}

void PicLibrary::unloadpicture(string filename) {
  if (library.erase(filename) == 0)
    cerr << "ERROR: Picture \"" << filename << "\" not found in library."
    << endl;
}

void PicLibrary::savepicture(string filename, string path) {
  if (library.find(filename) == library.end()) {
    cerr << "ERROR: Picture " << filename << " could not be saved. Picture "
                                             "does not exist in library" << endl;
    return;
  }
  auto picture = library.find(filename)->second;
  utils.saveimage(picture.getimage(), path);
  cout << "Image " << filename << " saved successfully in " << path << endl;
}

void PicLibrary::display(string filename) {
  auto pic_pair = library.find(filename);
  if (pic_pair == library.end()) {
    cerr << "ERROR: Display picture \"" << filename << "\" not found in "
                                                       "library." << endl;
    return;
  }
  Picture picture = pic_pair->second;
  utils.displayimage(picture.getimage());
}

// Picture transformation routines
void PicLibrary::invert(string filename) {
  if (library.find(filename) == library.end())
    return;
  Picture picture = library.find(filename)->second;
  for (int i = 0; i < picture.getwidth(); i++) {
    for (int j = 0; j < picture.getheight(); j++) {
      Colour pixel = picture.getpixel(i, j);
      pixel.setred(MAX_RGB - pixel.getred());
      pixel.setgreen(MAX_RGB - pixel.getgreen());
      pixel.setblue(MAX_RGB - pixel.getblue());
      picture.setpixel(i, j, pixel);
    }
  }
}

void PicLibrary::grayscale(string filename) {
  if (library.find(filename) == library.end())
    return;
  Picture picture = library.find(filename)->second;
  int red, green, blue;
  
  for (int i = 0; i < picture.getwidth(); i++) {
    for (int j = 0; j < picture.getheight(); j++) {
      Colour pixel = picture.getpixel(i, j);
      red = pixel.getred();
      green = pixel.getgreen();
      blue = pixel.getblue();
      int avg = (red + green + blue) / 3;

      pixel.setred(avg);
      pixel.setgreen(avg);
      pixel.setblue(avg);

      picture.setpixel(i, j, pixel);

    }
  }
}

void PicLibrary::rotate(int angle, string filename) {
  if (library.find(filename) == library.end())
    return;
  Picture picture = library.find(filename)->second;
  if (!(angle == 90 || angle == 180 || angle == 270))
        return;

  if (angle == 180) {
    Picture result(picture.getwidth(), picture.getheight());
    for (int i = 0; i < picture.getwidth(); i++) {
      for (int j = 0; j < picture.getheight(); j++) {
        Colour pixel = picture.getpixel(i, j);
        result.setpixel(picture.getwidth() - 1 - i, picture.getheight() - 1 -
        j, pixel);
      }
    }
    library[filename] = result;

  } else {
    Picture result(picture.getheight(), picture.getwidth());
    if (angle == 90) {
      for (int i = 0; i < picture.getwidth(); i++) {
        for (int j = 0; j < picture.getheight(); j++) {
          Colour pixel = picture.getpixel(i, j);
          result.setpixel(picture.getheight() - 1 - j, i, pixel);
        }
      }

    } else {
      for (int i = 0; i < picture.getwidth(); i++) {
        for (int j = 0; j < picture.getheight(); j++) {
          Colour pixel = picture.getpixel(i, j);
          result.setpixel(j, picture.getwidth() - 1 - i, pixel);
        }
      }
    }
    library[filename] = result;
  }
}

void PicLibrary::flipVH(char plane, string filename) {
  if (library.find(filename) == library.end())
    return;
  Picture picture = library.find(filename)->second;
  if (plane == HORIZONTAL) {
    for (int i = 0; i < picture.getwidth() / 2; i++) {
      for (int j = 0; j < picture.getheight(); j++) {
        Colour pixel = picture.getpixel(i, j);
        Colour opposite = picture.getpixel(picture.getwidth() - 1 - i, j);
        picture.setpixel(i, j, opposite);
        picture.setpixel(picture.getwidth() - 1 - i, j, pixel);
      }
    }
    
  } else if (plane == VERTICAL){
    for (int i = 0; i < picture.getwidth(); i++) {
      for (int j = 0; j < picture.getheight() / 2; j++) {
        Colour pixel = picture.getpixel(i, j);
        Colour opposite = picture.getpixel(i, picture.getheight() - 1 - j);
        picture.setpixel(i, j, opposite);
        picture.setpixel(i, picture.getheight() - 1 - j, pixel);
      }
    }
  }
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///////////// BLUR SEQUENTIAL TRANSFORMATION /////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void PicLibrary::blursequential(string filename) {
  if (library.find(filename) == library.end())
    return;
  Picture picture = library.find(filename)->second;
  Picture result(picture.getwidth(), picture.getheight());
  int red, green, blue;
  
  for (int i = 1; i < picture.getwidth() - 1; i++) {
    for (int j = 1; j < picture.getheight() - 1; j++) {
      red = green = blue = 0;

      for (int x = i - 1; x < i + BLUR_NEIGHBOURS; x++) {
        for (int y = j - 1; y < j + BLUR_NEIGHBOURS; y++) {
          Colour pixel = picture.getpixel(x, y);
          red += pixel.getred();
          green += pixel.getgreen();
          blue += pixel.getblue();
        }
      }

      Colour blurPixel(red / TOTAL_NEIGHBOURS, green / TOTAL_NEIGHBOURS, blue
      / TOTAL_NEIGHBOURS);
      result.setpixel(i, j, blurPixel);
    }
  }
  for (int k = 0; k < picture.getwidth(); k++) {
    result.setpixel(k, 0, picture.getpixel(k, 0));
    result.setpixel(k, picture.getheight() - 1, picture.getpixel(k, picture
    .getheight() - 1));
  }
  for (int k = 0; k < picture.getheight(); k++) {
    result.setpixel(0, k, picture.getpixel(0, k));
    result.setpixel(picture.getwidth() - 1, k, picture.getpixel(picture
    .getwidth() - 1, k));
  }
  library[filename] = result;
}

//////////////////////////////////////////////////////////////
//////////////// BLUR BORDERS FOR THREADS ////////////////////
//////////////////////////////////////////////////////////////
void PicLibrary::blurbordersthreads(Picture picture, Picture& result) {
  for (int k = 0; k < picture.getwidth(); k++) {
    resultlock.lock();
    result.setpixel(k, 0, picture.getpixel(k, 0));
    result.setpixel(k, picture.getheight() - 1, picture.getpixel(k, picture
    .getheight() - 1));
    resultlock.unlock();
  }
  for (int k = 0; k < picture.getheight(); k++) {
    resultlock.lock();
    result.setpixel(0, k, picture.getpixel(0, k));
    result.setpixel(picture.getwidth() - 1, k, picture.getpixel(picture
    .getwidth() - 1, k));
    resultlock.unlock();
  }
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///////////// BLUR INDIVIDUAL TRANSFORMATION /////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void PicLibrary::blurindividualaux(Picture picture, Picture& result, int i,
        int j) {
  int red, green, blue;
  red = green = blue = 0;
  
  for (int x = i - 1; x < i + BLUR_NEIGHBOURS; x++) {
    for (int y = j - 1; y < j + BLUR_NEIGHBOURS; y++) {
      Colour pixel = picture.getpixel(x, y);
      red += pixel.getred();
      green += pixel.getgreen();
      blue += pixel.getblue();
    }
  }
  
  Colour blurPixel(red / TOTAL_NEIGHBOURS, green / TOTAL_NEIGHBOURS, blue /
  TOTAL_NEIGHBOURS);
  resultlock.lock();
  result.setpixel(i, j, blurPixel);
  resultlock.unlock();
}

void PicLibrary::blurindividual(string filename) {
  if (library.find(filename) == library.end())
    return;
  Picture picture = library.find(filename)->second;
  Picture result(picture.getwidth(), picture.getheight());
  vector<thread> threadlist;
  
  thread borders(&PicLibrary::blurbordersthreads, this, picture, ref(result));
  for (int i = 1; i < picture.getwidth() - 1; i++) {
    for (int j = 1; j < picture.getheight() - 1; j ++) {
      threadlist.push_back(thread(&PicLibrary::blurindividualaux, this,
              picture, ref(result), i, j));
      
      if (threadlist.size() >= NUM_THREADS) {
        for (int i = 0; i < threadlist.size(); i++) {
          threadlist.at(i).join();
        }
        threadlist.clear();
      }
    }
  }
  borders.join();
  for (int i = 0; i < threadlist.size(); i++) {
    threadlist.at(i).join();
  }
  library[filename] = result;
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
///////////// BLUR ROW BY ROW TRANSFORMATION /////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void PicLibrary::blurrow(Picture picture, Picture& result, int start) {
  int red, green, blue;
  for (int j = 1 + start; j < picture.getheight() - 1; j += NUM_THREADS) {
    for (int i = 1; i < picture.getwidth() - 1; i++) {
      red = green = blue = 0;
      
      for (int x = i - 1; x < i + BLUR_NEIGHBOURS; x++) {
        for (int y = j - 1; y < j + BLUR_NEIGHBOURS; y++) {
          Colour pixel = picture.getpixel(x, y);
          red += pixel.getred();
          green += pixel.getgreen();
          blue += pixel.getblue();
        }
      }
      
      Colour blurPixel(red / TOTAL_NEIGHBOURS, green / TOTAL_NEIGHBOURS, blue
      / TOTAL_NEIGHBOURS);
      resultlock.lock();
      result.setpixel(i, j, blurPixel);
      resultlock.unlock();
    }
  }
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
////////// BLUR COLUMN BY COLUMN TRANSFORMATION //////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void PicLibrary::blurcolumn(Picture picture, Picture& result, int start) {
  int red, green, blue;

  for (int i = 1 + start; i < picture.getwidth() - 1; i += NUM_THREADS) {
    for (int j = 1; j < picture.getheight() - 1; j++) {
      red = green = blue = 0;

      for (int x = i - 1; x < i + BLUR_NEIGHBOURS; x++) {
        for (int y = j - 1; y < j + BLUR_NEIGHBOURS; y++) {
          Colour pixel = picture.getpixel(x, y);
          red += pixel.getred();
          green += pixel.getgreen();
          blue += pixel.getblue();
        }
      }

      Colour blurPixel(red / TOTAL_NEIGHBOURS, green / TOTAL_NEIGHBOURS, blue
      / TOTAL_NEIGHBOURS);
      resultlock.lock();
      result.setpixel(i, j, blurPixel);
      resultlock.unlock();
    }
  }
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////// BLUR TILING TRANSFORMATION //////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////

void PicLibrary::blurtiling(Picture picture, Picture& result, int start) {
  int red, green, blue;
  
  for (int ii = 1 + start * TILING_SIZE; ii < picture.getwidth() - 1; ii +=
          NUM_THREADS * TILING_SIZE) {
    for (int j = 1 + start; j < picture.getheight() - 1; j++) {
      for (int i = ii; i < min(ii + TILING_SIZE, picture.getwidth() - 1); i++) {
        red = green = blue = 0;
        
        for (int x = i - 1; x < i + BLUR_NEIGHBOURS; x++) {
          for (int y = j - 1; y < j + BLUR_NEIGHBOURS; y++) {
            Colour pixel = picture.getpixel(x, y);
            red += pixel.getred();
            green += pixel.getgreen();
            blue += pixel.getblue();
          }
        }
        
        Colour blurPixel(red / TOTAL_NEIGHBOURS, green / TOTAL_NEIGHBOURS,
                blue / TOTAL_NEIGHBOURS);
        resultlock.lock();
        result.setpixel(i, j, blurPixel);
        resultlock.unlock();
      }
    }
  }
}

// To apply other concurrent blur transformations, replace blurrow with the
// corresponding function
void PicLibrary::blur(string filename) {
  if (library.find(filename) == library.end())
    return;
  Picture picture = library.find(filename)->second;
  Picture result(picture.getwidth(), picture.getheight());
  vector<thread> threadlist;
  
  thread borders(&PicLibrary::blurbordersthreads, this, picture, ref(result));
  for (int i = 0; i < NUM_THREADS; i++) {
    threadlist.push_back(thread(&PicLibrary::blurrow, this, picture, ref
    (result), i));
  }
  borders.join();
  for (int i = 0; i < NUM_THREADS; i++) {
    threadlist.at(i).join();
  }
  library[filename] = result;
}