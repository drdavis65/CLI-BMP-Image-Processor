#include "Image.h"

#include <iostream>
#include <fstream>

Color::Color()
    : r(0), g(0), b(0)
{
}

Color::Color(float r, float g, float b)
    : r(r), g(g), b(b)
{
}

Color::~Color()
{
}

Image::Image()
    : m_width(0), m_height(0), m_colors(nullptr)
{
}

Image::Image(int width, int height)
    : m_width(width), m_height(height)
{
    m_colors = new Color[width * height];
}

// Image copy constructor
Image::Image(const Image& other) noexcept
    : m_width(other.m_width), m_height(other.m_height), m_colors(nullptr) 
{
    if (other.m_colors) {
        m_colors = new Color[m_width * m_height];
        std::copy(other.m_colors, other.m_colors + m_width * m_height, m_colors);
    }
}

// Copy assignment operator
Image& Image::operator=(const Image& other) noexcept 
{
    if (this != &other) {
        delete[] m_colors;

        m_width = other.m_width;
        m_height = other.m_height;
        m_colors = new Color[m_width * m_height];
        std::copy(other.m_colors, other.m_colors + m_width * m_height, m_colors);
    }
    return *this;
}

// Image destructor
Image::~Image()
{
    delete[] m_colors;
}

Color Image::GetColor(int col, int row) const
{
    return m_colors[row * m_width + col];
}

void Image::SetColor(const Color& color, int col, int row)
{
    m_colors[row * m_width + col].r = color.r;
    m_colors[row * m_width + col].g = color.g;
    m_colors[row * m_width + col].b = color.b;
}

void Image::Read(const std::string path)
{
    std::ifstream f;
    f.open(path, std::ios::in | std::ios::binary);

    if(!f.is_open())
    {
        std::cout << "File could not be opened" << std::endl;
        return;
    }

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;

    unsigned char fileHeader[fileHeaderSize];
    f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

    if(fileHeader[0] != 'B' || fileHeader[1] != 'M')
    {
        std::cout << "The specified path is not a bitmap image" << std::endl;
        f.close();
        return;
    }

    unsigned char informationHeader[informationHeaderSize];
    f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);

    m_width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
    m_height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

    m_colors = new Color[m_width * m_height];

    const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);

    for(int y = 0; y < m_height; y++)
    {
        for(int x = 0; x < m_width; x++)
        {
            unsigned char color[3];
            f.read(reinterpret_cast<char*>(color), 3);
            m_colors[y * m_width + x].r = static_cast<float>(color[2]) / 255.0f;
            m_colors[y * m_width + x].g = static_cast<float>(color[1]) / 255.0f;
            m_colors[y * m_width + x].b = static_cast<float>(color[0]) / 255.0f;

        }
        f.ignore(paddingAmount);
    }
    f.close();

    std::cout << "File read" << std::endl;
}

void Image::Export(const std::string path) const
{
    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);

    if(!f.is_open()) {
        std::cout << "File could not be opened" << std::endl;
        return;
    }

    unsigned char bmpPad[3] = {0,0,0};
    const int paddingAmount = ((4 - (m_width * 3) % 4) % 4);

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;
    const int fileSize = fileHeaderSize + informationHeaderSize + m_width * m_height * 3 + paddingAmount * m_height;

    unsigned char fileHeader[fileHeaderSize];

    // File type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    // File size ... fileSize is integer (distribute over 4 bytes)
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    // Reserved 1 (Not used)
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    // Reserved 2 (Not used)
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    // Pixel data offset
    fileHeader[10] = fileHeaderSize + informationHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    unsigned char informationHeader[informationHeaderSize];

    // Header size
    informationHeader[0] = informationHeaderSize;
    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;
    // Image width ... width is integer (distribute over 4 bytes)
    informationHeader[4] = m_width;
    informationHeader[5] = m_width >> 8;
    informationHeader[6] = m_width >> 16;
    informationHeader[7] = m_width >> 24;
    // Image height ... height is integer (distribute over 4 bytes)
    informationHeader[8] = m_height;
    informationHeader[9] = m_height >> 8;
    informationHeader[10] = m_height >> 16;
    informationHeader[11] = m_height >> 24;
    // Planes
    informationHeader[12] = 1;
    informationHeader[13] = 0;
    // Bits per pixel (RGB)
    informationHeader[14] = 24;
    informationHeader[15] = 0;
    // Compression (No compression)
    informationHeader[16] = 0;
    informationHeader[17] = 0;
    informationHeader[18] = 0;
    informationHeader[19] = 0;
    // Image size (No compression)
    informationHeader[20] = 0;
    informationHeader[21] = 0;
    informationHeader[22] = 0;
    informationHeader[23] = 0;
    // X pixels per meter (Not specified)
    informationHeader[24] = 0;
    informationHeader[25] = 0;
    informationHeader[26] = 0;
    informationHeader[27] = 0;
    // Y pixels per meter (Not specified)
    informationHeader[28] = 0;
    informationHeader[29] = 0;
    informationHeader[30] = 0;
    informationHeader[31] = 0;
    // Total colors (Color palette not used)
    informationHeader[32] = 0;
    informationHeader[33] = 0;
    informationHeader[34] = 0;
    informationHeader[35] = 0;
    // Important colors (Generally ignored)
    informationHeader[36] = 0;
    informationHeader[37] = 0;
    informationHeader[38] = 0;
    informationHeader[39] = 0;

    f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    for(int y = 0; y < m_height; y++) 
    {
        for(int x = 0; x < m_width; x++)
        {
            unsigned char r = static_cast<unsigned char>(GetColor(x, y).r * 255.0f);
            unsigned char g = static_cast<unsigned char>(GetColor(x, y).g * 255.0f);
            unsigned char b = static_cast<unsigned char>(GetColor(x, y).b * 255.0f);
            
            unsigned char color[3] = {b,g,r};

            f.write(reinterpret_cast<char*>(color), 3);
            
        }
        f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
    }
    f.close();
    std::cout << "File created" << std::endl;
}

void Image::FlipV() 
{
    struct Color temp;
    for(int col = 0; col < m_width; col++)
    {
        for(int row = 0; row < m_height / 2; row++)
        {
            int oppRow = m_height - row - 1;
            Color temp = GetColor(col, row);
            SetColor(GetColor(col, oppRow), col, row);
            SetColor(temp, col, oppRow);
        }
    }
}

void Image::FlipH() 
{
    struct Color temp;
    for(int col = 0; col < m_width / 2; col++)
    {
        for(int row = 0; row < m_height; row++)
        {
            int oppCol = m_width - col - 1;
            Color temp = GetColor(col, row);
            SetColor(GetColor(oppCol, row), col, row);
            SetColor(temp, oppCol, row);
        }
    }
}

void Image::InvertColors() 
{
    for(int col = 0; col < m_width; col++)
    {
        for(int row = 0; row < m_height; row++)
        {
            Color ogColor = GetColor(col, row);
            Color invColor;
            invColor.r = 1.0f - ogColor.r;
            invColor.g = 1.0f - ogColor.g;
            invColor.b = 1.0f - ogColor.b;
            SetColor(invColor, col, row);
        }
    }
}

float** Image::CreateGaussianKernel(int size, float sigma) {
    float** kernel = new float*[size];
    for (int i = 0; i < size; ++i) {
        kernel[i] = new float[size];
    }

    float sum = 0.0;
    int mid = size / 2;

    for (int x = -mid; x <= mid; x++) {
        for (int y = -mid; y <= mid; y++) {
            float val = exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
            kernel[x + mid][y + mid] = val;
            sum += val;
        }
    }

    // Normalize the kernel
    for (int x = 0; x < size; ++x)
        for (int y = 0; y < size; ++y)
            kernel[x][y] /= sum;

    return kernel;
}


void Image::GaussianBlur(int kernelSize, float sigma) 
{
    auto kernel = CreateGaussianKernel(kernelSize, sigma);

    Image temp = *this;

    for (int x = 0; x < m_width; ++x) {
        for (int y = 0; y < m_height; ++y) {
            Color blurredColor = {0, 0, 0};

            for (int i = -kernelSize / 2; i <= kernelSize / 2; ++i) {
                for (int j = -kernelSize / 2; j <= kernelSize / 2; ++j) {
                    int sampleX = std::min(std::max(x + i, 0), m_width - 1);
                    int sampleY = std::min(std::max(y + j, 0), m_height - 1);

                    Color sampleColor = temp.GetColor(sampleX, sampleY);
                    blurredColor.r += sampleColor.r * kernel[i + kernelSize / 2][j + kernelSize / 2];
                    blurredColor.g += sampleColor.g * kernel[i + kernelSize / 2][j + kernelSize / 2];
                    blurredColor.b += sampleColor.b * kernel[i + kernelSize / 2][j + kernelSize / 2];
                }
            }

            SetColor(blurredColor, x, y);
        }
    }
    for (int i = 0; i < kernelSize; ++i) {
        delete[] kernel[i];
    }
    delete[] kernel;
}

void Image::ColorToGray()
{
    for(int col = 0; col < m_width; col++)
    {
        for(int row = 0; row < m_height; row++)
        {
            Color ogColor = GetColor(col, row);
            float grayScale = 0.299 * ogColor.r + 0.587 * ogColor.g + 0.114 * ogColor.b;
            SetColor(Color(grayScale, grayScale, grayScale), col, row);
        }
    }
}


void Image::LaplacianEdgeDetection() {
    ColorToGray();
    // Laplacian kernel
    float laplacianKernel[3][3] = {
        { 0, -1,  0 },
        {-1,  4, -1 },
        { 0, -1,  0 }
    };

    Image tempImage = *this;

    for (int x = 1; x < m_width - 1; ++x) {
        for (int y = 1; y < m_height - 1; ++y) {
            float edgeStrength = 0.0f;

            for (int i = -1; i <= 1; ++i) {
                for (int j = -1; j <= 1; ++j) {
                    Color sampleColor = tempImage.GetColor(x + i, y + j);
                    float intensity = sampleColor.r;
                    edgeStrength += intensity * laplacianKernel[i + 1][j + 1];
                }
            }

            // Set the new color based on edge strength
            float newColorValue = std::min(std::max(edgeStrength, 0.0f), 1.0f);
            Color edgeColor = {newColorValue, newColorValue, newColorValue};
            SetColor(edgeColor, x, y);
        }
    }
}

void Image::StretchH(float i, bool l_r)
{
    if(i > 1 || i < 0) return;
    if(l_r) 
    {
        for(int col = 0; col < m_width; col++)
        {
            for(int row = 0; row < m_height; row++)
            {
                if(col > m_width * i) {
                    SetColor(GetColor(m_width * i,row), col, row);
                }
            }
        }
    }
    else 
    {
        for(int col = 0; col < m_width; col++)
        {
            for(int row = 0; row < m_height; row++)
            {
                if(col < m_width * i) {
                    SetColor(GetColor(m_width * i,row), col, row);
                }
            }
        }
    }
    
    
}

void Image::StretchV(float i, bool u_d)
{
    if(i > 1 || i < 0) return;
    if(u_d) 
    {
        for(int col = 0; col < m_width; col++)
        {
            for(int row = 0; row < m_height; row++)
            {
                if(row > m_height * (1-i)) {
                    SetColor(GetColor(col, m_height * (1-i)), col, row);
                }
            }
        }
    }
    else 
    {
        for(int col = 0; col < m_width; col++)
        {
            for(int row = 0; row < m_height; row++)
            {
                if(row < m_height * i) {
                    SetColor(GetColor(col, m_height * i), col, row);
                }
            }
        }
    }
    
    
}