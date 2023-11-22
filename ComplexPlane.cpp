#include "ComplexPlane.h"
#include <iostream>
#include <cmath>
#include <sstream>

using namespace std;
using namespace sf;

ComplexPlane::ComplexPlane(inr pixelWidth, int pixelHeight)
{
  m_pixel_size = Vector2f(pixelWidth, pixelHeight);
  m_aspectRatio = pixelHeight / pixelWidth;
  m_plane_center = Vector2f(0, 0);
  m_plane_size = Vector2f(BASE_WIDTH, BASE_HEIGHT * m_aspectRatio);
  m_zoomCount = 0;
  m_state = State::CALCULATING;
  m_vArray = VertexArray(Points, pixelWidth * pixelHeight);
}

void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
  target.draw(m_vArray);
}

void ComplexPlane::updateRender()
{
  if (m_state == CALCULATING)
  {
    //height (y)
    for (int i = 0; i < m_pixel_size.y; i++)
      {
        //width (x)
        for (int j = 0; j < m_pixel_size.x; j++)
          {
            m_vArray[j+ i * m_pixel_size.x].position = { (float)j, (float)i };
            Vector2f coord = mapPixelToCoords(Vector2i(j, i));
            Vector2f iterations = countIterations(coord);

            Uint8 r,g,b;
            iterationsToRGB(iterations, r, g, b);
            m_vArray[j + i * m_pixel_size.x].color = {r,g,b};
            m_state = DISPLAYING;
          }
      }
  }
}

void ComplexPlane::zoomIn()
{
  m_ZoomCount++;
  float xVar = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
  float yVar = BASE_HEIGHT * (pow(BASE_ZOOM, m_zoomCount));
  m_plane_size = Vector2f(xVar, yVar);
  m_state = CALCULATING;
}

void ComplexPlane::zoomOut()
{
  m_zoomCount--;
  float xVar = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
  float yVar = BASE_HEIGHT * (pow(BASE_ZOOM, m_zoomCount));
  m_plane_size = Vector2f(xVar, yVar);
  m_state = CALCULATING;
}

void ComplexPlane::setCenter(Vector2i mousePixel)
{
  m_plane_center = mapPixelToCoords(mousePixel);
  m_state = CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
  m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(Text& text)
{
  ostringstream strmOut; 
  strmOut << "Mandelbrot Set" << endl;
  strmOut << "Center: (" << m_plane_center.x << "," << m_plane_center.y << ")" << endl;
  strmOut << "Cursor: (" << m_mouseLocation.x << "," << m_mouseLocation.y << ")" << endl;
  strmOut << "Left-Click to Zoom in" << endl;
  strmOut << "Right-click to Zoom out" << endl;

  text.setPosition(0,0);
  text.setString(strmOut.str());
}

size_t ComplexPlane::countIterations(Vector2f coord)
{
//Count # of iterations of the set for the given coordinate as specified above
  size_t iterations = 0;
  complex <float> c (coord.x, coord.y);
  complex <float> z (0.0,0.0);

  while (iterations < MAX_ITER)
    {
      z = z * z + c;
      if (abs(z) > 2.0)
      {
        return iterations;
      }
      iterations++;
    }

  return MAX_ITER;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    Uint8 colors[5][3] = 
    {
        {128, 0, 128},  // Purple / Blue
        {0, 128, 128},  // Turquoise
        {0, 128, 0},    // Green
        {255, 255, 0},  // Yellow
        {255, 0, 0}     // Red
    };

    if (count == MAX_ITER)
    {
      r = 0;
      g = 0;
      b = 0;
    }

    else
    {
      //Creates a region based on number of iterations
      size_t region = (count * 5) / maxIterations;
      
      // Calculate the color within the region by sliding the iteration count
      
      size_t regionIterations = count % (maxIterations / 5);
      size_t slidingEffect = (regionIterations * 255) / (maxIterations / 5);

      r = baseColors[region][0] + slidingEffect;
      g = baseColors[region][1] + slidingEffect;
      b = baseColors[region][2] + slidingEffect;
    }

  Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
  {
    // Calculate the normalized coordinates in the range [0, 1]
    float normalizedX = (mousePixel.x / m_pixel_size.x);
    float normalizedY = (mousePixel.y / m_pixel_size.y);

    // Calculate the coordinates in the complex plane
    float realPart = normalizedX * m_plane_size.x + (m_plane_center.x - m_plane_size.x / 2.0);
    float imaginaryPart = normalizedY * m_plane_size.y + (m_plane_center.y - m_plane_size.y / 2.0);

    return Vector2f(realPart, imaginaryPart);
  }

    



}

