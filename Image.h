#include <vector>
#include <cmath>
#include <string>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


struct Color {
    float r,g,b;
    Color();
    Color(float r, float g, float b);
    ~Color();
};

class Image 
{
public:
    Image();
    Image(int width, int height);
    Image(const Image& other) noexcept;
    Image &operator=(const Image &other) noexcept;
    ~Image();

    Color GetColor(int x, int y) const;
    void SetColor(const Color& color, int x, int y);

    void Read(const std::string path);
    void Export(const std::string path) const;

    void FlipV();
    void FlipH();
    void InvertColors();
    float** CreateGaussianKernel(int size, float sigma);
    void GaussianBlur(int kernelSize, float sigma);
private:
    int m_width;
    int m_height;
    Color* m_colors;
};