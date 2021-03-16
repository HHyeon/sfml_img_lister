
#include <string>
#include <windows.h>
#include <dirent.h>
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

int currentscrolldirection = 0;
int currentscrolldirectionpast = -1;

vector<string> dirlist;

int currentimgpos=0;
int targetimgpos=9;

void runner()
{
   while(window.isOpen())
   {
      for(int i=0;i<IMGQUEUE;i++)
      {
         if(!loaded[i])
         {
            texture[i] = Texture();
            texture[i].loadFromFile(dirlist.at(currentimgpos)); // names[i]
            sprite[i] = Sprite();
            sprite[i].setTexture(texture[i]);
            loaded[i] = true;

            cout << "loaded : " << dirlist.at(currentimgpos) << endl;

            if(currentimgpos!=targetimgpos)
            {
               currentimgpos++;
               if(currentimgpos>=(int)dirlist.size())
                  currentimgpos=0;
            }

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

         string name = path + "/" + item;

         if(opendir(name.c_str())!=nullptr)
         {
            seekdir(name);
         }
         else
         {
            string ext = name.substr(name.find_last_of(".")+1);

            if(ext=="jpg"||ext=="jpeg"||ext=="png")
            {
               dirlist.push_back(name);
            }
         }
      }
      closedir(dir);
   }
   else
      return 1;
   return 0;
}

int main()
{
   TCHAR buffer[MAX_PATH] = {0};
   GetModuleFileName(NULL, buffer, MAX_PATH);
   string pwd = string(buffer);
   pwd = pwd.substr(0, pwd.find_last_of("\\"));

   while(true)
   {
      unsigned int n = pwd.find("\\");
      if(n==string::npos) break;
      pwd.replace(n,1,"/");
   }

   seekdir(pwd);

   for(int i=0;i<(int)dirlist.size();i++)
      cout << i << ". " << dirlist.at(i) << endl;

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
               currentscrolldirection = 1;


               bool scrollup = true;

               for(int i=0;i<9;i++)
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


                  if(currentscrolldirection!=currentscrolldirectionpast && currentscrolldirectionpast!=-1)
                  {
                     currentimgpos-=12;
                     if(currentimgpos<0)
                        currentimgpos+=dirlist.size();

                     targetimgpos=currentimgpos+3;
                     if(targetimgpos>=(int)dirlist.size())
                        targetimgpos%=dirlist.size();

                  }
                  else if(currentscrolldirection==currentscrolldirectionpast || currentscrolldirectionpast==-1)
                  {
                     if(currentscrolldirectionpast==-1)
                        currentimgpos=3;

                     currentimgpos-=6;
                     if(currentimgpos<0)
                        currentimgpos+=dirlist.size();

                     targetimgpos=currentimgpos-3;
                     if(targetimgpos>=(int)dirlist.size())
                        targetimgpos%=dirlist.size();
                  }
//                  cout << "from " << currentimgpos << " to " << targetimgpos << endl;
               }
               currentscrolldirectionpast = currentscrolldirection;
            }
            else
            {
               currentscrolldirection = 2;

               bool scrolldn = true;

               for(int i=0;i<9;i++)
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

                  if(currentscrolldirection!=currentscrolldirectionpast && currentscrolldirectionpast!=-1)
                  {
                     currentimgpos+=6;
                     if(currentimgpos>=(int)dirlist.size())
                        currentimgpos%=dirlist.size();

                     targetimgpos=currentimgpos+3;
                     if(targetimgpos>=(int)dirlist.size())
                        targetimgpos%=dirlist.size();
                  }
                  else if(currentscrolldirection==currentscrolldirectionpast || currentscrolldirectionpast==-1)
                  {
                     targetimgpos+=3;
                     if(targetimgpos>=(int)dirlist.size())
                        targetimgpos%=dirlist.size();
                  }

//                  cout << "from " << currentimgpos << " to " << targetimgpos << endl;

               }

               currentscrolldirectionpast = currentscrolldirection;


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

            float scaletofit;

            if(texture[currindex].getSize().x > texture[currindex].getSize().y)
               scaletofit = (float)imgwid / (float)texture[currindex].getSize().x;
            else
               scaletofit = (float)imghei / (float)texture[currindex].getSize().y;

            int px = i%3;
            int py = i<3 ? 0 : i/3;

            int xpush = imgwid/2 - ((float)texture[currindex].getSize().x * scaletofit)/2;
            int ypush = imghei/2 - ((float)texture[currindex].getSize().y * scaletofit)/2;

            sprite[currindex].setScale(scaletofit,scaletofit);
            sprite[currindex].setPosition(px*imgwid+xpush, py*imghei+ypush);

            window.draw(sprite[currindex]);
         }
      }

      window.display();
   }

   imgdynamicloadrunner.wait();

}

