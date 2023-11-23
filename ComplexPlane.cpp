#include "ComplexPlane.h"
#include <iostream>
#include <cmath>
#include <sstream>


using namespace std;
using namespace sf;

//Constructor Initializes private member variables
ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
  m_pixel_size = Vector2i(pixelWidth, pixelHeight);
  m_aspectRatio = pixelHeight / pixelWidth;
  m_plane_center = Vector2f(0, 0);
  m_plane_size = Vector2f(BASE_WIDTH, BASE_HEIGHT * m_aspectRatio);
  m_zoomCount = 0;
  m_state = State::CALCULATING;
  m_vArray = VertexArray(Points, pixelWidth * pixelHeight);
}

//Draw function actually displays Mandlebrot Set
void ComplexPlane::draw(RenderTarget& target, RenderStates states) const
{
  target.draw(m_vArray);
}

//Updates the vertex array with color and position information
void ComplexPlane::updateRender()
{
  //Checks if state is calculating
  if (m_state == CALCULATING)
  {
    //height (y)
    for (int i = 0; i < m_pixel_size.y; i++)
      {
        //width (x)
        for (int j = 0; j < m_pixel_size.x; j++)
          {
            //Sets the position of vertexes
            m_vArray[j+ i * m_pixel_size.x].position = { (float)j, (float)i };
            //Maps coords to ComplexPlane 
            Vector2f coord = mapPixelToCoords(Vector2i(j, i));
            size_t iterations = countIterations(coord);

            Uint8 r,g,b;
            //Generations color information for pixels
            iterationsToRGB(iterations, r, g, b);
            m_vArray[j + i * m_pixel_size.x].color = {r,g,b};
        
          }
      }
      //Sets the state to DISPLAYING to display new information
      m_state = DISPLAYING;
  }
}

//Zooms in
void ComplexPlane::zoomIn()
{
  //Increment zoomCount to keep track of current zoom level
  m_zoomCount++;
  //Calculates new X and Y size
  float xVar = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
  float yVar = BASE_HEIGHT * (pow(BASE_ZOOM, m_zoomCount));
  //Sets the plane to that new size
  m_plane_size = Vector2f(xVar, yVar);
  //Changes the state to update display information
  m_state = CALCULATING;
}

//Same thing as zoom in, just zooms out
void ComplexPlane::zoomOut()
{
  m_zoomCount--;
  float xVar = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
  float yVar = BASE_HEIGHT * (pow(BASE_ZOOM, m_zoomCount));
  m_plane_size = Vector2f(xVar, yVar);
  m_state = CALCULATING;
}

//Takes mouse position info and sets it equal to the center
void ComplexPlane::setCenter(Vector2i mousePixel)
{
  m_plane_center = mapPixelToCoords(mousePixel);
  //New center position requires recalculation to render set
  m_state = CALCULATING;
}

void ComplexPlane::setMouseLocation(Vector2i mousePixel)
{
  //Creates coordinates for the mouse location to be displayed
  m_mouseLocation = mapPixelToCoords(mousePixel);
}

//Outputs text information
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
  //Complex #
  complex <float> c (coord.x, coord.y);
  //Tracking variable
  complex <float> z (0.0,0.0);

  while (iterations < MAX_ITER)
    {
      //Mandlebrot Formula
      z = z * z + c;
      //If it goes above 2, return how many iterations it takes
      if (abs(z) > 2.0)
      {
        return iterations;
      }
      iterations++;
    }
  //If it never goes above 2 just return max
  return MAX_ITER;
}

void ComplexPlane::iterationsToRGB(size_t count, Uint8& r, Uint8& g, Uint8& b)
{
    if (count == MAX_ITER)
    {
        // Pixels in the set are black 
        r = 0;
        g = 0;
        b = 0;
    }
      //Based on the number of iterations for a pixel, changes color information
    else
    {
        // used to create a gradient
        float scale = static_cast<float>(count) / MAX_ITER;

        // Blue component increases for background gradient
        //Closer pixels to the set are more blue, creating a glow effect
        b = static_cast<Uint8>(255 * scale);

        // Green and blue components are initalized to 0
        g = 0;
        r = 0;

        // Add a white glow around pixels not in the set
        //"glow" decreases as you get further from the set
        const float glowIntensity = 0.7;
        r += static_cast<Uint8>(255 * glowIntensity * (1.0 - scale));
        g += static_cast<Uint8>(0 * glowIntensity * (1.0 - scale));
        b += static_cast<Uint8>(255 * glowIntensity * (1.0 - scale));
    }
}

Vector2f ComplexPlane::mapPixelToCoords(Vector2i mousePixel)
{
    // Calculate the coordinates in the complex plane
    float realPart = mousePixel.x * m_plane_size.x / m_pixel_size.x + (m_plane_center.x - m_plane_size.x / 2.0);
    float imaginaryPart = mousePixel.y * m_plane_size.y / m_pixel_size.y + (m_plane_center.y - m_plane_size.y / 2.0);

    return Vector2f(realPart, imaginaryPart);
}

    





