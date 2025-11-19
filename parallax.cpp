#include "bird2d.h"

int main()
{
 char perfomance[8];
 bool limit;
 BIRD2D::Common::Timer timer;
 BIRD2D::Input::Keyboard keyboard;
 BIRD2D::Input::Joystick joystick;
 BIRD2D::Misc::Audio audio;
 BIRD2D::Input::Mouse mouse;
 BIRD2D::Graphics::Screen screen;
 BIRD2D::Graphics::Parallax sky;
 BIRD2D::Graphics::Sprite ship;
 BIRD2D::Graphics::Text text;
 BIRD2D::Tools::enable_logging("./log.txt");
 keyboard.initialize();
 screen.initialize();
 joystick.initialize("/dev/input/js0");
 sky.load("./sky.tga");
 sky.prepare(screen);
 sky.set_vertical_speed(0.001f);
 ship.load("./ship.tga",BIRD2D::HORIZONTAL_ANIMATED,2);
 ship.set_position(screen.get_width()/2,screen.get_height()/2);
 text.load_font("./font.tga");
 text.set_position(text.get_font_width(),text.get_font_height());
 mouse.initialize();
 mouse.hide();
 timer.set_timer(1);
 audio.initialize();
 audio.load("./space.mp3");
 memset(perfomance,0,8);
 limit=true;
 while(true)
 {
  joystick.update();
  audio.play_loop();
  if (keyboard.check_hold(59)==true)
  {
   sky.disable_mirror();
  }
  if (keyboard.check_hold(60)==true)
  {
   sky.horizontal_mirror();
  }
  if (keyboard.check_hold(61)==true)
  {
   sky.vertical_mirror();
  }
  if (keyboard.check_hold(62)==true)
  {
   sky.complex_mirror();
  }
   if (keyboard.check_hold(63)==true)
  {
   limit=true;
  }
  if (keyboard.check_hold(64)==true)
  {
   limit=false;
  }
  if (keyboard.check_hold(1)==true)
  {
   break;
  }
  if (keyboard.check_hold(72)==true)
  {
   ship.decrease_y();
  }
  if (keyboard.check_hold(80)==true)
  {
   ship.increase_y();
  }
  if (keyboard.check_hold(75)==true)
  {
   ship.decrease_x();
  }
  if (keyboard.check_hold(77)==true)
  {
   ship.increase_x();
  }
  if (joystick.check_button_hold(0)==true)
  {
   break;
  }
  if (joystick.get_axis(7)<0)
  {
   ship.decrease_y();
  }
  if (joystick.get_axis(7)>0)
  {
   ship.increase_y();
  }
  if (joystick.get_axis(6)>0)
  {
   ship.increase_x();
  }
  if (joystick.get_axis(6)<0)
  {
   ship.decrease_x();
  }
  if (screen.check_horizontal_border(ship.get_box())==true)
  {
   ship.decrease_x();
  }
  if (screen.check_vertical_border(ship.get_box())==true)
  {
   ship.decrease_y();
  }
  sprintf(perfomance,"%u",screen.get_fps());
  sky.draw();
  text.print(perfomance);
  ship.draw();
  if (timer.check_timer()==true)
  {
   ship.step();
  }
  screen.sync(limit);
 }
 return 0;
}