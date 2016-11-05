//
//  main.cpp
//
//  Copyright © 2016 Tyree Stevenson. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string>
#include <cmath>
using namespace std;
class PhotoFile {
    // creates fuccitons to upload file and edit it.
protected:
    struct Pixels {
        int r,g,b;
    };
    ifstream *replacement_background_file;
    ifstream *background_file; // background file
    ifstream *infile;  // input file
    ofstream *outfile; // output file location
    int numPixels, numBackgroundPixels,width, height, background_width,background_height,replacementNumBackgroundPixels,replacement_background_width, replacement_background_height;
    string headers[4],background_headers[4],replacement_background_headers[4];
    Pixels* pixels;   // Store the pixels when constructed
    Pixels* background_image_pixels;
    Pixels* replacement_background_image_pixels;
    void apply_colorize();
    void parse_file();
    void apply_grayscale();
    void background_parse_file();
    void replacement_background_parse_file();
    void compare_pixels();
    void replace_pixels();
    void blend_pixels();
public:
    PhotoFile(string infile, string outfile);
    void grey_scale(); // edits the file in the file variable and writes to outfile
    void colorize();
    void subtract_background(string background_filename);
    void replace_background(string background_filename,string replacement_background_filename);
    void blend_photo(string other_photo_name);
};
PhotoFile::PhotoFile(string i, string o) {
    infile = new ifstream(i);
    outfile = new ofstream(o);
    parse_file();
}


void PhotoFile::parse_file() {
    for (int i = 0; i < 4; i++) {
        getline(*infile,headers[i]);
    }
    istringstream sizestream(headers[2]);
    sizestream >> width;
    sizestream >> height;
    numPixels = width * height;
    pixels = new Pixels[numPixels];
    string num1,num2,num3;
    for (int i = 0; i < numPixels; i++) {
        *infile >> num1 >> num2 >> num3;
        pixels[i].r = stoi(num1);
        pixels[i].g = stoi(num2);
        pixels[i].b = stoi(num3);
    }
    infile->close();
}
void PhotoFile::background_parse_file() {
    for (int i = 0; i < 4; i++) {
        getline(*background_file,background_headers[i]);
    }
    istringstream sizestream(background_headers[2]);
    sizestream >> background_width;
    sizestream >> background_height;
    numBackgroundPixels = background_height * background_width;
    background_image_pixels = new Pixels[numBackgroundPixels];
    string num1,num2,num3;
    for (int i = 0; i < numBackgroundPixels; i++) {
        *background_file >> num1 >> num2 >> num3;
        background_image_pixels[i].r = stoi(num1);
        background_image_pixels[i].g = stoi(num2);
        background_image_pixels[i].b = stoi(num3);
    }
    background_file->close();
}
void PhotoFile::replacement_background_parse_file() {
    for (int i = 0; i < 4; i++) {
        getline(*replacement_background_file,replacement_background_headers[i]);
    }
    istringstream sizestream(replacement_background_headers[2]);
    sizestream >> replacement_background_height;
    sizestream >> replacement_background_width;
    replacementNumBackgroundPixels = replacement_background_width * replacement_background_height;
    replacement_background_image_pixels = new Pixels[replacementNumBackgroundPixels];
    string num1,num2,num3;
    for (int i = 0; i < replacementNumBackgroundPixels; i++) {
        *replacement_background_file >> num1 >> num2 >> num3;
        replacement_background_image_pixels[i].r = stoi(num1);
        replacement_background_image_pixels[i].g = stoi(num2);
        replacement_background_image_pixels[i].b = stoi(num3);
    }
    replacement_background_file->close();
}

void PhotoFile::apply_grayscale() {
    int a1,a2,a3;
    int gray;
    for (int i = 0; i < numPixels; i++) {
        a1 = pixels[i].r;
        a2 = pixels[i].g;
        a3 = pixels[i].b;
        gray = (a1 + a2 + a3)/3;
        pixels[i].r = gray;
        pixels[i].b = gray;
        pixels[i].g = gray;
        cout << pixels[i].r << " " <<pixels[i].b << " "<< pixels[i].g << endl;
    }
}

void PhotoFile:: apply_colorize() {
    for (int i = 0; i < numPixels; i++){
            pixels[i].r *= 0.4;
            pixels[i].g *= 2.59;
            pixels[i].b *= 0.33;
        cout << pixels[i].r << " " <<pixels[i].b << " "<< pixels[i].g << endl;
    }
}

void PhotoFile:: grey_scale() {
    apply_grayscale();
    *outfile << headers[0] << endl;
    *outfile << headers[1] << endl;
    *outfile << headers[2] << endl;
    *outfile << headers[3] << endl;
    for (int i = 0; i < numPixels; i++) {
        *outfile << pixels[i].r << " " << pixels[i].g << " " << pixels[i].b << endl;
    }
    outfile->close();
}

void PhotoFile:: colorize() {
    apply_colorize();
    *outfile << headers[0] << endl;
    *outfile << headers[1] << endl;
    *outfile << headers[2] << endl;
    *outfile << headers[3] << endl;
    for (int i = 0; i < numPixels; i++) {
        *outfile << pixels[i].r << " " << pixels[i].b << " " << pixels[i].g << endl;
    }
    outfile->close();
}
void PhotoFile :: compare_pixels() {
    int amount_pixels;
    if (numPixels > numBackgroundPixels) {
        amount_pixels = numBackgroundPixels;
    }
    else {
        amount_pixels = numPixels;
    }
    for (int i = 0; i < amount_pixels; i++) {
        if ((abs(pixels[i].r - background_image_pixels[i].r) + abs(pixels[i].g - background_image_pixels[i].g) + abs(pixels[i].b - background_image_pixels[i].b)) <= 80){
            pixels[i].r = 255;
            pixels[i].g = 255;
            pixels[i].b = 255;
            *outfile << pixels[i].r << " " << pixels[i].g << " " << pixels[i].b << endl;
        }
        else {
            *outfile << pixels[i].r << " " << pixels[i].g << " " << pixels[i].b << endl;
        }
    }
}
void PhotoFile:: subtract_background(string background_filename) {
    background_file = new ifstream(background_filename);
    background_parse_file();
    *outfile << headers[0] << endl;
    *outfile << headers[1] << endl;
    *outfile << headers[2] << endl;
    *outfile << headers[3] << endl;
    compare_pixels();
}
void PhotoFile :: replace_pixels() {
    for (int i = 0; i < replacementNumBackgroundPixels; i++) {
        if ((abs(pixels[i].r - background_image_pixels[i].r) + abs(pixels[i].g - background_image_pixels[i].g) + abs(pixels[i].b - background_image_pixels[i].b)) <= 80){
            *outfile << replacement_background_image_pixels[i].r << " " << replacement_background_image_pixels[i].g << " " << replacement_background_image_pixels[i].b << endl;
        }
        else {
            *outfile << pixels[i].r << " " << pixels[i].g << " " << pixels[i].b << endl;
        }
    }
}
void PhotoFile:: replace_background(string background_filename, string replacement_background_filename) {
    background_file = new ifstream(background_filename);
    replacement_background_file = new ifstream(replacement_background_filename);
    background_parse_file();
    replacement_background_parse_file();
    *outfile << replacement_background_headers[0] << endl;
    *outfile << replacement_background_headers[1] << endl;
    *outfile << replacement_background_headers[2] << endl;
    *outfile << replacement_background_headers[3] << endl;
    replace_pixels();
}
void PhotoFile:: blend_pixels() {
    for (int i = 0; i < numPixels; i++) {
  *outfile << (pixels[i].r + background_image_pixels[i].r)/2 << " "<< (pixels[i].g + background_image_pixels[i].g)/2<< " " << (pixels[i].b + background_image_pixels[i].b)/2 << endl;
    }
}
void PhotoFile:: blend_photo(string other_photo_name) {
    background_file = new ifstream(other_photo_name);
    background_parse_file();
    *outfile << headers[0] << endl;
    *outfile << headers[1] << endl;
    *outfile << headers[2] << endl;
    *outfile << headers[3] << endl;
    blend_pixels();
}
void clear_input_buffer();
void convert_grayscale();
void convert_to_color();
void erase_background();
void change_background();
void blend_background();
void menu();
int main() {
    int decission;
    do{
        cout << "enter 1 to continue and enter 0 to quit: " << endl;
        cin >> decission;
        if (decission == 1) {
            menu();
        }
    }while(decission != 0);
    return 0;
}

void menu() {
    int decission;
    do {
        cout << "type 1 to convert photo to black and white: " << endl;
        cout << "type 2 to convert black and white to color: " << endl;
        cout << "type 3 to earse background of a photo: " << endl;
        cout << "type 4 to change background of a photo: " << endl;
        cout << "type 5 to blend two photos: " << endl;
        cin >> decission;
    } while (decission != 1 && decission != 2 && decission != 3 && decission != 4 && decission != 5);
    if (decission == 1) {
        convert_grayscale();
    }
    if (decission == 2) {
        convert_to_color();
    }
    if (decission == 3) {
        erase_background();
    }
    if (decission == 4) {
        change_background();
    }
    if (decission == 5) {
         blend_background();
    }
}
void clear_input_buffer() {
    cin.clear();
    cin.ignore();
}
void convert_grayscale() {
    clear_input_buffer();
    string file_name;
    cout << "enter name of file to be gray scaled: ";
    getline(cin, file_name);
    PhotoFile photo(file_name,"output.ppm");
    photo.grey_scale();
}
void convert_to_color() {
    clear_input_buffer();
    string file_name;
    cout << "enter name of file to be colorized: ";
    getline(cin, file_name);
    PhotoFile photo(file_name,"output.ppm");
    photo.colorize();
}
void erase_background() {
    clear_input_buffer();
    string file_name,file_name2;
    cout << "enter name of the file with foreground: ";
    getline(cin, file_name);
    cout << "enter the name of the background file: ";
    getline(cin,file_name2);
    PhotoFile photo(file_name,"output.ppm");
    photo.subtract_background(file_name2);
    photo.grey_scale();
}
void change_background() {
    clear_input_buffer();
    string file_name,file_name2,file_name3;
    cout << "enter name of the file with foreground: ";
    getline(cin, file_name);
    cout << "enter the name of the file with the  background: ";
    getline(cin,file_name2);
    cout << "enter the name of file with new background: ";
    getline(cin,file_name3);
    PhotoFile photo(file_name,"output.ppm");
    photo.replace_background(file_name2, file_name3);
}
void blend_background() {
    clear_input_buffer();
    string file_name,file_name2;
    cout << "enter name of the first file: ";
    getline(cin, file_name);
    cout << "enter the name of the second file: ";
    getline(cin,file_name2);
    PhotoFile photo(file_name,"output.ppm");
    photo.blend_photo(file_name2);
}












