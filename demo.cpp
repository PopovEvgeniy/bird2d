#include "bird2d.h"

int main()
{
 char perfomance[8];
 BIRD2D::Common::Timer timer;
 BIRD2D::Input::Keyboard keyboard;
 BIRD2D::Input::Mouse mouse;
 BIRD2D::Graphics::Surface screen;
 BIRD2D::Graphics::Background space;
 BIRD2D::Graphics::Sprite ship;
 BIRD2D::Graphics::Text text;
 BIRD2D::Tools::enable_logging("./log.txt");
 keyboard.initialize();
 screen.initialize();
 space.load("./space.tga");
 space.prepare(screen);
 ship.load("./ship.tga",BIRD2D::HORIZONTAL_ANIMATED,2);
 ship.set_position(screen.get_width()/2,screen.get_height()/2);
 text.load_font("./font.tga");
 text.set_position(0,80);
 mouse.hide();
 timer.set_timer(1);
 memset(perfomance,0,8);
 while(screen.sync())
 {
  if (mouse.check_press(BIRD2D::MOUSE_LEFT)==true)
  {
    break;
  }
  if (keyboard.check_hold(59)==true)
  {
   space.disable_mirror();
  }
  if (keyboard.check_hold(60)==true)
  {
   space.horizontal_mirror();
  }
  if (keyboard.check_hold(61)==true)
  {
   space.vertical_mirror();
  }
  if (keyboard.check_hold(62)==true)
  {
   space.complex_mirror();
  }
  if (keyboard.check_hold(57)==true)
  {
   break;
  }
  if (keyboard.check_hold(72)==true)
  {
   ship.decrease_y(2);
  }
  if (keyboard.check_hold(80)==true)
  {
   ship.increase_y(2);
  }
  if (keyboard.check_hold(75)==true)
  {
   ship.decrease_x(2);
  }
  if (keyboard.check_hold(77)==true)
  {
   ship.increase_x(2);
  }
  if (ship.get_x()>screen.get_width())
  {
   ship.set_x(screen.get_width()/2);
  }
  if (ship.get_y()>screen.get_height())
  {
   ship.set_y(screen.get_height()/2);
  }
  sprintf(perfomance,"%u",screen.get_fps());
  space.draw();
  text.print(perfomance);
  ship.draw();
  if (timer.check_timer()==true)
  {
   ship.step();
  }

 }
 return 0;
}