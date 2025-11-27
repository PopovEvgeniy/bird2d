#include "bird2d.h"

int main()
{
 BIRD2D::Graphics::Screen screen;
 BIRD2D::Graphics::Scene city;
 BIRD2D::Graphics::Cartoon punk;
 BIRD2D::Graphics::Camera camera;
 BIRD2D::Input::Keyboard keyboard;
 BIRD2D::Input::Mouse mouse;
 keyboard.initialize();
 screen.initialize();
 mouse.initialize();
 mouse.hide();
 city.load("city.tga");
 punk.load("punk.tga");
 city.prepare(screen);
 camera.initialize(screen);
 camera.set_viewport(600,screen.get_height());
 camera.update();
 punk.set_size(96,300);
 punk.set_position(0,camera.get_viewport_height()-punk.get_height());
 while (true)
 {
  screen.sync();
  if (keyboard.check_hold(1)==true)
  {
   break;
  }
  if (keyboard.check_hold(77)==true)
  {
   if (camera.check_horizontal_border(punk.get_box())==true)
   {
    camera.increase_x(12);
   }
   else
   {
    punk.increase_x();
   }

  }
  if (keyboard.check_hold(75)==true)
  {
   if (punk.get_x()<=camera.get_x())
   {
    camera.decrease_x(12);
   }
   else
   {
    punk.decrease_x();
   }

  }
  camera.update();
  city.draw();
  punk.draw();
 }
 return 0;
}