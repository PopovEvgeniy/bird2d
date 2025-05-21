#include "bird2d.h"

int main()
{
 BIRD2D::Graphics::Screen screen;
 BIRD2D::Graphics::Scene city;
 BIRD2D::Graphics::Cartoon punk;
 BIRD2D::Graphics::Text text;
 BIRD2D::Graphics::Camera camera;
 BIRD2D::Input::Keyboard keyboard;
 BIRD2D::Input::Mouse mouse;
 keyboard.initialize();
 screen.initialize();
 mouse.hide();
 city.load("city.tga");
 punk.load("punk.tga");
 text.load_font("font.tga");
 city.prepare(screen);
 camera.initialize(screen);
 camera.set_viewport(640,screen.get_height());
 camera.update();
 punk.set_size(96,300);
 punk.set_position(0,camera.get_viewport_height()-punk.get_height());
 while (screen.sync())
 {
  if (keyboard.check_hold(1)==true)
  {
   break;
  }
  if (keyboard.check_hold(77)==true)
  {
   punk.increase_x(3);
   camera.increase_x(5);
  }
  if (keyboard.check_hold(75)==true)
  {
   punk.decrease_x(3);
   camera.decrease_x(5);
  }
  if ((punk.get_x()+punk.get_width())>=camera.get_screen_width())
  {
   punk.decrease_x(3);
  }
  camera.update();
  city.draw();
  punk.draw();
 }
 return 0;
}