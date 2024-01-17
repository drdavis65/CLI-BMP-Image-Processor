#include "Image.h"

#include <iostream>
#include <cstdlib>
#include <string>

int main(int argc, char* argv[])
{
    /* create rainbow image
    const int width = 640;
    const int height = 480;
    Image image(width, height);
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            image.SetColor(Color((float)x / (float)width, 1.0f - ((float)x / (float)width), (float)y / (float)height), x, y);
        }
    }
    image.Export("bmp_test.bmp");
    */
    if (argc < 3) 
    {
        std::cerr << "Usage: " << argv[0] << " <filename> <operation> [additional args]" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    Image image;
    image.Read(filename);

    std::string process = "";
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];
        
        if (arg == "-fv") {
            process += "_flipV";
            image.FlipV();
        } else if (arg == "-fh") {
            process += "_flipH";
            image.FlipH();
        } else if (arg == "-i") {
            process += "_invert";
            image.InvertColors();
        } else if (arg == "-g") {
            if (i + 2 >= argc) {
                std::cerr << "Gaussian blur requires kernel size and sigma value" << std::endl;
                return 1;
            }
            process += "_gaussian";
            int kernelSize = std::atoi(argv[++i]);
            float sigma = std::atof(argv[++i]);
            image.GaussianBlur(kernelSize, sigma);
        } else if (arg == "-bw") {
            process += "_greyscale";
            image.ColorToGray();
        } else if (arg == "-edge") {
            process += "_edgeDetect";
            image.LaplacianEdgeDetection();
        } else if (arg == "-stretchL") {
            if (i + 1 >= argc) {
                std::cerr << "Stretch requires start position value (decimal > 0 and < 1)" << std::endl;
                return 1;
            }
            float pos = std::atof(argv[++i]);
            process += "_stretchL";
            image.StretchH(pos, false);
        } else if (arg == "-stretchR") {
            if (i + 1 >= argc) {
                std::cerr << "Stretch requires start position value (decimal > 0 and < 1)" << std::endl;
                return 1;
            }
            float pos = std::atof(argv[++i]);
            process += "_stretchR";
            image.StretchH(pos, true);
        } else if (arg == "-stretchU") {
            if (i + 1 >= argc) {
                std::cerr << "Stretch requires start position value (decimal > 0 and < 1)" << std::endl;
                return 1;
            }
            float pos = std::atof(argv[++i]);
            process += "_stretchU";
            image.StretchV(pos, true);

        } else if (arg == "-stretchD") {
            if (i + 1 >= argc) {
                std::cerr << "Stretch requires start position value (decimal > 0 and < 1)" << std::endl;
                return 1;
            }
            float pos = std::atof(argv[++i]);
            process += "_stretchD";
            image.StretchV(pos, false);

        }
    }
    filename = filename.substr(0, filename.size()-4);
    std::string outputFilename = filename + process + ".bmp";
    image.Export(outputFilename.c_str());

    std::cout << "Output saved to " << outputFilename << std::endl;

    return 0;
    
}