
#include <dirent.h>
#include <string>
#include <windows.h>
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
int displayindexfield[IMGQUEUE] = {0,1,2,3,4,5,6,7,8};

RenderWindow window(VideoMode(WIDTH,HEIGHT),"imglister", Style::Default);

bool scrolldn = false;
bool scrollup = false;

string names[IMGQUEUE] =
{
   "1.png",
   "2.png",
   "3.png",
   "4.png",
   "5.png",
   "6.png",
   "7.png",
   "8.png",
   "9.png"
};

void runner()
{
   while(window.isOpen())
   {
      for(int i=0;i<IMGQUEUE;i++)
      {
         if(!loaded[i])
         {
            texture[i] = Texture();
            texture[i].loadFromFile(names[i]);
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

int seekdir(string path)
{
   DIR *dir;
   struct dirent *dirread;

   if((dir=opendir(path.c_str()))!=nullptr)
   {
      while((dirread=readdir(dir))!=nullptr)
      {
         string item = string(dirread->d_name);
         if(item == "." || item == "..") continue;

         string name = path + item;
         cout << name << endl;
      }

      closedir(dir);
   }
   else
   {
      return 1;
   }
}

int main()
{
   TCHAR buffer[MAX_PATH] = {0};
   GetModuleFileName(NULL, buffer, MAX_PATH);
   string pwd = string(buffer);
   pwd = pwd.substr(0, pwd.find_last_of("\\"));

   while(true)
   {
      int n = pwd.find("\\");
      if(n==string::npos) break;
      pwd.replace(n,1,"/");
   }

//   seekdir(pwd);
//
//   return 0;

   window.setFramerateLimit(30);

   Thread imgdynamicloadrunner(runner);
   imgdynamicloadrunner.launch();

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
               scrollup = true;

               for(int i=3;i<9;i++)
               {
                  if(!loaded[displayindexfield[i]])
                     scrollup = false;
                  if(!scrollup) break;
               }

               if(scrollup)
               {
                  int tmp[3]={0,0,0};

                  for(int i=0;i<3;i++)
                     tmp[i] = displayindexfield[i+6];
                  for(int i=8;i>=3;i--)
                     displayindexfield[i] = displayindexfield[i-3];
                  for(int i=0;i<3;i++)
                  {
                     displayindexfield[i] = tmp[i];
                     loaded[tmp[i]] = false;
                  }

               }
               else
               {
                  cout << "wait for loading...\n";
               }
            }
            else
            {
               scrolldn = true;

               for(int i=0;i<9-3;i++)
               {
                  if(!loaded[displayindexfield[i]])
                     scrolldn = false;
                  if(!scrolldn) break;
               }

               if(scrolldn)
               {
                  int tmp[3]={0,0,0};

                  for(int i=0;i<3;i++)
                     tmp[i] = displayindexfield[i];
                  for(int i=0;i<6;i++)
                     displayindexfield[i] = displayindexfield[i+3];
                  for(int i=0;i<3;i++)
                  {
                     displayindexfield[6+i] = tmp[i];
                     loaded[tmp[i]] = false;
                  }
               }
               else
               {
                  cout << "wait for loading...\n";
               }


            }
            break;
         }
      }


      window.clear();

      for(int i=0;i<IMGQUEUE;i++)
      {
         int currindex = displayindexfield[i];

         if(loaded[currindex])
         {
            int imgwid = WIDTH/3;
            int imghei = HEIGHT/3;

            float scaletofit=0;

            if(texture[currindex].getSize().x > texture[currindex].getSize().y)
               scaletofit = (float)imgwid / (float)texture[currindex].getSize().x;
            else
               scaletofit = (float)imghei / (float)texture[currindex].getSize().y;

            int px = i%3;
            int py = i<3 ? 0 : i/3;

            sprite[currindex].setScale(scaletofit,scaletofit);
            sprite[currindex].setPosition(px*imgwid, py*imghei);

//            cout << px <<", "<< py << endl;

            window.draw(sprite[currindex]);
         }
      }

      window.display();
   }

   imgdynamicloadrunner.wait();

}

