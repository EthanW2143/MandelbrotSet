#include <iostream>
#incldue <complex>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <vector>

using namespace sf;
using namespace std;

int main() 
{
  //To Do list:
  //After MouseButtonPress set state to: CALCULATING
	
  VideoMode vm(1920, 1080);
  int width = VideoMode::getDesktopMode().width / 2;
  int height = VideoMode::getDesktopMode().height / 2;


  //Loads the font file
  sf::Font font;
  
  if (!font.loadFromFile("planet benson 2.ttf"))
    {
      	cout << "Error loading font file" << endl; // Throw error instead of cout
    }
  
  sf::Text text;
  text.setFont(font);
  text.setString("Click on any 3 points on your screen");
  text.setCharacterSize(36);
  text.setFillColor(sf::Color::Red);
  text.setStyle(sf::Text::Bold | sf::Text::Underlined);

  RenderWindow window(vm, "Mandelbrot", Style::Default);

  ComplexPlane CPlane(width , height);

  while (window.isOpen())
    {
      Event event;

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
			CPlane.setCenter(event.mouseButton.x, event.mouseButton.y);
                }
                  
               // Right click will zoomOut and call setCenter 
               // on the ComplexPlane object with the (x,y) pixel location of the mouse click
                  
                else if (event.mouseButton.button == sf::Mouse::Right)
                {
			CPlane.zoomOut();
			CPlane.setCenter(event.mouseButton.x, event.mouseButton.y);
                }
              
            }

            if (event.type == sf::Event::MouseMoved)
            {
            // Call setMouseLocation on the 
            // ComplexPlane object to store the (x,y) pixel location of the mouse click
            // This will be used later to display the mouse coordinates as it moves
		    CPlane.setMouseLocation(event.mouseButton.x, event.mouseButton.y);
            }

            
 
        }
    
    if (event.type == sf::Event::Keyboard::isKeyPress::(Keyboard::Escape))
      {
        	window.close();
      }

    CPlane.updateRender();
    Cplane.loadText();

    window.clear();
    CPlane.draw(); //Need Parameters
    window.draw(text);
    window.display();

    }
}


  
