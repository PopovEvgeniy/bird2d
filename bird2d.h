/*
Bird 2d made by Popov Evgeniy Alekseyevich

Bird 2d license

Copyright (C) 2023 Popov Evgeniy Alekseyevich

This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

*/

#ifndef BIRD2D_H
#define BIRD2D_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <new>
#include <unistd.h>
#include <sys/time.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glxext.h>

namespace BIRD2D
{

 typedef enum
 {
  STATIC_IMAGE=0,
  HORIZONTAL_ANIMATED=1,
  VERTICAL_ANIMATED=2
 } IMAGE_KIND;

 typedef enum
 {
  HORIZONTAL_TEXT=0,
  VERTICAL_TEXT=1
 } TEXT_KIND;

 typedef enum
 {
  MOUSE_LEFT=0,
  MOUSE_RIGHT=1,
  MOUSE_MIDDLE=2
 } MOUSE_BUTTON;

 typedef struct
 {
  unsigned int x;
  unsigned int y;
  unsigned int width;
  unsigned int height;
 } BOX;

 void Halt(const char *message);

 namespace Internal
 {

  unsigned char get_scan_code(const unsigned int key);

  class Synchronization
  {
   private:
   unsigned long int start;
   unsigned long int delay;
   unsigned long int get_time() const;
   void do_pause(const unsigned long int interval);
   protected:
   void set_timer(const unsigned long int interval);
   void wait_timer();
   public:
   Synchronization();
    ~Synchronization();
  };

  class Engine
  {
   private:
   unsigned int display_width;
   unsigned int display_height;
   unsigned int display_depth;
   XVisualInfo *visual_information;
   GLXContext context;
   void open_display();
   void get_visual();
   void get_root_window();
   void create_context();
   void create_window();
   void set_context();
   void disable_vsync();
   void event_setup();
   void show_window();
   protected:
   void prepare_engine();
   bool process_message();
   void Swap();
   GLXContext get_context();
   unsigned int get_display_width() const;
   unsigned int get_display_height() const;
   unsigned int get_depth() const;
   public:
   Engine();
   ~Engine();
  };

 }

 namespace Resource
 {

  template <class RESOURCE>
  RESOURCE *create()
  {
   RESOURCE *target=NULL;
   try
   {
    target=new RESOURCE;
   }
   catch (...)
   {
    BIRD2D::Halt("Can't allocate memory");
   }
   return target;
  }

  template <class RESOURCE>
  RESOURCE *create_array(const size_t amount)
  {
   RESOURCE *target=NULL;
   try
   {
    target=new RESOURCE[amount];
   }
   catch (...)
   {
    BIRD2D::Halt("Can't allocate memory");
   }
   return target;
  }

  template <class RESOURCE>
  void destroy(RESOURCE *target)
  {
   if (target!=NULL)
   {
    delete target;
   }

  }

  template <class RESOURCE>
  void destroy_array(RESOURCE *target)
  {
   if (target!=NULL)
   {
    delete[] target;
   }

  }

 }

 namespace Core
 {

  typedef struct
 {
  int x;
  int y;
 } Vertex;

  typedef struct
 {
  double u;
  double v;
 } Point;

 typedef enum
 {
  MIRROR_NONE=0,
  HORIZONTAL_MIRROR=1,
  VERTICAL_MIRROR=2,
  MIRROR_BOTH=3
 } MIRROR_KIND;

 double get_start_offset(const double current,const double total);
 double get_end_offset(const double current,const double total);

 template <class DATA_TYPE>
 class Buffer
 {
  private:
  DATA_TYPE *buffer;
  size_t length;
  public:

  Buffer()
  {
   buffer=NULL;
   length=0;
  }

  ~Buffer()
  {
   Resource::destroy_array(buffer);
   buffer=NULL;
   length=0;
  }

  void set_length(const size_t amount)
  {
   length=amount;
  }

  void destroy_buffer()
  {
   Resource::destroy_array(buffer);
   buffer=NULL;
   length=0;
  }

  void fill_buffer(const DATA_TYPE value)
  {
   size_t index;
   for (index=0;index<length;++index)
   {
    buffer[index]=value;
   }

  }

  void create_buffer()
  {
   buffer=Resource::create_array<DATA_TYPE>(length);
  }

  size_t get_length() const
  {
   return length;
  }

  DATA_TYPE *get_buffer()
  {
   return buffer;
  }

  DATA_TYPE& operator[](const size_t index)
  {
   return buffer[index];
  }

 };

  class Resizer
  {
   private:
   Buffer<unsigned int> image;
   unsigned int source_width;
   unsigned int source_height;
   unsigned int target_width;
   unsigned int target_height;
   size_t get_source_offset(const unsigned int x,const unsigned int y) const;
   void scale_image(const unsigned int *target);
   void load_image(const unsigned int *target);
   void resize_image(const unsigned int *target);
   void set_setting(const unsigned int width,const unsigned int height);
   void correct_size(const unsigned int limit);
   void calculate_size();
   void create_texture();
   public:
   Resizer();
   ~Resizer();
   void make_texture(const unsigned int *target,const unsigned int width,const unsigned int height,const unsigned int limit);
   unsigned int get_width() const;
   unsigned int get_height() const;
   unsigned int *get_buffer();
  };

  class FPS
  {
   private:
   time_t start;
   unsigned int current;
   unsigned int fps;
   protected:
   void update_counter();
   unsigned int get_fps_amount() const;
   public:
   FPS();
   ~FPS();
  };

  class Shape
  {
   private:
   unsigned int target_width;
   unsigned int target_height;
   unsigned int total_width;
   unsigned int total_height;
   unsigned int current_x;
   unsigned int current_y;
   protected:
   Vertex vertex[4];
   Point point[4];
   void set_data(const Core::MIRROR_KIND kind);
   unsigned int get_total_width() const;
   unsigned int get_total_height() const;
   public:
   Shape();
   ~Shape();
   void set_total_size(const unsigned int width,const unsigned int height);
   void set_size(const unsigned int width,const unsigned int height);
   void set_position(const unsigned int x,const unsigned int y);
   void set_tile_offset(const double row,const double rows,const double column,const double columns);
   void set_horizontal_offset(const double current,const double total);
   void set_vertical_offset(const double current,const double total);
  };

  class Rectangle:public Shape
  {
   private:
   unsigned int texture;
   void create_texture(const unsigned int *buffer);
   void delete_texture();
   void check_texture();
   void load_data();
   void draw_rectangle();
   public:
   Rectangle();
   ~Rectangle();
   void enable_transparent();
   void disable_transparent();
   void prepare(const unsigned int *buffer);
   void draw(const Core::MIRROR_KIND kind);
   void destroy_texture();
   bool is_texture_exist() const;
  };

  class Render
  {
   private:
   unsigned int get_maximum_texture_size() const;
   void set_perfomance_setting();
   void set_render_hints();
   void set_common_setting();
   void disable_depth_buffer();
   void set_matrix_setting();
   void set_perspective(const unsigned int width,const unsigned int height);
   void create_render(const unsigned int width,const unsigned int height);
   protected:
   void clear_stage();
   void start_render(const unsigned int width,const unsigned int height);
   public:
   Render();
   ~Render();
  };

 }

 namespace Input
 {

  class Keyboard
  {
   private:
   Core::Buffer<unsigned char> preversion;
   bool check_state(const unsigned char code,const unsigned char state);
   public:
   Keyboard();
   ~Keyboard();
   void initialize();
   bool check_hold(const unsigned char code);
   bool check_press(const unsigned char code);
   bool check_release(const unsigned char code);
   bool is_ready() const;
  };

  class Mouse
  {
   private:
   unsigned char preversion[3];
   bool check_state(const BIRD2D::MOUSE_BUTTON button,const unsigned char state);
   public:
   Mouse();
   ~Mouse();
   void show();
   void hide();
   void set_position(const unsigned int x,const unsigned int y);
   unsigned int get_x();
   unsigned int get_y();
   bool check_hold(const BIRD2D::MOUSE_BUTTON button);
   bool check_press(const BIRD2D::MOUSE_BUTTON button);
   bool check_release(const BIRD2D::MOUSE_BUTTON button);
  };

 }

 namespace File
 {

  class Binary_File
  {
   private:
   FILE *target;
   protected:
   FILE *get_target();
   void set_target(FILE *point);
   public:
   Binary_File();
   ~Binary_File();
   void close();
   void set_position(const long int offset);
   long int get_position();
   long int get_length();
   bool check_error();
   bool is_open() const;
  };

  class Input_File:public Binary_File
  {
   public:
   Input_File();
   ~Input_File();
   void open(const char *name);
   void read(void *buffer,const size_t length);
  };

  class Output_File:public Binary_File
  {
   public:
   Output_File();
   ~Output_File();
   void open(const char *name);
   void create_temp();
   void write(const void *buffer,const size_t length);
   void flush();
  };

 }

 namespace Graphics
 {

  class Surface:public Core::FPS, public Core::Render, public Internal::Engine, public Internal::Synchronization
 {
   private:
   void screen_setup();
   public:
   Surface();
   ~Surface();
   void clear_screen();
   void initialize();
   bool update();
   bool sync();
   bool is_ready();
   unsigned long int get_color() const;
   unsigned int get_fps() const;
   unsigned int get_width() const;
   unsigned int get_height() const;
   Surface* get_handle();
  };

   class Image
  {
   private:
   Core::Buffer<unsigned char> data;
   unsigned int width;
   unsigned int height;
   void uncompress_tga_data(const unsigned char *target);
   void load_tga(File::Input_File &target);
   public:
   Image();
   ~Image();
   unsigned int get_width() const;
   unsigned int get_height() const;
   size_t get_length() const;
   unsigned char *get_data();
   Image* get_handle();
   void destroy_image();
   unsigned char *load_tga(const char *name);
  };

  class Picture
  {
   private:
   Core::Buffer<unsigned int> image;
   unsigned int image_width;
   unsigned int image_height;
   protected:
   void set_image_size(const unsigned int width,const unsigned int height);
   void create_storage();
   void copy_image(const unsigned int *target);
   void load_image(Image *buffer);
   public:
   Picture();
   ~Picture();
   void destroy_image();
   bool is_storage_empty() const;
   unsigned int get_image_width() const;
   unsigned int get_image_height() const;
   size_t get_image_length() const;
   unsigned int *get_image();
  };

  class Animation
  {
   private:
   unsigned int frames;
   unsigned int frame;
   protected:
   void reset_animation_setting();
   void increase_frame();
   void set_frame(const unsigned int target);
   void set_frames(const unsigned int amount);
   public:
   Animation();
   ~Animation();
   unsigned int get_frames() const;
   unsigned int get_frame() const;
  };

  class Billboard
  {
   private:
   Core::MIRROR_KIND mirror;
   bool transparent;
   unsigned int current_x;
   unsigned int current_y;
   unsigned int sprite_width;
   unsigned int sprite_height;
   void check_transparent();
   void draw_sprite_image();
   protected:
   Core::Rectangle billboard;
   void reset_billboard_setting();
   void prepare(const unsigned int width,const unsigned int height,const unsigned int *picture);
   public:
   Billboard();
   ~Billboard();
   void set_transparent(const bool enabled);
   bool get_transparent() const;
   void set_width(const unsigned int width);
   void set_height(const unsigned int height);
   void set_size(const unsigned int width,const unsigned int height);
   void set_position(const unsigned int x,const unsigned int y);
   void set_x(const unsigned int x);
   void set_y(const unsigned int y);
   unsigned int increase_x();
   unsigned int decrease_x();
   unsigned int increase_y();
   unsigned int decrease_y();
   unsigned int increase_x(const unsigned int increment);
   unsigned int decrease_x(const unsigned int decrement);
   unsigned int increase_y(const unsigned int increment);
   unsigned int decrease_y(const unsigned int decrement);
   unsigned int get_x() const;
   unsigned int get_y() const;
   unsigned int get_width() const;
   unsigned int get_height() const;
   BIRD2D::BOX get_box() const;
   void disable_mirror();
   void horizontal_mirror();
   void vertical_mirror();
   void complex_mirror();
   void draw();
   void draw(const unsigned int x,const unsigned int y);
   void draw(const bool transparency);
   void draw(const bool transparency,const unsigned int x,const unsigned int y);
  };

  class Sprite:public Billboard,public Animation,public Picture
  {
   private:
   BIRD2D::IMAGE_KIND current_kind;
   void reset_sprite_setting();
   void set_sprite_setting();
   void configure_sprite();
   void set_sprite_frame();
   void set_kind(const BIRD2D::IMAGE_KIND kind);
   public:
   Sprite();
   ~Sprite();
   Sprite* get_handle();
   BIRD2D::IMAGE_KIND get_kind() const;
   void set_setting(const BIRD2D::IMAGE_KIND kind,const unsigned int frames);
   void load(Image *buffer,const BIRD2D::IMAGE_KIND kind,const unsigned int frames);
   void load(Image *buffer);
   void load(Image &buffer,const BIRD2D::IMAGE_KIND kind,const unsigned int frames);
   void load(Image &buffer);
   void load(const char *name,const BIRD2D::IMAGE_KIND kind,const unsigned int frames);
   void load(const char *name);
   void set_target(const unsigned int target);
   void step();
   void destroy();
   void clone(Sprite *target);
   void clone(Sprite &target);
  };

  class Sheet:public Billboard,public Picture,public Animation
  {
   private:
   unsigned int rows;
   unsigned int columns;
   void reset_sheet_setting();
   void prepare_sheet();
   public:
   Sheet();
   ~Sheet();
   Sheet* get_handle();
   unsigned int get_row(const unsigned int target) const;
   unsigned int get_column(const unsigned int target) const;
   unsigned int calculate(const unsigned int row,const unsigned int column) const;
   unsigned int get_rows() const;
   unsigned int get_columns() const;
   void destroy();
   void clone(Sheet *target);
   void clone(Sheet &target);
   void select(const unsigned int row,const unsigned int column);
   void select(const unsigned int target);
   void step();
   void load(Image *sheet,const unsigned int row_amount,const unsigned int column_amount);
   void load(Image &sheet,const unsigned int row_amount,const unsigned int column_amount);
   void load(const char *name,const unsigned int row_amount,const unsigned int column_amount);
  };

  class Background
  {
   private:
   Graphics::Sprite stage;
   public:
   Background();
   ~Background();
   void prepare(const Surface *surface);
   void prepare(Surface &surface);
   void prepare(const unsigned int width,const unsigned int height);
   void set_setting(const BIRD2D::IMAGE_KIND kind,const unsigned int frames);
   void load(Image *background,const IMAGE_KIND kind,const unsigned int frames);
   void load(Image *background);
   void load(Image &background,const BIRD2D::IMAGE_KIND kind,const unsigned int frames);
   void load(Image &background);
   void load(const char *name,const BIRD2D::IMAGE_KIND kind,const unsigned int frames);
   void load(const char *name);
   void disable_mirror();
   void horizontal_mirror();
   void vertical_mirror();
   void complex_mirror();
   void set_target(const unsigned int target);
   void step();
   void draw();
   void draw(const unsigned int target);
   void destroy_image();
   void destroy();
   unsigned int get_frame() const;
   unsigned int get_frames() const;
   unsigned int get_width() const;
   unsigned int get_height() const;
   BIRD2D::IMAGE_KIND get_kind() const;
  };

  class Text
  {
   private:
   Graphics::Sheet text;
   BIRD2D::TEXT_KIND orientation;
   unsigned int current_x;
   unsigned int current_y;
   void increase_position();
   void restore_position();
   public:
   Text();
   ~Text();
   BIRD2D::TEXT_KIND get_orientation() const;
   void set_orientation(const BIRD2D::TEXT_KIND target);
   unsigned int get_font_width() const;
   unsigned int get_font_height() const;
   void set_position(const unsigned int x,const unsigned int y);
   void set_size(const unsigned int width,const unsigned int height);
   void set_width(const unsigned int width);
   void set_height(const unsigned int height);
   void set_setting(const unsigned int width,const unsigned int height,const BIRD2D::TEXT_KIND kind);
   void load_font(Image *font);
   void load_font(Image &font);
   void load_font(const char *name);
   void print(const char target);
   void print(const char *target);
   void print(const unsigned int x,const unsigned int y,const char target);
   void print(const unsigned int x,const unsigned int y,const char *target);
   void disable_mirror();
   void horizontal_mirror();
   void vertical_mirror();
   void complex_mirror();
   void destroy_image();
   void destroy_font();
  };

 }

 namespace Common
 {

  class Timer
  {
   private:
   double interval;
   time_t start;
   public:
   Timer();
   ~Timer();
   void set_timer(const double seconds);
   double get_interval() const;
   bool check_timer();
  };

  class Collision
  {
   private:
   BIRD2D::BOX first;
   BIRD2D::BOX second;
   bool check_horizontal_collision() const;
   bool check_vertical_collision() const;
   public:
   Collision();
   ~Collision();
   void set_target(const BIRD2D::BOX &first_target,const BIRD2D::BOX &second_target);
   bool check_collision() const;
   bool check_collision(const BIRD2D::BOX &first_target,const BIRD2D::BOX &second_target);
  };

 }

 namespace Filesystem
 {
  bool delete_file(const char *name);
  bool file_exist(const char *name);
 }

 namespace Tools
 {
  BIRD2D::BOX generate_box(const unsigned int x,const unsigned int y,const unsigned int width,const unsigned int height);
  void quit();
  bool enable_logging(const char *name);
  void randomize();
  unsigned int get_random(const unsigned int number);
  unsigned int get_texture_size();
 }

}

#endif