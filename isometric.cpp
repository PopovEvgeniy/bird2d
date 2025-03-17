#include "bird2d.h"

int main()
{
 int row,column;
 BIRD2D::Graphics::Screen screen;
 BIRD2D::Graphics::Scene sky;
 BIRD2D::Graphics::Cartoon ground;
 BIRD2D::Graphics::Text text;
 BIRD2D::Transformation::Coordinates cartesian;
 BIRD2D::Transformation::Level level;
 BIRD2D::Input::Keyboard keyboard;
 keyboard.initialize();
 screen.initialize();
 sky.load("sky.tga");
 sky.prepare(screen);
 ground.load("ground.tga");
 text.load_font("font.tga");
 text.set_position(text.get_font_width(),text.get_font_height());
 cartesian.initialize(screen.get_width(),screen.get_height());
 level.initialize(ground.get_width(),ground.get_height());
 while (screen.sync())
 {
  if (keyboard.check_hold(1)==true)
  {
   break;
  }
  sky.draw();
  for (row=0;row<10;++row)
  {
   for (column=0;column<10;++column)
   {
    ground.draw(cartesian.get_screen_x(level.get_x(row,column)),cartesian.get_screen_y(level.get_y(row,column)));
   }

  }
  text.print("Press Esc to exit");
 }
 return 0;
}