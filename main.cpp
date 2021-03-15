#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

#define WIDTH 1200
#define HEIGHT 900

#define IMGQUEUE 9
bool loaded[IMGQUEUE];
Texture texture[IMGQUEUE];
Sprite sprite[IMGQUEUE];

RenderWindow window(VideoMode(WIDTH,HEIGHT),"imglister", Style::Default);

void runner()
{
   while(window.isOpen())
   {
      for(int i=0;i<IMGQUEUE;i++)
      {
         if(!loaded[i])
         {
            texture[i] = Texture();
            texture[i].loadFromFile("11758.jpg");
            sprite[i] = Sprite();
            sprite[i].setTexture(texture[i]);
            loaded[i] = true;
            cout <<"loaded "<< i << endl;
         }

         if(!window.isOpen()) break;
      }
      sleep(milliseconds(100));
   }
}

int main()
{
   window.setFramerateLimit(60);

   Thread imgdynamicloadrunner(runner);
   imgdynamicloadrunner.launch();

   int ypos = 0;

   Event e;
   while(window.isOpen())
   {
      while(window.pollEvent(e))
      {
         switch((int)e.type)
         {
         case Event::Closed:
            window.close();
            break;

         case Event::KeyPressed:
            {
               if(e.key.code == Keyboard::Q)
                  window.close();
            }
            break;

         case Event::MouseWheelMoved:
            if(e.mouseWheel.delta>0)
            {
               ypos+=100;
            }
            else
            {
               ypos-=100;
            }

            cout << ypos << endl;
            break;
         }
      }

      window.clear();

      for(int i=0;i<IMGQUEUE;i++)
      {
         if(loaded[i])
         {
            int imgwid = WIDTH/3;
            int imghei = HEIGHT/3;

            float scaletofit=0;

            if(texture[i].getSize().x > texture[i].getSize().y)
               scaletofit = (float)imgwid / (float)texture[i].getSize().x;
            else
               scaletofit = (float)imghei / (float)texture[i].getSize().y;

            int px = i%3;
            int py = i<3 ? 0 : i/3;

            sprite[i].setScale(scaletofit,scaletofit);
            sprite[i].setPosition(px*imgwid, py*imghei + ypos);

//            cout << px <<", "<< py << endl;

            window.draw(sprite[i]);

         }
         else
         {

         }
      }

      window.display();
   }

   imgdynamicloadrunner.wait();



}

