#include <iostream>
#include <complex>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <vector>
#include "ComplexPlane.h"

using namespace sf;
using namespace std;

int main() 
{	
  VideoMode vm(1920, 1080);
  //Generate window info for display
  int width = VideoMode::getDesktopMode().width;
  int height = VideoMode::getDesktopMode().height;



  //Loads the font file
  sf::Font font;
  
  if (!font.loadFromFile("planet benson 2.ttf"))
    {
      	cout << "Error loading font file" << endl; // Throw error instead of cout
    }
  
  sf::Text text;
  text.setFont(font);
  text.setString("Click on any 3 points on your screen");
  text.setCharacterSize(24);
  text.setFillColor(sf::Color::Red);
  

  RenderWindow window(vm, "Mandelbrot", Style::Default);
  //Creates our complex plane object
  ComplexPlane CPlane(width , height);

  while (window.isOpen())
    {
      Event event;
      //Event handling
      while (window.pollEvent(event))
      {
            if (event.type == Event::Closed)
              {
			// Quit the game when the window is closed
			window.close(); 
              }
          
            if (event.type == sf::Event::MouseButtonPressed)
            {
              // Left Mouse Click event 
              // will zoomIn and call setCenter
              // on the ComplexPlane object with the (x,y) pixel location of the mouse click
              
                if (event.mouseButton.button == sf::Mouse::Left)
                {
  			CPlane.zoomIn();
			CPlane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
                  
               // Right click will zoomOut and call setCenter 
               // on the ComplexPlane object with the (x,y) pixel location of the mouse click
                  
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
			CPlane.zoomOut();
			CPlane.setCenter(Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
		CPlane.setState(State::CALCULATING);  
            }

          while (event.type == sf::Event::MouseMoved)
    {
        // Call setMouseLocation on the 
        // ComplexPlane object to store the (x,y) pixel location of the mouse move
        // This will be used later to display the mouse coordinates as it moves
        CPlane.setMouseLocation(Vector2i(event.mouseMove.x, event.mouseMove.y));
        event = sf::Event(); // Consume the event to avoid an infinite loop
    }

		if (Keyboard::isKeyPressed(Keyboard::Escape))
     			 {
        			window.close();
     			 }

            
 
	    }
    //After all events are handled, updates the render, draws the text
    //Displays the window and Mandlebrot Set (Not in that order)
    CPlane.updateRender();
    CPlane.loadText(text);
    sf::RenderStates states = sf::RenderStates::Default;
    window.clear();
    CPlane.draw(window, states); //Need Parameters
    window.draw(text);
    window.display();

    }
}


  
