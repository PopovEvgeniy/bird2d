/*
Bird 2D was made by Popov Evgeniy Alekseyevich

Bird 2D license

Copyright (C) 2023 - 2025 Popov Evgeniy Alekseyevich

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

#include "bird2d.h"

namespace
{

 typedef struct
 {
  unsigned char id:8;
  unsigned char color_map:8;
  unsigned char type:8;
 } TGA_head;

 typedef struct
 {
  unsigned short int index:16;
  unsigned short int length:16;
  unsigned char map_size:8;
 } TGA_map;

 typedef struct
 {
  unsigned short int x:16;
  unsigned short int y:16;
  unsigned short int width:16;
  unsigned short int height:16;
  unsigned char color:8;
  unsigned char descriptor:8;
 } TGA_image;

 unsigned int mouse_x=0;
 unsigned int mouse_y=0;
 unsigned int MAXIMUM_TEXTURE_SIZE=0;
 const size_t KEYBOARD=256;
 const size_t MOUSE=3;
 const unsigned char KEY_RELEASE=0;
 const unsigned char KEY_PRESS=1;

 unsigned char Keys[KEYBOARD]={KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,
                               KEY_RELEASE,KEY_RELEASE,KEY_RELEASE,KEY_RELEASE};

 unsigned char Buttons[MOUSE]={KEY_RELEASE,KEY_RELEASE,KEY_RELEASE};

Window window=None;
Display *display=NULL;
}

namespace BIRD2D
{

 void Halt(const char *message)
 {
  puts(message);
  exit(EXIT_FAILURE);
 }

 namespace Internal
 {

   unsigned char get_scan_code(const KeySym key)
   {
     unsigned char code;
     code=0;
     switch  (key)
     {
       case XK_Escape:
       code=1;
       break;
       case XK_1:
       code=2;
       break;
       case XK_exclam:
       code=2;
       break;
       case XK_2:
       code=3;
       break;
       case XK_at:
       code=3;
       break;
       case XK_3:
       code=4;
       break;
       case XK_numbersign:
       code=4;
       break;
       case XK_4:
       code=5;
       break;
       case XK_dollar:
       code=5;
       break;
       case XK_5:
       code=6;
       break;
       case XK_percent:
       code=6;
       break;
       case XK_6:
       code=7;
       break;
       case XK_asciicircum:
       code=7;
       break;
       case XK_7:
       code=8;
       break;
       case XK_ampersand:
       code=8;
       break;
       case XK_8:
       code=9;
       break;
       case XK_asterisk:
       code=9;
       break;
       case XK_9:
       code=10;
       break;
       case XK_parenleft:
       code=10;
       break;
       case XK_0:
       code=11;
       break;
       case XK_parenright:
       code=11;
       break;
       case XK_minus:
       code=12;
       break;
       case XK_underscore:
       code=12;
       break;
       case XK_equal:
       code=13;
       break;
       case XK_plus:
       code=13;
       break;
       case XK_BackSpace:
       code=14;
       break;
       case XK_Tab:
       code=15;
       break;
       case XK_Q:
       code=16;
       break;
       case XK_q:
       code=16;
       break;
       case XK_W:
       code=17;
       break;
       case XK_w:
       code=17;
       break;
       case XK_E:
       code=18;
       break;
       case XK_e:
       code=18;
       break;
       case XK_R:
       code=19;
       break;
       case XK_r:
       code=19;
       break;
       case XK_T:
       code=20;
       break;
       case XK_t:
       code=20;
       break;
       case XK_Y:
       code=21;
       break;
       case XK_y:
       code=21;
       break;
       case XK_U:
       code=22;
       break;
       case XK_u:
       code=22;
       break;
       case XK_I:
       code=23;
       break;
       case XK_i:
       code=23;
       break;
       case XK_O:
       code=24;
       break;
       case XK_o:
       code=24;
       break;
       case XK_P:
       code=25;
       break;
       case XK_p:
       code=25;
       break;
       case XK_braceright:
       code=26;
       break;
       case XK_bracketleft:
       code=27;
       break;
       case XK_Return:
       code=28;
       break;
       case XK_KP_Enter:
       code=28;
       break;
       case XK_Control_L:
       code=29;
       break;
       case XK_Control_R:
       code=29;
       break;
       case XK_A:
       code=30;
       break;
       case XK_a:
       code=30;
       break;
       case XK_S:
       code=31;
       break;
       case XK_s:
       code=31;
       break;
       case XK_D:
       code=32;
       break;
       case XK_d:
       code=32;
       break;
       case XK_F:
       code=33;
       break;
       case XK_f:
       code=33;
       break;
       case XK_G:
       code=34;
       break;
       case XK_g:
       code=34;
       break;
       case XK_H:
       code=35;
       break;
       case XK_h:
       code=35;
       break;
       case XK_J:
       code=36;
       break;
       case XK_j:
       code=36;
       break;
       case XK_K:
       code=37;
       break;
       case XK_k:
       code=37;
       break;
       case XK_L:
       code=38;
       break;
       case XK_l:
       code=38;
       break;
       case XK_colon:
       code=39;
       break;
       case XK_semicolon:
       code=39;
       break;
       case XK_quotedbl:
       code=40;
       break;
       case XK_quoteright:
       code=40;
       break;
       case XK_asciitilde:
       code=41;
       break;
       case XK_Shift_L:
       code=42;
       break;
       case XK_backslash:
       code=43;
       break;
       case XK_bar:
       code=43;
       break;
       case XK_Z:
       code=44;
       break;
       case XK_z:
       code=44;
       break;
       case XK_X:
       code=45;
       break;
       case XK_x:
       code=45;
       break;
       case XK_C:
       code=46;
       break;
       case XK_c:
       code=46;
       break;
       case XK_V:
       code=47;
       break;
       case XK_v:
       code=47;
       break;
       case XK_B:
       code=48;
       break;
       case XK_b:
       code=48;
       break;
       case XK_N:
       code=49;
       break;
       case XK_n:
       code=49;
       break;
       case XK_M:
       code=50;
       break;
       case XK_m:
       code=50;
       break;
       case XK_comma:
       code=51;
       break;
       case XK_less:
       code=51;
       break;
       case XK_period:
       code=52;
       break;
       case XK_greater:
       code=52;
       break;
       case XK_slash:
       code=53;
       break;
       case XK_question:
       code=53;
       break;
       case XK_Shift_R:
       code=54;
       break;
       case XK_Print:
       code=55;
       break;
       case XK_Alt_L:
       code=56;
       break;
       case XK_Alt_R:
       code=56;
       break;
       case XK_space:
       code=57;
       break;
       case XK_Caps_Lock:
       code=58;
       break;
       case XK_F1:
       code=59;
       break;
       case XK_F2:
       code=60;
       break;
       case XK_F3:
       code=61;
       break;
       case XK_F4:
       code=62;
       break;
       case XK_F5:
       code=63;
       break;
       case XK_F6:
       code=64;
       break;
       case XK_F7:
       code=65;
       break;
       case XK_F8:
       code=66;
       break;
       case XK_F9:
       code=67;
       break;
       case XK_F10:
       code=68;
       break;
       case XK_Num_Lock:
       code=69;
       break;
       case XK_Scroll_Lock:
       code=70;
       break;
       case XK_Home:
       code=71;
       break;
       case XK_KP_7:
       code=71;
       break;
       case XK_KP_Home:
       code=71;
       break;
       case XK_Up:
       code=72;
       break;
       case XK_KP_8:
       code=72;
       break;
       case XK_KP_Up:
       code=72;
       break;
       case XK_Page_Up:
       code=73;
       break;
       case XK_KP_9:
       code=73;
       break;
       case XK_KP_Page_Up:
       code=73;
       break;
       case XK_KP_Subtract:
       code=74;
       break;
       case XK_Left:
       code=75;
       break;
       case XK_KP_4:
       code=75;
       break;
       case XK_KP_Left:
       code=75;
       break;
       case XK_KP_5:
       code=76;
       break;
       case XK_Right:
       code=77;
       break;
       case XK_KP_6:
       code=77;
       break;
       case XK_KP_Right:
       code=77;
       break;
       case XK_KP_Add:
       code=78;
       break;
       case XK_End:
       code=79;
       break;
       case XK_KP_1:
       code=79;
       break;
       case XK_KP_End:
       code=79;
       break;
       case XK_Down:
       code=80;
       break;
       case XK_KP_2:
       code=80;
       break;
       case XK_KP_Down:
       code=80;
       break;
       case XK_Page_Down:
       code=81;
       break;
       case XK_KP_3:
       code=81;
       break;
       case XK_KP_Page_Down:
       code=81;
       break;
       case XK_Insert:
       code=82;
       break;
       case XK_KP_0:
       code=82;
       break;
       case XK_KP_Insert:
       code=82;
       break;
       case XK_Delete:
       code=83;
       break;
       case XK_KP_Decimal:
       code=83;
       break;
       case XK_KP_Delete:
       code=83;
       break;
       case XK_F11:
       code=133;
       break;
       case XK_F12:
       code=134;
       break;
       default:
       code=0;
       break;
     }
     return code;
   }

   Synchronization::Synchronization()
   {
     start=0;
     delay=0;
   }

   Synchronization::~Synchronization()
   {

   }

   unsigned long int Synchronization::get_time() const
   {
     timeval current;
     if (gettimeofday(&current,NULL)!=0)
     {
       current.tv_sec=0;
       current.tv_usec=1;
     }
     return (current.tv_sec*1000)+(current.tv_usec/1000);
   }

   void Synchronization::do_pause(const unsigned long int interval)
   {
     usleep(interval*1000);
   }

   void Synchronization::set_timer(const unsigned long int interval)
   {
     start=this->get_time();
     delay=interval;
   }

   void Synchronization::wait_timer()
   {
     unsigned long int interval;
     interval=this->get_time()-start;
     if  (interval<delay)
     {
       this->do_pause(delay-interval);
     }
     start=this->get_time();
   }

   Engine::Engine()
   {
     display_width=0;
     display_height=0;
     display_depth=0;
     root=None;
     visual_information=NULL;
     context=NULL;
     glXSwapIntervalEXT=NULL;
   }

   Engine::~Engine()
   {
     if  (context!=NULL)
     {
       glXMakeCurrent(display,None,NULL);
       glXDestroyContext(display,context);
       context=NULL;
     }
     if  (visual_information!=NULL)
     {
       XFree(visual_information);
       visual_information=NULL;
     }
     if  (window!=None)
     {
       XDestroyWindow(display,window);
       window=None;
     }
     if  (display!=NULL)
     {
       XCloseDisplay(display);
       display=NULL;
     }

   }

    void Engine::open_display()
   {
     display=XOpenDisplay(NULL);
     if (display==NULL)
     {
       BIRD2D::Halt("Can't open display");
     }

   }

   void Engine::get_visual()
  {
    int setting[]={GLX_RGBA,GLX_DEPTH_SIZE,16,GLX_DOUBLEBUFFER,None};
    visual_information=glXChooseVisual(display,DefaultScreen(display),setting);
    if  (visual_information==NULL)
    {
      BIRD2D::Halt("Can't get visual information");
    }
    else
    {
      display_width=DisplayWidth(display,visual_information->screen);
      display_height=DisplayHeight(display,visual_information->screen);
      display_depth=visual_information->depth;
    }

   }

   void Engine::get_root_window()
   {
     root=RootWindow(display,visual_information->screen);
     if  (root==None)
    {
      BIRD2D::Halt("Can't get root window");
    }

   }

   void Engine::create_context()
   {
     context=glXCreateContext(display,visual_information,NULL,GL_TRUE);
     if  (context==NULL)
     {
       BIRD2D::Halt("Can't create OPENGL context");
     }

   }

   void Engine::create_window()
   {
     XSetWindowAttributes attributes;
     memset(&attributes,0,sizeof(XSetWindowAttributes));
     attributes.border_pixel=BlackPixel(display,visual_information->screen);
     attributes.background_pixel=WhitePixel(display,visual_information->screen);
     attributes.override_redirect=True;
     attributes.save_under=False;
     attributes.backing_store=NotUseful;
     attributes.event_mask=KeyPressMask|KeyRelease|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|ButtonMotionMask|StructureNotifyMask;
     attributes.colormap=XCreateColormap(display,root,visual_information->visual,AllocNone);
     window=XCreateWindow(display,root,0,0,display_width,display_height,0,visual_information->depth,InputOutput,visual_information->visual,CWSaveUnder|CWBackingStore|CWOverrideRedirect|CWBackPixel|CWColormap|CWBorderPixel|CWEventMask,&attributes);
     if  (window==None)
     {
       BIRD2D::Halt("Can't create window");
     }

   }

   void Engine::set_context()
   {
     if  (glXMakeCurrent(display,window,context)==False)
     {
       BIRD2D::Halt("Can't set render context");
     }

   }

   void Engine::disable_vsync()
   {
     glXSwapIntervalEXT=reinterpret_cast<PFNGLXSWAPINTERVALEXTPROC>(glXGetProcAddress(reinterpret_cast<const GLubyte*>("glXSwapIntervalEXT")));
     if  (glXSwapIntervalEXT!=NULL)
     {
       glXSwapIntervalEXT(display,window,0);
     }

   }

   void Engine::event_setup()
   {
     if (XSelectInput(display,window,KeyPressMask|KeyRelease|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|ButtonMotionMask|StructureNotifyMask)==0)
     {
       BIRD2D::Halt("Can't set event configuration");
     }

   }

   void Engine::show_window()
   {
     XMapWindow(display,window);
     XFlush(display);
   }

   void Engine::prepare_engine()
   {
     this->open_display();
     this->get_visual();
     this->get_root_window();
     this->create_context();
     this->create_window();
     this->set_context();
     this->disable_vsync();
     this->event_setup();
     this->show_window();
   }

   bool Engine::process_message()
   {
     bool run;
     XEvent event;
     run=true;
     XSetInputFocus(display,window,RevertToParent,CurrentTime);
     while (XCheckWindowEvent(display,window,KeyPressMask|KeyRelease|ButtonPressMask|ButtonReleaseMask|PointerMotionMask|ButtonMotionMask|StructureNotifyMask,&event)==True)
     {
       switch (event.type)
       {
         case DestroyNotify:
         run=false;
         break;
         case KeyPress:
         Keys[Internal::get_scan_code(XLookupKeysym(&event.xkey,0))]=KEY_PRESS;
         break;
         case KeyRelease:
         Keys[Internal::get_scan_code(XLookupKeysym(&event.xkey,0))]=KEY_RELEASE;
         break;
         case MotionNotify:
         mouse_x=event.xbutton.x;
         mouse_y=event.xbutton.y;
         break;
         case ButtonPress:
         if  (event.xbutton.button==Button1) Buttons[BIRD2D::MOUSE_LEFT]=KEY_PRESS;
         if  (event.xbutton.button==Button2) Buttons[BIRD2D::MOUSE_RIGHT]=KEY_PRESS;
         if  (event.xbutton.button==Button3) Buttons[BIRD2D::MOUSE_MIDDLE]=KEY_PRESS;
         break;
         case ButtonRelease:
         if  (event.xbutton.button==Button1) Buttons[BIRD2D::MOUSE_LEFT]=KEY_RELEASE;
         if  (event.xbutton.button==Button2) Buttons[BIRD2D::MOUSE_RIGHT]=KEY_RELEASE;
         if  (event.xbutton.button==Button3) Buttons[BIRD2D::MOUSE_MIDDLE]=KEY_RELEASE;
         break;
         default:
         ;
         break;
       }

     }
     return run;
   }

   void Engine::Swap()
   {
     if  (context!=NULL)
     {
       glXSwapBuffers(display,window);
     }

   }

   GLXContext Engine::get_context()
   {
     return context;
   }

   unsigned int Engine::get_display_width() const
   {
     return display_width;
   }

   unsigned int Engine::get_display_height() const
   {
     return display_height;
   }

   unsigned int Engine::get_depth() const
   {
     return display_depth;
   }

 }

 namespace Core
 {

  float get_start_offset(const float current,const float total)
  {
   return (1.0f/total)*(current-1.0f);
  }

  float get_end_offset(const float current,const float total)
  {
   return (1.0f/total)*current;
  }

  unsigned int get_pixel_component(const unsigned int pixel,const Core::PIXEL_COMPONENT component)
  {
   return (pixel >> component) & 0xFF;
  }

  unsigned int make_pixel(const unsigned int red,const unsigned int green,const unsigned int blue,const unsigned int alpha)
  {
   return (alpha << 24)|(blue << 16)|(green << 8)|red;
  }

  size_t get_offset(const unsigned int x,const unsigned int y,const unsigned int width)
  {
   return static_cast<size_t>(x)+static_cast<size_t>(y)*static_cast<size_t>(width);
  }

  Resizer::Resizer()
  {
   image=NULL;
   source_width=0;
   source_height=0;
   x_ratio=0;
   y_ratio=0;
   target_width=1;
   target_height=1;
   normalization=UCHAR_MAX*UCHAR_MAX;
  }

  Resizer::~Resizer()
  {
   Resource::destroy_array(image);
   image=NULL;
  }

  unsigned int Resizer::get_x_difference(const unsigned int x) const
  {
   return (x*x_ratio+1)%UCHAR_MAX;
  }

  unsigned int Resizer::get_y_difference(const unsigned int y) const
  {
   return (y*y_ratio+1)%UCHAR_MAX;
  }

  unsigned int Resizer::get_source_x(const unsigned int x) const
  {
   return (x*x_ratio+1)/UCHAR_MAX;
  }

  unsigned int Resizer::get_source_y(const unsigned int y) const
  {
   return (y*y_ratio+1)/UCHAR_MAX;
  }

  unsigned int Resizer::get_next_x(const unsigned int x) const
  {
   unsigned int next_x;
   next_x=x+1;
   if (next_x==source_width)
   {
    --next_x;
   }
   return next_x;
  }

  unsigned int Resizer::get_next_y(const unsigned int y) const
  {
   unsigned int next_y;
   next_y=y+1;
   if (next_y==source_height)
   {
    --next_y;
   }
   return next_y;
  }

  void Resizer::scale_image(const unsigned int *target)
  {
   size_t index;
   unsigned int x,y,source_x,source_y,next_x,next_y,first,second,third,last,red,green,blue,alpha,x_difference,y_difference,x_weigh,y_weigh;
   index=0;
   for (y=0;y<target_height;++y)
   {
    source_y=this->get_source_y(y);
    next_y=this->get_next_y(source_y);
    y_difference=this->get_y_difference(y);
    y_weigh=UCHAR_MAX-y_difference;
    for (x=0;x<target_width;++x)
    {
     source_x=this->get_source_x(x);
     next_x=this->get_next_x(source_x);
     x_difference=this->get_x_difference(x);
     x_weigh=UCHAR_MAX-x_difference;
     first=target[Core::get_offset(source_x,source_y,source_width)];
     second=target[Core::get_offset(next_x,source_y,source_width)];
     third=target[Core::get_offset(source_x,next_y,source_width)];
     last=target[Core::get_offset(next_x,next_y,source_width)];
     red=(get_pixel_component(first,Core::RED_COMPONENT)*x_weigh*y_weigh+get_pixel_component(second,Core::RED_COMPONENT)*x_difference*y_weigh+get_pixel_component(third,Core::RED_COMPONENT)*y_difference*x_weigh+get_pixel_component(last,Core::RED_COMPONENT)*x_difference*y_difference+1)/normalization;
     green=(get_pixel_component(first,Core::GREEN_COMPONENT)*x_weigh*y_weigh+get_pixel_component(second,Core::GREEN_COMPONENT)*x_difference*y_weigh+get_pixel_component(third,Core::GREEN_COMPONENT)*y_difference*x_weigh+get_pixel_component(last,Core::GREEN_COMPONENT)*x_difference*y_difference+1)/normalization;
     blue=(get_pixel_component(first,Core::BLUE_COMPONENT)*x_weigh*y_weigh+get_pixel_component(second,Core::BLUE_COMPONENT)*x_difference*y_weigh+get_pixel_component(third,Core::BLUE_COMPONENT)*y_difference*x_weigh+get_pixel_component(last,Core::BLUE_COMPONENT)*x_difference*y_difference+1)/normalization;
     alpha=(get_pixel_component(first,Core::ALPHA_COMPONENT)*x_weigh*y_weigh+get_pixel_component(second,Core::ALPHA_COMPONENT)*x_difference*y_weigh+get_pixel_component(third,Core::ALPHA_COMPONENT)*y_difference*x_weigh+get_pixel_component(last,Core::ALPHA_COMPONENT)*x_difference*y_difference+1)/normalization;
     image[index]=Core::make_pixel(red,green,blue,alpha);
     ++index;
    }

   }

  }

  void Resizer::set_setting(const unsigned int width,const unsigned int height)
  {
   source_width=width;
   source_height=height;
  }

  void Resizer::correct_size(const unsigned int limit)
  {
   if (target_width>limit)
   {
    target_width=limit;
   }
   if (target_height>limit)
   {
    target_height=limit;
   }

  }

  void Resizer::calculate_ratio()
  {
   x_ratio=((source_width-1)*UCHAR_MAX)/(target_width-1);
   y_ratio=((source_height-1)*UCHAR_MAX)/(target_height-1);
  }

  void Resizer::calculate_size()
  {
   while (target_width<source_width)
   {
    target_width*=2;
   }
   while (target_height<source_height)
   {
    target_height*=2;
   }

  }

  void Resizer::create_texture()
  {
   size_t length;
   length=static_cast<size_t>(target_width)*static_cast<size_t>(target_height);
   Resource::create(&image,length);
  }

  bool Resizer::is_dont_need_resize() const
  {
   return (source_width==target_width) && (source_height==target_height);
  }

  unsigned int Resizer::get_width() const
  {
   return target_width;
  }

  unsigned int Resizer::get_height() const
  {
   return target_height;
  }

  unsigned int *Resizer::get_buffer()
  {
   return image;
  }

  void Resizer::make_texture(const unsigned int *target,const unsigned int width,const unsigned int height,const unsigned int limit)
  {
   this->set_setting(width,height);
   this->calculate_size();
   this->correct_size(limit);
   if (this->is_dont_need_resize()==false)
   {
    this->calculate_ratio();
    this->create_texture();
    this->scale_image(target);
   }

  }

  FPS::FPS()
  {
   start=time(NULL);
   current=0;
   fps=0;
  }

  FPS::~FPS()
  {

  }

  void FPS::update_counter()
  {
   ++current;
   if (difftime(time(NULL),start)>=1.0)
   {
    fps=current;
    current=0;
    start=time(NULL);
   }

  }

  unsigned int FPS::get_fps_amount() const
  {
   return fps;
  }

  Shape::Shape()
  {
   target_width=0;
   target_height=0;
   total_width=0;
   total_height=0;
   current_x=0;
   current_y=0;
   vertex[0].x=0;
   vertex[0].y=0;
   vertex[1].x=0;
   vertex[1].y=0;
   vertex[2].x=0;
   vertex[2].y=0;
   vertex[3].x=0;
   vertex[3].y=0;
   point[0].u=0.0f;
   point[0].v=1.0f;
   point[1].u=1.0f;
   point[1].v=1.0f;
   point[2].u=1.0f;
   point[2].v=0.0f;
   point[3].u=0.0f;
   point[3].v=0.0f;
  }

  Shape::~Shape()
  {

  }

  void Shape::set_data(const Core::MIRROR_KIND kind)
  {
   switch (kind)
   {
    case Core::HORIZONTAL_MIRROR:
    vertex[0].x=current_x+target_width;
    vertex[0].y=current_y+target_height;
    vertex[1].x=current_x;
    vertex[1].y=current_y+target_height;
    vertex[2].x=current_x;
    vertex[2].y=current_y;
    vertex[3].x=current_x+target_width;
    vertex[3].y=current_y;
    break;
    case Core::VERTICAL_MIRROR:
    vertex[0].x=current_x;
    vertex[0].y=current_y;
    vertex[1].x=current_x+target_width;
    vertex[1].y=current_y;
    vertex[2].x=current_x+target_width;
    vertex[2].y=current_y+target_height;
    vertex[3].x=current_x;
    vertex[3].y=current_y+target_height;
    break;
    case Core::MIRROR_BOTH:
    vertex[0].x=current_x+target_width;
    vertex[0].y=current_y;
    vertex[1].x=current_x;
    vertex[1].y=current_y;
    vertex[2].x=current_x;
    vertex[2].y=current_y+target_height;
    vertex[3].x=current_x+target_width;
    vertex[3].y=current_y+target_height;
    break;
    default:
    vertex[0].x=current_x;
    vertex[0].y=current_y+target_height;
    vertex[1].x=current_x+target_width;
    vertex[1].y=current_y+target_height;
    vertex[2].x=current_x+target_width;
    vertex[2].y=current_y;
    vertex[3].x=current_x;
    vertex[3].y=current_y;
    break;
   }

  }

  unsigned int Shape::get_total_width() const
  {
   return total_width;
  }

  unsigned int Shape::get_total_height() const
  {
   return total_height;
  }

  void Shape::set_total_size(const unsigned int width,const unsigned int height)
  {
   total_width=width;
   total_height=height;
  }

  void Shape::set_size(const unsigned int width,const unsigned int height)
  {
   target_width=width;
   target_height=height;
  }

  void Shape::set_position(const unsigned int x,const unsigned int y)
  {
   current_x=x;
   current_y=y;
  }

  void Shape::set_tile_offset(const float row,const float rows,const float column,const float columns)
  {
   point[0].u=Core::get_start_offset(row,rows);
   point[0].v=Core::get_end_offset(column,columns);
   point[1].u=Core::get_end_offset(row,rows);
   point[1].v=Core::get_end_offset(column,columns);
   point[2].u=Core::get_end_offset(row,rows);
   point[2].v=Core::get_start_offset(column,columns);
   point[3].u=Core::get_start_offset(row,rows);
   point[3].v=Core::get_start_offset(column,columns);
  }

  void Shape::set_horizontal_offset(const float current,const float total)
  {
   this->set_tile_offset(current,total,1.0f,1.0f);
  }

  void Shape::set_vertical_offset(const float current,const float total)
  {
   this->set_tile_offset(1.0f,1.0f,current,total);
  }

  Rectangle::Rectangle()
  {
   texture=0;
  }

  Rectangle::~Rectangle()
  {
   if (texture!=0)
   {
    glDeleteTextures(1,&texture);
    texture=0;
   }

  }

  void Rectangle::load_texture(const unsigned int width,const unsigned int height,const void *buffer)
  {
   glGenTextures(1,&texture);
   glBindTexture(GL_TEXTURE_2D,texture);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
   glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,buffer);
  }

  void Rectangle::create_texture(const unsigned int *buffer)
  {
   Resizer resizer;
   resizer.make_texture(buffer,this->get_total_width(),this->get_total_height(),MAXIMUM_TEXTURE_SIZE);
   if (resizer.is_dont_need_resize()==true)
   {
    this->load_texture(this->get_total_width(),this->get_total_height(),buffer);
   }
   else
   {
    this->load_texture(resizer.get_width(),resizer.get_height(),resizer.get_buffer());
   }

  }

  void Rectangle::check_texture()
  {
   if (glGetError()!=GL_NO_ERROR)
   {
    texture=0;
   }

  }

  void Rectangle::draw_rectangle()
  {
   glVertexPointer(2,GL_INT,0,vertex);
   glTexCoordPointer(2,GL_FLOAT,0,point);
   glBindTexture(GL_TEXTURE_2D,texture);
   glDrawArrays(GL_TRIANGLE_FAN,0,4);
  }

  void Rectangle::enable_transparent()
  {
   if (glIsEnabled(GL_ALPHA_TEST)==GL_FALSE)
   {
    glEnable(GL_ALPHA_TEST);
    glEnable(GL_BLEND);
   }

  }

  void Rectangle::disable_transparent()
  {
   if (glIsEnabled(GL_ALPHA_TEST)==GL_TRUE)
   {
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_BLEND);
   }

  }

  void Rectangle::destroy_texture()
  {
   if (texture!=0)
   {
    glDeleteTextures(1,&texture);
    texture=0;
   }

  }

  void Rectangle::prepare(const unsigned int *buffer)
  {
   if (buffer!=NULL)
   {
    this->destroy_texture();
    this->create_texture(buffer);
    this->check_texture();
   }

  }

  void Rectangle::draw(const Core::MIRROR_KIND kind)
  {
   if (texture!=0)
   {
    this->set_data(kind);
    this->draw_rectangle();
   }

  }

  bool Rectangle::is_texture_exist() const
  {
   return texture!=0;
  }

  Render::Render()
  {

  }

  Render::~Render()
  {

  }

  unsigned int Render::get_maximum_texture_size() const
  {
   int maximum_size;
   glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maximum_size);
   return maximum_size;
  }

  void Render::set_image_setting()
  {
   glPixelStorei(GL_UNPACK_ALIGNMENT,4);
   glPixelStorei(GL_UNPACK_SWAP_BYTES,0);
   glPixelStorei(GL_UNPACK_LSB_FIRST,0);
   glPixelStorei(GL_UNPACK_ROW_LENGTH,0);
   glPixelStorei(GL_UNPACK_SKIP_PIXELS,0);
   glPixelStorei(GL_UNPACK_SKIP_ROWS,0);
   glPixelStorei(GL_PACK_ALIGNMENT,4);
   glPixelStorei(GL_PACK_SWAP_BYTES,0);
   glPixelStorei(GL_PACK_LSB_FIRST,0);
   glPixelStorei(GL_PACK_ROW_LENGTH,0);
   glPixelStorei(GL_PACK_SKIP_PIXELS,0);
   glPixelStorei(GL_PACK_SKIP_ROWS,0);
  }

  void Render::set_perfomance_setting()
  {
   glDisable(GL_POINT_SMOOTH);
   glDisable(GL_LINE_SMOOTH);
   glDisable(GL_POLYGON_SMOOTH);
   glDisable(GL_POLYGON_OFFSET_POINT);
   glDisable(GL_POLYGON_OFFSET_LINE);
   glDisable(GL_POLYGON_OFFSET_FILL);
   glDisable(GL_POLYGON_STIPPLE);
   glDisable(GL_DITHER);
   glDisable(GL_LOGIC_OP);
   glDisable(GL_INDEX_LOGIC_OP);
   glDisable(GL_COLOR_LOGIC_OP);
   glDisable(GL_FOG);
   glDisable(GL_STENCIL_TEST);
   glDisable(GL_SCISSOR_TEST);
   glDisable(GL_LIGHTING);
   glDisable(GL_NORMALIZE);
   glDisable(GL_AUTO_NORMAL);
   glDisable(GL_COLOR_MATERIAL);
   glDisable(GL_TEXTURE_GEN_Q);
   glDisable(GL_TEXTURE_GEN_R);
   glDisable(GL_TEXTURE_GEN_S);
   glDisable(GL_TEXTURE_GEN_T);
   glDisable(GL_TEXTURE_1D);
   glDisable(GL_CULL_FACE);
   glDisable(GL_MAP1_COLOR_4);
   glDisable(GL_MAP1_INDEX);
   glDisable(GL_MAP1_NORMAL);
   glDisable(GL_MAP1_TEXTURE_COORD_1);
   glDisable(GL_MAP1_TEXTURE_COORD_2);
   glDisable(GL_MAP1_TEXTURE_COORD_3);
   glDisable(GL_MAP1_TEXTURE_COORD_4);
   glDisable(GL_MAP1_VERTEX_3);
   glDisable(GL_MAP1_VERTEX_4);
   glDisable(GL_MAP2_COLOR_4);
   glDisable(GL_MAP2_INDEX);
   glDisable(GL_MAP2_NORMAL);
   glDisable(GL_MAP2_TEXTURE_COORD_1);
   glDisable(GL_MAP2_TEXTURE_COORD_2);
   glDisable(GL_MAP2_TEXTURE_COORD_3);
   glDisable(GL_MAP2_TEXTURE_COORD_4);
   glDisable(GL_MAP2_VERTEX_3);
   glDisable(GL_MAP2_VERTEX_4);
   glEnable(GL_TEXTURE_2D);
   glEnable(GL_ALPHA_TEST);
   glEnable(GL_BLEND);
   glEnableClientState(GL_VERTEX_ARRAY);
   glEnableClientState(GL_TEXTURE_COORD_ARRAY);
   glDisableClientState(GL_COLOR_ARRAY);
   glDisableClientState(GL_EDGE_FLAG_ARRAY);
   glDisableClientState(GL_INDEX_ARRAY);
   glDisableClientState(GL_NORMAL_ARRAY);
  }

  void Render::set_render_hints()
  {
   glHint(GL_FOG_HINT,GL_FASTEST);
   glHint(GL_LINE_SMOOTH_HINT,GL_FASTEST);
   glHint(GL_POINT_SMOOTH_HINT,GL_FASTEST);
   glHint(GL_POLYGON_SMOOTH_HINT,GL_FASTEST);
   glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
  }

  void Render::set_common_setting()
  {
   glDrawBuffer(GL_BACK);
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
   glShadeModel(GL_FLAT);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   glAlphaFunc(GL_GREATER,0.6f);
   glClearColor(0.0,0.0,0.0,0.0);
  }

  void Render::disable_depth_buffer()
  {
   glEnable(GL_DEPTH_TEST);
   glClear(GL_DEPTH_BUFFER_BIT);
   glDepthMask(GL_FALSE);
   glDisable(GL_DEPTH_TEST);
  }

  void Render::set_matrix_setting()
  {
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glMatrixMode(GL_TEXTURE);
   glLoadIdentity();
  }

  void Render::set_perspective(const unsigned int width,const unsigned int height)
  {
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0,static_cast<double>(width),static_cast<double>(height),0.0,0.0,1.0);
   glViewport(0,0,width,height);
  }

  void Render::create_render(const unsigned int width,const unsigned int height)
  {
   this->set_image_setting();
   this->set_perspective(width,height);
   this->set_perfomance_setting();
   this->set_render_hints();
   this->set_common_setting();
   this->set_matrix_setting();
   this->disable_depth_buffer();
   MAXIMUM_TEXTURE_SIZE=this->get_maximum_texture_size();
  }

  void Render::clear_stage()
  {
   glClear(GL_COLOR_BUFFER_BIT);
  }

  void Render::start_render(const unsigned int width,const unsigned int height)
  {
   this->create_render(width,height);
   this->clear_stage();
  }

 }

 namespace Input
 {

  Keyboard::Keyboard()
  {
   preversion=NULL;
  }

  Keyboard::~Keyboard()
  {
   Resource::destroy_array(preversion);
   preversion=NULL;
  }

  void Keyboard::prepare()
  {
   size_t index;
   for (index=0;index<KEYBOARD;++index)
   {
    preversion[index]=KEY_RELEASE;
   }

  }

  bool Keyboard::check_state(const unsigned char code,const unsigned char state)
  {
   bool accept;
   accept=false;
   if (preversion!=NULL)
   {
    accept=(Keys[code]==state) && (preversion[code]!=state);
    preversion[code]=Keys[code];
   }
   return accept;
  }

  void Keyboard::initialize()
  {
   if (preversion==NULL)
   {
    Resource::create(&preversion,KEYBOARD);
    this->prepare();
   }

  }

  bool Keyboard::check_hold(const unsigned char code)
  {
   if (preversion!=NULL)
   {
    preversion[code]=Keys[code];
   }
   return Keys[code]==KEY_PRESS;
  }

  bool Keyboard::check_press(const unsigned char code)
  {
   return this->check_state(code,KEY_PRESS);
  }

  bool Keyboard::check_release(const unsigned char code)
  {
   return this->check_state(code,KEY_RELEASE);
  }

  bool Keyboard::is_ready() const
  {
   return preversion!=NULL;
  }

  Mouse::Mouse()
  {
   preversion[BIRD2D::MOUSE_LEFT]=KEY_RELEASE;
   preversion[BIRD2D::MOUSE_RIGHT]=KEY_RELEASE;
   preversion[BIRD2D::MOUSE_MIDDLE]=KEY_RELEASE;
   hidden=None;
  }

  Mouse::~Mouse()
  {
    if  (window!=None)
    {
      if  (hidden!=None)
      {
        XUndefineCursor(display,window);
        XFlush(display);
        XFreeCursor(display,hidden);
      }

    }

  }

  bool Mouse::check_state(const BIRD2D::MOUSE_BUTTON button,const unsigned char state)
  {
   bool accept;
   accept=(Buttons[button]==state) && (preversion[button]!=state);
   preversion[button]=Buttons[button];
   return accept;
  }

  void Mouse::initialize()
  {
    XColor color;
    Pixmap image;
    color.flags=DoRed|DoGreen|DoBlue;
    color.red=0;
    color.green=0;
    color.blue=0;
    color.pad=0;
    color.pixel=0;
    if  (hidden==None)
    {
      if  (window!=None)
      {
        image=XCreateBitmapFromData(display,window,&color.pad,1,1);
      }
      if  (image!=None)
      {
        hidden=XCreatePixmapCursor(display,image,image,&color,&color,0,0);
        XFreePixmap(display,image);
      }

    }

  }

  void Mouse::show()
  {
    if  (window!=None)
    {
      if  (hidden!=None)
      {
        XUndefineCursor(display,window);
        XFlush(display);
      }

    }

  }

  void Mouse::hide()
  {
    if  (window!=None)
    {
      if  (hidden!=None)
      {
        XDefineCursor(display,window,hidden);
        XFlush(display);
      }

    }

  }

  void Mouse::set_position(const unsigned int x,const unsigned int y)
  {
    if  (window!=None)
    {
      XWarpPointer(display,None,window,0,0,0,0,x,y);
      XFlush(display);
    }

  }

  unsigned int Mouse::get_x()
  {
   return mouse_x;
  }

  unsigned int Mouse::get_y()
  {
   return mouse_y;
  }

  bool Mouse::check_hold(const BIRD2D::MOUSE_BUTTON button)
  {
   preversion[button]=Buttons[button];
   return Buttons[button]==KEY_PRESS;
  }

  bool Mouse::check_press(const BIRD2D::MOUSE_BUTTON button)
  {
   return this->check_state(button,KEY_PRESS);
  }

  bool Mouse::check_release(const BIRD2D::MOUSE_BUTTON button)
  {
   return this->check_state(button,KEY_RELEASE);
  }

  Joystick::Joystick()
  {
    current.set_length(0);
    preversion.set_length(0);
    axis.set_length(0);
    device=-1;
  }

  Joystick::~Joystick()
  {
    if  (device!=-1)
    {
      close(device);
    }
    current.destroy_buffer();
    preversion.destroy_buffer();
    axis.destroy_buffer();
  }

  void Joystick::open_device(const char *joystick)
  {
    if  (device==-1)
    {
      device=open(joystick,O_RDONLY|O_NONBLOCK);
    }

  }

  void Joystick::read_configuration()
  {
    char amount;
    amount=0;
    if  (device!=-1)
    {
      if  (ioctl(device,JSIOCGAXES,&amount)==0)
      {
        axis.set_length(static_cast<size_t>(amount));
        axis.create_buffer();
        axis.fill_buffer(0);
      }
      if  (ioctl(device,JSIOCGBUTTONS,&amount)==0)
      {
        current.set_length(static_cast<size_t>(amount));
        current.create_buffer();
        current.fill_buffer(KEY_RELEASE);
        preversion.set_length(static_cast<size_t>(amount));
        preversion.create_buffer();
        preversion.fill_buffer(KEY_RELEASE);
      }

    }

  }

   bool Joystick::check_current_button(const size_t button)
   {
     bool state;
     state=false;
     if  (current.get_length()>0)
     {
       if (button<current.get_length())
       {
         state=current[button]==KEY_PRESS;
         preversion[button]=current[button];
       }

     }
     return state;
   }

   bool Joystick::check_preversion_button(const size_t button)
   {
     bool state;
     state=false;
     if  (preversion.get_length()>0)
     {
       if (button<preversion.get_length())
       {
         state=preversion[button]==KEY_PRESS;
       }

     }
     return state;
   }

  void Joystick::update()
  {
    js_event event;
    if  (device!=-1)
    {
      while (read(device,&event,sizeof(js_event))>0)
      {
        switch (event.type)
        {
          case JS_EVENT_AXIS:
          if  (axis.get_length()>0) axis[event.number]=event.value;
          break;
          case JS_EVENT_BUTTON:
          if  (current.get_length()>0) current[event.number]=event.value;
          break;
          default:
          ;
          break;
        }


      }

    }

  }

  void Joystick::initialize(const char *joystick)
  {
    this->open_device(joystick);
    this->read_configuration();
  }

  bool Joystick::is_ready() const
  {
    return device!=-1;
  }

  bool Joystick::check_button_hold(const size_t button)
  {
   return this->check_current_button(button);
  }

  bool Joystick::check_button_press(const size_t button)
  {
   return (this->check_current_button(button)==true) && (this->check_preversion_button(button)==false);
  }

  bool Joystick::check_button_release(const size_t button)
  {
   return (this->check_current_button(button)==false) && (this->check_preversion_button(button)==true);
  }

  short int Joystick::get_axis(const size_t target)
  {
    short int value;
    value=0;
    if  (axis.get_length()>0)
    {
      if  (target<axis.get_length())
      {
        value=axis[target];
      }

    }
    return value;
  }

  size_t Joystick::get_button_amount() const
  {
    return current.get_length();
  }

  size_t Joystick::get_axes() const
  {
    return axis.get_length();
  }

 }

 namespace File
 {

  Binary_File::Binary_File()
  {
   target=NULL;
  }

  Binary_File::~Binary_File()
  {
   if (target!=NULL)
   {
    fclose(target);
    target=NULL;
   }

  }

  void Binary_File::close()
  {
   if (target!=NULL)
   {
    fclose(target);
    target=NULL;
   }

  }

  void Binary_File::set_position(const long int offset)
  {
   if (target!=NULL)
   {
    fseek(target,offset,SEEK_SET);
   }

  }

  long int Binary_File::get_length()
  {
   long int length;
   length=0;
   if (target!=NULL)
   {
    fseek(target,0,SEEK_END);
    length=ftell(target);
    rewind(target);
   }
   return length;
  }

  long int Binary_File::get_position()
  {
   return (target==NULL) ? 0:ftell(target);
  }

  bool Binary_File::check_error()
  {
   return (target==NULL) ? true:(ferror(target)!=0);
  }

  bool Binary_File::is_open() const
  {
   return target!=NULL;
  }

  Input_File::Input_File()
  {

  }

  Input_File::~Input_File()
  {

  }

  Input_File* Input_File::get_handle()
  {
   return this;
  }

  void Input_File::open(const char *name)
  {
   this->close();
   target=fopen(name,"rb");
  }

  void Input_File::read(void *buffer,const size_t length)
  {
   if (this->target!=NULL)
   {
    if (buffer!=NULL)
    {
     fread(buffer,sizeof(char),length,target);
    }

   }

  }

  Output_File::Output_File()
  {

  }

  Output_File::~Output_File()
  {

  }

  Output_File* Output_File::get_handle()
  {
   return this;
  }

  void Output_File::open(const char *name)
  {
   this->close();
   target=fopen(name,"wb");
  }

  void Output_File::create_temp()
  {
   this->close();
   target=tmpfile();
  }

  void Output_File::write(const void *buffer,const size_t length)
  {
   if (this->target!=NULL)
   {
    if (buffer!=NULL)
    {
     fwrite(buffer,sizeof(char),length,target);
    }

   }

  }

  void Output_File::flush()
  {
   if (target!=NULL)
   {
    fflush(target);
   }

  }

 }

 namespace Misc
 {

  Memory::Memory()
  {
   memset(&information,0,sizeof(struct sysinfo));
  }

   Memory::~Memory()
   {

   }

   void Memory::read_system_information()
   {
    if (sysinfo(&information)==-1)
    {
      memset(&information,0,sizeof(struct sysinfo));
    }

   }

   unsigned long long int Memory::get_total_physical()
   {
    this->read_system_information();
    return information.totalram*information.mem_unit;
   }

   unsigned long long int Memory::get_free_physical()
   {
    this->read_system_information();
    return information.freeram*information.mem_unit;
   }

   unsigned long long int Memory::get_total_virtual()
   {
    this->read_system_information();
    return information.totalswap*information.mem_unit;
   }

   unsigned long long int Memory::get_free_virtual()
   {
    this->read_system_information();
    return information.freeswap*information.mem_unit;
   }

   unsigned long long int Memory::get_physical_usage()
   {
    this->read_system_information();
    return (information.totalram-information.freeram)*information.mem_unit;
   }

   unsigned long long int Memory::get_virtual_usage()
   {
    this->read_system_information();
    return (information.totalswap-information.freeswap)*information.mem_unit;
   }

   Audio::Audio()
   {
     engine=NULL;
     player=NULL;
     media=NULL;
   }

   Audio::~Audio()
   {
     if (player!=NULL)
     {
       libvlc_media_player_stop(player);
       libvlc_media_player_release(player);
       player=NULL;
     }
     if (media!=NULL)
     {
       libvlc_media_release(media);
       media=NULL;
     }
     if (engine!=NULL)
     {
       libvlc_release(engine);
       engine=NULL;
     }

   }

   void Audio::create_engine()
   {
     if (engine==NULL)
     {
       engine=libvlc_new(0,NULL);
     }

   }

   void Audio::create_player()
   {
     if (player==NULL)
     {
       if (engine!=NULL)
       {
         player=libvlc_media_player_new(engine);
       }

     }

   }

   void Audio::destoy_media()
   {
     if (media!=NULL)
     {
       libvlc_media_release(media);
       media=NULL;
     }

   }

   void Audio::load_media(const char *name)
   {
     if (media==NULL)
     {
       if (engine!=NULL)
       {
         media=libvlc_media_new_path(engine,name);
       }

     }

   }

   void Audio::set_media()
   {
     if (player!=NULL)
     {
       if (media!=NULL)
       {
         libvlc_media_player_set_media(player,media);
         libvlc_media_add_option(media,"--no-video");
       }

     }

   }

   void Audio::set_volume()
   {
     if (player!=NULL)
     {
       libvlc_audio_set_volume(player,100);
     }

   }

   void Audio::play_media()
   {
     if (player!=NULL)
     {
       libvlc_media_player_play(player);
     }

   }

   void Audio::initialize()
   {
     this->create_engine();
     this->create_player();
   }

   void Audio::stop()
   {
     if (player!=NULL)
     {
       libvlc_media_player_stop(player);
     }

   }

    void Audio::play()
    {
      this->stop();
      this->play_media();
    }

    void Audio::load(const char *name)
    {
      this->stop();
      this->destoy_media();
      this->load_media(name);
      this->set_media();
      this->set_volume();
    }

    bool Audio::check_playing()
    {
      bool playing;
      playing=false;
      if (player!=NULL)
      {
        playing=libvlc_media_player_is_playing(player);
      }
      return playing;
    }

    void Audio::play_loop()
    {
      if (this->check_playing()==false)
      {
        this->play();
      }

    }

    void Audio::play(const bool loop)
    {
      if (loop==true)
      {
        this->play_loop();
      }
      else
      {
        this->play();
      }

    }

   void Audio::initialize(const char *name)
   {
     this->initialize();
     this->load(name);
   }

 }

 namespace Graphics
 {

  Surface::Surface()
  {

  }

  Surface::~Surface()
  {

  }

  void Surface::screen_setup()
  {
   this->prepare_engine();
   this->start_render(this->get_display_width(),this->get_display_height());
   this->set_timer(17);
  }

  void Surface::clear_screen()
  {
   if (this->get_context()!=NULL)
   {
    this->clear_stage();
   }

  }

  void Surface::initialize()
  {
   if (this->get_context()==NULL)
   {
    this->screen_setup();
   }

  }

  bool Surface::update()
  {
   bool run;
   run=false;
   if (this->get_context()!=NULL)
   {
    this->Swap();
    this->update_counter();
    this->clear_stage();
    run=this->process_message();
   }
   return run;
  }

  bool Surface::sync()
  {
   this->wait_timer();
   return this->update();
  }

  bool Surface::sync(const bool limit)
  {
   if (limit==true)
   {
    this->wait_timer();
   }
   return this->update();
  }

  bool Surface::is_ready()
  {
   return this->get_context()!=NULL;
  }

  unsigned int Surface::get_color() const
  {
   return this->get_depth();
  }

  unsigned int Surface::get_fps() const
  {
   return this->get_fps_amount();
  }

  unsigned int Surface::get_width() const
  {
   return this->get_display_width();
  }

  unsigned int Surface::get_height() const
  {
   return this->get_display_height();
  }

  bool Surface::check_x(const unsigned int x) const
  {
   return x<this->get_width();
  }

  bool Surface::check_y(const unsigned int y) const
  {
   return y<this->get_height();
  }

  Surface* Surface::get_handle()
  {
   return this;
  }

  Image::Image()
  {
   data=NULL;
   width=0;
   height=0;
   length=0;
  }

  Image::~Image()
  {
   Resource::destroy_array(data);
   data=NULL;
  }

  size_t Image::get_source_position(const unsigned int x,const unsigned int y,const Core::MIRROR_KIND mirror) const
  {
   size_t position;
   switch (mirror)
   {
    case Core::HORIZONTAL_MIRROR:
    position=Core::get_offset(width-x-1,y,width);
    break;
    case Core::VERTICAL_MIRROR:
    position=Core::get_offset(x,height-y-1,width);
    break;
    case Core::MIRROR_BOTH:
    position=Core::get_offset(width-x-1,height-y-1,width);
    break;
    default:
    position=Core::get_offset(x,y,width);
    break;
   }
   return position*3;
  }

  void Image::set_size(const unsigned int image_width,const unsigned int image_height)
  {
   width=image_width;
   height=image_height;
   length=static_cast<size_t>(width)*static_cast<size_t>(height)*3;
  }

  void Image::mirror_image(const Core::MIRROR_KIND mirror)
  {
   unsigned char *mirrored;
   unsigned int x,y;
   size_t index,position;
   index=0;
   position=0;
   mirrored=NULL;
   Resource::create(&mirrored,length);
   for (y=0;y<height;++y)
   {
    for (x=0;x<width;++x)
    {
     position=this->get_source_position(x,y,mirror);
     mirrored[index]=data[position];
     mirrored[index+1]=data[position+1];
     mirrored[index+2]=data[position+2];
     index+=3;
    }

   }
   Resource::destroy_array(data);
   data=mirrored;
  }

  void Image::uncompress_tga_data(const unsigned char *source)
  {
   size_t index,position,amount;
   index=0;
   position=0;
   while (index<length)
   {
    if (source[position]<128)
    {
     for (amount=source[position]+1;amount>0;--amount)
     {
      data[index]=source[position+1];
      data[index+1]=source[position+2];
      data[index+2]=source[position+3];
      index+=3;
      position+=3;
     }
     ++position;
    }
    else
    {
     for (amount=source[position]-127;amount>0;--amount)
     {
      data[index]=source[position+1];
      data[index+1]=source[position+2];
      data[index+2]=source[position+3];
      index+=3;
     }
     position+=sizeof(unsigned int);
    }

   }

  }

  void Image::mirror_tga(const unsigned char descriptor)
  {
   switch (descriptor)
   {
    case 0x00: // Bottom-left
    this->mirror_image(Core::VERTICAL_MIRROR);
    break;
    case 0x30: // Top-right
    this->mirror_image(Core::HORIZONTAL_MIRROR);
    break;
    case 0x10: // Bottom-right
    this->mirror_image(Core::MIRROR_BOTH);
    break;
    default: // Top-left
    ;
    break;
   }

  }

  void Image::load_tga(File::Input_File &target)
  {
   unsigned char *buffer;
   size_t compressed_length;
   TGA_head head;
   TGA_map color_map;
   TGA_image image;
   buffer=NULL;
   compressed_length=static_cast<size_t>(target.get_length()-18);
   target.read(&head,3);
   target.read(&color_map,5);
   target.read(&image,10);
   if (image.color==24)
   {
    this->set_size(image.width,image.height);
    Resource::create(&data,length);
    switch (head.type)
    {
     case 2:
     target.read(data,length);
     break;
     case 10:
     Resource::create(&buffer,compressed_length);
     target.read(buffer,compressed_length);
     this->uncompress_tga_data(buffer);
     Resource::destroy_array(buffer);
     buffer=NULL;
     break;
     default:
     this->destroy_image();
     break;
    }
    this->mirror_tga(image.descriptor);
   }

  }

  void Image::destroy_image()
  {
   Resource::destroy_array(data);
   data=NULL;
   this->set_size(0,0);
  }

  size_t Image::get_length() const
  {
   return length;
  }

  unsigned int Image::get_width() const
  {
   return width;
  }

  unsigned int Image::get_height() const
  {
   return height;
  }

  unsigned char *Image::get_data()
  {
   return data;
  }

  unsigned char *Image::load(const char *name)
  {
   File::Input_File target;
   if (name!=NULL)
   {
    this->destroy_image();
    target.open(name);
   }
   if (target.is_open()==true)
   {
    this->load_tga(target);
    target.close();
   }
   return data;
  }

  Image* Image::get_handle()
  {
   return this;
  }

  Picture::Picture()
  {
   image=NULL;
   image_width=0;
   image_height=0;
   pixels=0;
  }

  Picture::~Picture()
  {
   Resource::destroy_array(image);
   image=NULL;
  }

  void Picture::set_image_size(const unsigned int width,const unsigned int height)
  {
   image_width=width;
   image_height=height;
   pixels=static_cast<size_t>(width)*static_cast<size_t>(height);
  }

  void Picture::create_storage()
  {
   Resource::create(&image,pixels);
  }

  void Picture::copy_image(const unsigned int *target)
  {
   size_t index;
   for (index=0;index<pixels;++index)
   {
    image[index]=target[index];
   }

  }

  void Picture::convert_image(const unsigned char *target)
  {
   size_t index,position;
   position=0;
   for (index=0;index<pixels;++index)
   {
    image[index]=Core::make_pixel(target[position+2],target[position+1],target[position],0);
    if (image[0]!=image[index])
    {
     image[index]=Core::make_pixel(target[position+2],target[position+1],target[position],UCHAR_MAX);
    }
    position+=3;
   }

  }

  void Picture::load_image(Image *buffer)
  {
   if (buffer!=NULL)
   {
    if (buffer->get_length()>0)
    {
     this->destroy_image();
     this->set_image_size(buffer->get_width(),buffer->get_height());
     this->create_storage();
     this->convert_image(buffer->get_data());
    }

   }

  }

  void Picture::destroy_image()
  {
   Resource::destroy_array(image);
   image=NULL;
   this->set_image_size(0,0);
  }

  bool Picture::is_storage_empty() const
  {
   return pixels==0;
  }

  unsigned int Picture::get_image_width() const
  {
   return image_width;
  }

  unsigned int Picture::get_image_height() const
  {
   return image_height;
  }

  size_t Picture::get_pixel_amount() const
  {
   return pixels;
  }

  size_t Picture::get_image_length() const
  {
   return pixels*sizeof(unsigned int);
  }

  unsigned int *Picture::get_image()
  {
   return image;
  }

  Animation::Animation()
  {
   frame=1;
   frames=1;
  }

  Animation::~Animation()
  {

  }

  void Animation::reset_animation_setting()
  {
   frame=1;
   frames=1;
  }

  void Animation::increase_frame()
  {
   ++frame;
   if (frame>frames)
   {
    frame=1;
   }

  }

  void Animation::set_frame(const unsigned int target)
  {
   if (this->check_frame(target)==true)
   {
    frame=target;
   }

  }

  void Animation::set_frames(const unsigned int amount)
  {
   if (amount>0)
   {
    frames=amount;
   }

  }

  unsigned int Animation::get_frames() const
  {
   return frames;
  }

  unsigned int Animation::get_frame() const
  {
   return frame;
  }

  bool Animation::is_last_frame() const
  {
   return frame==frames;
  }

  bool Animation::check_frame(const unsigned int target) const
  {
   return (target>0) && (target<=frames);
  }

  Billboard::Billboard()
  {
   billboard.set_size(0,0);
   transparent=true;
   current_x=0;
   current_y=0;
   start_x=0;
   start_y=0;
   sprite_width=0;
   sprite_height=0;
   mirror=Core::MIRROR_NONE;
  }

  Billboard::~Billboard()
  {
   billboard.destroy_texture();
  }

  void Billboard::check_transparent()
  {
   if (transparent==true)
   {
    billboard.enable_transparent();
   }
   else
   {
    billboard.disable_transparent();
   }

  }

  void Billboard::draw_sprite_image()
  {
   billboard.set_size(sprite_width,sprite_height);
   billboard.set_position(current_x,current_y);
   billboard.draw(mirror);
  }

  void Billboard::reset_billboard_setting()
  {
   transparent=true;
   current_x=0;
   current_y=0;
   start_x=0;
   start_y=0;
   sprite_width=0;
   sprite_height=0;
   mirror=Core::MIRROR_NONE;
  }

  void Billboard::prepare(const unsigned int width,const unsigned int height,const unsigned int *picture)
  {
   billboard.set_total_size(width,height);
   billboard.prepare(picture);
  }

  void Billboard::set_transparent(const bool enabled)
  {
   transparent=enabled;
  }

  bool Billboard::get_transparent() const
  {
   return transparent;
  }

  void Billboard::set_width(const unsigned int width)
  {
   if (billboard.is_texture_exist()==true)
   {
    if (width>0)
    {
     sprite_width=width;
    }

   }

  }

  void Billboard::set_height(const unsigned int height)
  {
   if (billboard.is_texture_exist()==true)
   {
    if (height>0)
    {
     sprite_height=height;
    }

   }

  }

  void Billboard::set_size(const unsigned int width,const unsigned int height)
  {
   this->set_width(width);
   this->set_height(height);
  }

  void Billboard::set_start(const unsigned int x,const unsigned int y)
  {
   if (billboard.is_texture_exist()==true)
   {
    start_x=x;
    start_y=y;
   }

  }

  void Billboard::set_position(const unsigned int x,const unsigned int y)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_x=x;
    current_y=y;
   }

  }

  void Billboard::set_x(const unsigned int x)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_x=x;
   }

  }

  void Billboard::set_y(const unsigned int y)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_y=y;
   }

  }

  unsigned int Billboard::increase_x()
  {
   if (billboard.is_texture_exist()==true)
   {
    ++current_x;
   }
   return current_x;
  }

  unsigned int Billboard::decrease_x()
  {
   if (billboard.is_texture_exist()==true)
   {
    --current_x;
   }
   return current_x;
  }

  unsigned int Billboard::increase_y()
  {
   if (billboard.is_texture_exist()==true)
   {
    ++current_y;
   }
   return current_y;
  }

  unsigned int Billboard::decrease_y()
  {
   if (billboard.is_texture_exist()==true)
   {
    --current_y;
   }
   return current_y;
  }

  unsigned int Billboard::increase_x(const unsigned int increment)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_x+=increment;
   }
   return current_x;
  }

  unsigned int Billboard::decrease_x(const unsigned int decrement)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_x-=decrement;
   }
   return current_x;
  }

  unsigned int Billboard::increase_y(const unsigned int increment)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_y+=increment;
   }
   return current_y;
  }

  unsigned int Billboard::decrease_y(const unsigned int decrement)
  {
   if (billboard.is_texture_exist()==true)
   {
    current_y-=decrement;
   }
   return current_y;
  }

  unsigned int Billboard::get_start_x() const
  {
   return start_x;
  }

  unsigned int Billboard::get_start_y() const
  {
   return start_y;
  }

  unsigned int Billboard::get_x() const
  {
   return current_x;
  }

  unsigned int Billboard::get_y() const
  {
   return current_y;
  }

  unsigned int Billboard::get_width() const
  {
   return sprite_width;
  }

  unsigned int Billboard::get_height() const
  {
   return sprite_height;
  }

  bool Billboard::is_load() const
  {
   return billboard.is_texture_exist();
  }

  BIRD2D::BOX Billboard::get_box() const
  {
   BIRD2D::BOX collision;
   collision.x=current_x;
   collision.y=current_y;
   collision.width=sprite_width;
   collision.height=sprite_height;
   return collision;
  }

  void Billboard::disable_mirror()
  {
   mirror=Core::MIRROR_NONE;
  }

  void Billboard::horizontal_mirror()
  {
   mirror=Core::HORIZONTAL_MIRROR;
  }

  void Billboard::vertical_mirror()
  {
   mirror=Core::VERTICAL_MIRROR;
  }

  void Billboard::complex_mirror()
  {
   mirror=Core::MIRROR_BOTH;
  }

  void Billboard::go_start()
  {
   this->set_position(start_x,start_y);
  }

  void Billboard::go_start_x()
  {
   this->set_x(start_x);
  }

  void Billboard::go_start_y()
  {
   this->set_y(start_y);
  }

  void Billboard::draw()
  {
   this->check_transparent();
   this->draw_sprite_image();
  }

  void Billboard::draw(const unsigned int x,const unsigned int y)
  {
   this->set_position(x,y);
   this->draw();
  }

  void Billboard::draw(const bool transparency)
  {
   this->set_transparent(transparency);
   this->draw();
  }

  void Billboard::draw(const bool transparency,const unsigned int x,const unsigned int y)
  {
   this->set_transparent(transparency);
   this->draw(x,y);
  }

  Sprite::Sprite()
  {
   current_kind=BIRD2D::HORIZONTAL_ANIMATED;
  }

  Sprite::~Sprite()
  {

  }

  void Sprite::reset_sprite_setting()
  {
   current_kind=BIRD2D::HORIZONTAL_ANIMATED;
  }

  void Sprite::set_sprite_setting()
  {
   if (current_kind==BIRD2D::HORIZONTAL_ANIMATED)
   {
    this->set_size(this->get_image_width()/this->get_frames(),this->get_image_height());
   }
   else
   {
    this->set_size(this->get_image_width(),this->get_image_height()/this->get_frames());
   }

  }

  void Sprite::configure_sprite()
  {
   if (this->is_storage_empty()==false)
   {
    this->set_sprite_setting();
   }

  }

  void Sprite::set_sprite_frame()
  {
   if (current_kind==BIRD2D::HORIZONTAL_ANIMATED)
   {
    billboard.set_horizontal_offset(static_cast<float>(this->get_frame()),static_cast<float>(this->get_frames()));
   }
   else
   {
    billboard.set_vertical_offset(static_cast<float>(this->get_frame()),static_cast<float>(this->get_frames()));
   }

  }

  void Sprite::set_kind(const BIRD2D::IMAGE_KIND kind)
  {
   current_kind=kind;
   this->configure_sprite();
   this->set_sprite_frame();
  }

  Sprite* Sprite::get_handle()
  {
   return this;
  }

  BIRD2D::IMAGE_KIND Sprite::get_kind() const
  {
   return current_kind;
  }

  void Sprite::set_settings(const BIRD2D::IMAGE_KIND kind,const unsigned int frames)
  {
   this->reset_animation_setting();
   this->set_frames(frames);
   this->set_kind(kind);
  }

  void Sprite::load(Image *buffer,const BIRD2D::IMAGE_KIND kind,const unsigned int frames)
  {
   this->load_image(buffer);
   if (this->is_storage_empty()==false)
   {
    this->prepare(this->get_image_width(),this->get_image_height(),this->get_image());
    this->set_settings(kind,frames);
   }

  }

  void Sprite::load(Image &buffer,const BIRD2D::IMAGE_KIND kind,const unsigned int frames)
  {
   this->load(buffer.get_handle(),kind,frames);
  }

  void Sprite::load(const char *name,const BIRD2D::IMAGE_KIND kind,const unsigned int frames)
  {
   Image picture;
   picture.load(name);
   this->load(picture,kind,frames);
   picture.destroy_image();
  }

  void Sprite::set_target(const unsigned int target)
  {
   this->set_frame(target);
   this->set_sprite_frame();
  }

  void Sprite::step()
  {
   this->increase_frame();
   this->set_sprite_frame();
  }

  void Sprite::destroy()
  {
   billboard.destroy_texture();
   this->destroy_image();
   this->reset_billboard_setting();
   this->reset_animation_setting();
   this->reset_sprite_setting();
  }

  void Sprite::clone(Sprite *target)
  {
   if (target!=NULL)
   {
    if (target->get_image_length()>0)
    {
     this->destroy();
     this->set_image_size(target->get_image_width(),target->get_image_height());
     this->create_storage();
     this->set_settings(target->get_kind(),target->get_frames());
     this->set_transparent(target->get_transparent());
     this->copy_image(target->get_image());
     this->prepare(this->get_image_width(),this->get_image_height(),this->get_image());
     this->set_size(target->get_width(),target->get_height());
    }

   }

  }

  void Sprite::clone(Sprite &target)
  {
   this->clone(target.get_handle());
  }

  Cartoon::Cartoon()
  {

  }

  Cartoon::~Cartoon()
  {

  }

  Cartoon* Cartoon::get_handle()
  {
   return this;
  }

  void Cartoon::load(Image *buffer)
  {
   this->load_image(buffer);
   if (this->is_storage_empty()==false)
   {
    this->reset_billboard_setting();
    this->prepare(this->get_image_width(),this->get_image_height(),this->get_image());
    this->set_size(this->get_image_width(),this->get_image_height());
   }

  }

  void Cartoon::load(Image &buffer)
  {
   this->load(buffer.get_handle());
  }

  void Cartoon::load(const char *name)
  {
   Image picture;
   picture.load(name);
   this->load(picture);
   picture.destroy_image();
  }

  void Cartoon::destroy()
  {
   billboard.destroy_texture();
   this->destroy_image();
   this->reset_billboard_setting();
  }

  void Cartoon::clone(Cartoon *target)
  {
   if (target!=NULL)
   {
    if (target->get_image_length()>0)
    {
     this->destroy();
     this->set_image_size(target->get_image_width(),target->get_image_height());
     this->create_storage();
     this->set_transparent(target->get_transparent());
     this->copy_image(target->get_image());
     this->prepare(this->get_image_width(),this->get_image_height(),this->get_image());
     this->set_size(target->get_width(),target->get_height());
    }

   }

  }

  void Cartoon::clone(Cartoon &target)
  {
   this->clone(target.get_handle());
  }

  Sheet::Sheet()
  {
   rows=1;
   columns=1;
  }

  Sheet::~Sheet()
  {

  }

  void Sheet::reset_sheet_setting()
  {
   rows=1;
   columns=1;
  }

  void Sheet::prepare_sheet()
  {
   this->prepare(this->get_image_width(),this->get_image_height(),this->get_image());
   this->set_size(this->get_image_width()/rows,this->get_image_height()/columns);
  }

  Sheet* Sheet::get_handle()
  {
   return this;
  }

  bool Sheet::check_row(const unsigned int target) const
  {
   return (target>0) && (target<=rows);
  }

  bool Sheet::check_column(const unsigned int target) const
  {
   return (target>0) && (target<=columns);
  }

  unsigned int Sheet::get_row(const unsigned int target) const
  {
   unsigned int row;
   row=1;
   if (this->check_frame(target)==true)
   {
    row=target%rows;
   }
   if (row==0)
   {
    row=rows;
   }
   return row;
  }

  unsigned int Sheet::get_column(const unsigned int target) const
  {
   unsigned int column;
   column=1;
   if (target>rows)
   {
    if (this->check_frame(target)==true)
    {
     column+=(target-1)/rows;
    }

   }
   return column;
  }

  unsigned int Sheet::calculate(const unsigned int row,const unsigned int column) const
  {
   unsigned int target;
   target=1;
   if (this->check_row(row)==true)
   {
    if (this->check_column(column)==true)
    {
     target=row+column*rows;
    }

   }
   return target;
  }

  unsigned int Sheet::get_rows() const
  {
   return rows;
  }

  unsigned int Sheet::get_columns() const
  {
   return columns;
  }

  void Sheet::destroy()
  {
   billboard.destroy_texture();
   this->destroy_image();
   this->reset_billboard_setting();
   this->reset_animation_setting();
   this->reset_sheet_setting();
  }

  void Sheet::clone(Sheet *target)
  {
   if (target!=NULL)
   {
    if (target->get_image_length()>0)
    {
     this->destroy();
     this->set_image_size(target->get_image_width(),target->get_image_height());
     this->create_storage();
     this->copy_image(target->get_image());
     rows=target->get_rows();
     columns=target->get_columns();
     this->set_frames(rows*columns);
     this->prepare_sheet();
     this->set_transparent(target->get_transparent());
     this->set_target(target->get_frame());
     this->set_size(target->get_width(),target->get_height());
    }

   }

  }

  void Sheet::clone(Sheet &target)
  {
   this->clone(target.get_handle());
  }

  void Sheet::select(const unsigned int row,const unsigned int column)
  {
   if (this->check_row(row)==true)
   {
    if (this->check_column(column)==true)
    {
     billboard.set_tile_offset(static_cast<float>(row),static_cast<float>(rows),static_cast<float>(column),static_cast<float>(columns));
    }

   }

  }

  void Sheet::set_target(const unsigned int target)
  {
   this->set_frame(target);
   this->select(this->get_row(this->get_frame()),this->get_column(this->get_frame()));
  }

  void Sheet::step()
  {
   this->increase_frame();
   this->select(this->get_row(this->get_frame()),this->get_column(this->get_frame()));
  }

  void Sheet::load(Image *sheet,const unsigned int row_amount,const unsigned int column_amount)
  {
   if (row_amount>0)
   {
    if (column_amount>0)
    {
     this->load_image(sheet);
     if (this->is_storage_empty()==false)
     {
      rows=row_amount;
      columns=column_amount;
      this->reset_animation_setting();
      this->set_frames(rows*columns);
      this->set_target(1);
      this->prepare_sheet();
     }

    }

   }

  }

  void Sheet::load(Image &sheet,const unsigned int row_amount,const unsigned int column_amount)
  {
   this->load(sheet.get_handle(),row_amount,column_amount);
  }

  void Sheet::load(const char *name,const unsigned int row_amount,const unsigned int column_amount)
  {
   Image picture;
   picture.load(name);
   this->load(picture,row_amount,column_amount);
   picture.destroy_image();
  }

  Background::Background()
  {
   stage.set_position(0,0);
  }

  Background::~Background()
  {
   stage.destroy();
  }

  Background* Background::get_handle()
  {
   return this;
  }

  void Background::prepare(const Surface *surface)
  {
   if (surface!=NULL)
   {
    stage.set_size(surface->get_width(),surface->get_height());
   }

  }

  void Background::prepare(const unsigned int width,const unsigned int height)
  {
   stage.set_size(width,height);
  }

  void Background::prepare(Surface &surface)
  {
   this->prepare(surface.get_handle());
  }

  void Background::set_settings(const BIRD2D::IMAGE_KIND kind,const unsigned int frames)
  {
   stage.set_settings(kind,frames);
  }

  void Background::load(Image *background,const BIRD2D::IMAGE_KIND kind,const unsigned int frames)
  {
   stage.load(background,kind,frames);
  }

  void Background::load(Image &background,const BIRD2D::IMAGE_KIND kind,const unsigned int frames)
  {
   stage.load(background,kind,frames);
  }

  void Background::load(const char *name,const BIRD2D::IMAGE_KIND kind,const unsigned int frames)
  {
   stage.load(name,kind,frames);
  }

  void Background::disable_mirror()
  {
   stage.disable_mirror();
  }

  void Background::horizontal_mirror()
  {
   stage.horizontal_mirror();
  }

  void Background::vertical_mirror()
  {
   stage.vertical_mirror();
  }

  void Background::complex_mirror()
  {
   stage.complex_mirror();
  }

  void Background::set_target(const unsigned int target)
  {
   stage.set_target(target);
  }

  void Background::step()
  {
   stage.step();
  }

  void Background::draw()
  {
   stage.draw(false);
  }

  void Background::draw(const unsigned int target)
  {
   stage.set_target(target);
   stage.draw(false);
  }

  void Background::destroy_image()
  {
   stage.destroy_image();
  }

  void Background::destroy()
  {
   stage.destroy();
  }

  bool Background::is_load() const
  {
   return stage.is_load();
  }

  bool Background::is_last_frame() const
  {
   return stage.is_last_frame();
  }

  bool Background::check_frame(const unsigned int target) const
  {
   return stage.check_frame(target);
  }

  unsigned int Background::get_frame() const
  {
   return stage.get_frame();
  }

  unsigned int Background::get_frames() const
  {
   return stage.get_frames();
  }

  unsigned int Background::get_width() const
  {
   return stage.get_width();
  }

  unsigned int Background::get_height() const
  {
   return stage.get_height();
  }

  BIRD2D::IMAGE_KIND Background::get_kind() const
  {
   return stage.get_kind();
  }

  Scene::Scene()
  {
   stage.set_position(0,0);
  }

  Scene::~Scene()
  {
   stage.destroy();
  }

  Scene* Scene::get_handle()
  {
   return this;
  }

  void Scene::prepare(const Surface *surface)
  {
   if (surface!=NULL)
   {
    stage.set_size(surface->get_width(),surface->get_height());
   }

  }

  void Scene::prepare(const unsigned int width,const unsigned int height)
  {
   stage.set_size(width,height);
  }

  void Scene::prepare(Surface &surface)
  {
   this->prepare(surface.get_handle());
  }

  void Scene::load(Image *background)
  {
   stage.load(background);
  }

  void Scene::load(Image &background)
  {
   stage.load(background);
  }

  void Scene::load(const char *name)
  {
   stage.load(name);
  }

  void Scene::disable_mirror()
  {
   stage.disable_mirror();
  }

  void Scene::horizontal_mirror()
  {
   stage.horizontal_mirror();
  }

  void Scene::vertical_mirror()
  {
   stage.vertical_mirror();
  }

  void Scene::complex_mirror()
  {
   stage.complex_mirror();
  }

  void Scene::draw()
  {
   stage.draw(false);
  }

  void Scene::destroy_image()
  {
   stage.destroy_image();
  }

  void Scene::destroy()
  {
   stage.destroy();
  }

  bool Scene::is_load() const
  {
   return stage.is_load();
  }

  unsigned int Scene::get_width() const
  {
   return stage.get_width();
  }

  unsigned int Scene::get_height() const
  {
   return stage.get_height();
  }

  Text::Text()
  {
   text.set_size(0,0);
   orientation=BIRD2D::HORIZONTAL_TEXT;
  }

  Text::~Text()
  {
   text.destroy();
  }

  Text* Text::get_handle()
  {
   return this;
  }

  void Text::increase_position()
  {
   if (orientation==BIRD2D::HORIZONTAL_TEXT)
   {
    text.increase_x(text.get_width());
   }
   else
   {
    text.increase_y(text.get_height());
   }

  }

  void Text::restore_position()
  {
   text.go_start();
  }

  BIRD2D::TEXT_KIND Text::get_orientation() const
  {
   return orientation;
  }

  void Text::set_orientation(const BIRD2D::TEXT_KIND target)
  {
   orientation=target;
  }

  bool Text::is_font_load() const
  {
   return text.is_load();
  }

  unsigned int Text::get_font_width() const
  {
   return text.get_width();
  }

  unsigned int Text::get_font_height() const
  {
   return text.get_height();
  }

  unsigned int Text::get_x() const
  {
   return text.get_start_x();
  }

  unsigned int Text::get_y() const
  {
   return text.get_start_y();
  }

  void Text::set_position(const unsigned int x,const unsigned int y)
  {
   text.set_start(x,y);
   text.set_position(x,y);
  }

  void Text::set_size(const unsigned int width,const unsigned int height)
  {
   text.set_size(width,height);
  }

  void Text::set_width(const unsigned int width)
  {
   text.set_width(width);
  }

  void Text::set_height(const unsigned int height)
  {
   text.set_height(height);
  }

  void Text::set_settings(const unsigned int width,const unsigned int height,const BIRD2D::TEXT_KIND kind)
  {
   this->set_size(width,height);
   this->set_orientation(kind);
  }

  void Text::load_font(Image *font)
  {
   text.load(font,16,16);
  }

  void Text::load_font(Image &font)
  {
   text.load(font,16,16);
  }

  void Text::load_font(const char *name)
  {
   text.load(name,16,16);
  }

  void Text::print(const char target)
  {
   text.set_target(static_cast<unsigned char>(target)+1);
   text.draw(true);
  }

  void Text::print(const unsigned int x,const unsigned int y,const char target)
  {
   this->set_position(x,y);
   this->print(target);
  }

  size_t Text::print(const char *target)
  {
   size_t index,length;
   length=strlen(target);
   this->restore_position();
   for (index=0;index<length;++index)
   {
    this->print(target[index]);
    this->increase_position();
   }
   return length;
  }

  size_t Text::print(const unsigned int x,const unsigned int y,const char *target)
  {
   this->set_position(x,y);
   return this->print(target);
  }

  void Text::disable_mirror()
  {
   text.disable_mirror();
  }

  void Text::horizontal_mirror()
  {
   text.horizontal_mirror();
  }

  void Text::vertical_mirror()
  {
   text.vertical_mirror();
  }

  void Text::complex_mirror()
  {
   text.complex_mirror();
  }

  void Text::destroy_image()
  {
   text.destroy_image();
  }

  void Text::destroy_font()
  {
   text.destroy();
  }

 }

 namespace Transformation
 {

  Coordinates::Coordinates()
  {
   half_viewport_width=1;
   half_viewport_height=1;
  }

  Coordinates::~Coordinates()
  {

  }

  Coordinates* Coordinates::get_handle()
  {
   return this;
  }

  void Coordinates::initialize(const int viewport_width,const int viewport_height)
  {
   if (viewport_width>1)
   {
    half_viewport_width=viewport_width/2;
   }
   if (viewport_height>1)
   {
    half_viewport_height=viewport_height/2;
   }

  }

  int Coordinates::get_viewport_width() const
  {
   return 2*half_viewport_width;
  }

  int Coordinates::get_viewport_height() const
  {
   return 2*half_viewport_height;
  }

  int Coordinates::get_lowest_cartesian_x() const
  {
   return -1*half_viewport_width;
  }

  int Coordinates::get_highest_cartesian_x() const
  {
   return half_viewport_width;
  }

  int Coordinates::get_lowest_cartesian_y() const
  {
   return -1*half_viewport_height;
  }

  int Coordinates::get_highest_cartesian_y() const
  {
   return half_viewport_height;
  }

  bool Coordinates::check_cartesian_x(const int x) const
  {
   return (x>=this->get_lowest_cartesian_x()) && (x<=this->get_highest_cartesian_x());
  }

  bool Coordinates::check_cartesian_y(const int y) const
  {
   return (y>=this->get_lowest_cartesian_y()) && (y<=this->get_highest_cartesian_y());
  }

  int Coordinates::get_cartesian_x(const int x) const
  {
   return (x<0) ? half_viewport_width:(x-half_viewport_width);
  }

  int Coordinates::get_cartesian_y(const int y) const
  {
   return (y<0) ? half_viewport_height:(half_viewport_height-y);
  }

  int Coordinates::get_screen_x(const int x) const
  {
   return x+half_viewport_width;
  }

  int Coordinates::get_screen_y(const int y) const
  {
   return half_viewport_height-y;
  }

  int Isometric::get_isometric_x(const int x,const int y)
  {
   return x-y;
  }

  int Isometric::get_isometric_y(const int x,const int y)
  {
   int isometric_y;
   isometric_y=0;
   if ((x!=0)||(y!=0))
   {
    isometric_y=(x+y)/2;
   }
   return isometric_y;
  }

  int Isometric::get_cartesian_x(const int x,const int y)
  {
   int cartesian_x;
   cartesian_x=0;
   if ((x!=0)||(y!=0))
   {
    cartesian_x=(2*y+x)/2;
   }
   return cartesian_x;
  }

  int Isometric::get_cartesian_y(const int x,const int y)
  {
   int cartesian_y;
   cartesian_y=0;
   if ((x!=0)||(y!=0))
   {
    cartesian_y=(2*y-x)/2;
   }
   return cartesian_y;
  }

  Level::Level()
  {
   start_x=0;
   start_y=0;
   halft_tile_width=0;
   halft_tile_height=0;
  }

  Level::~Level()
  {

  }

  Level* Level::get_handle()
  {
   return this;
  }

  void Level::initialize(const int tile_width,const int tile_height)
  {
   if (tile_width>0)
   {
    halft_tile_width=tile_width/2;
   }
   if (tile_height>0)
   {
    halft_tile_height=tile_height/2;
   }

  }

  void Level::set_offset(const int x_offset,const int y_offset)
  {
   start_x=x_offset;
   start_y=y_offset;
  }

  int Level::get_x_offset() const
  {
   return start_x;
  }

  int Level::get_y_offset() const
  {
   return start_y;
  }

  int Level::get_x(const int row,const int column) const
  {
   return start_x+(row-column)*halft_tile_width;
  }

  int Level::get_y(const int row,const int column) const
  {
   return start_y+(row+column)*halft_tile_height;
  }

 }

 namespace Map
 {

  Tilemap::Tilemap()
  {
   cell_width=1;
   cell_height=1;
  }

  Tilemap::~Tilemap()
  {

  }

  Tilemap* Tilemap::get_handle()
  {
   return this;
  }

  void Tilemap::initialize(const unsigned int tile_width,const unsigned int tile_height)
  {
   if (tile_width>0)
   {
    cell_width=tile_width;
   }
   if (tile_height>0)
   {
    cell_height=tile_height;
   }

  }

  unsigned int Tilemap::get_tile_width() const
  {
   return cell_width;
  }

  unsigned int Tilemap::get_tile_height() const
  {
   return cell_height;
  }

  unsigned int Tilemap::get_x(const unsigned int row) const
  {
   return row*cell_width;
  }

  unsigned int Tilemap::get_y(const unsigned int column) const
  {
   return column*cell_height;
  }

  unsigned int Tilemap::get_row(const unsigned int x) const
  {
   unsigned int row;
   row=0;
   if (x>cell_width)
   {
    row=x/cell_width;
   }
   return row;
  }

  unsigned int Tilemap::get_column(const unsigned int y) const
  {
   unsigned int column;
   column=0;
   if (y>cell_height)
   {
    column=y/cell_height;
   }
   return column;
  }

  unsigned int Tilemap::get_row_amount(const unsigned int viewport_width) const
  {
   unsigned int amount;
   amount=0;
   if (viewport_width>0)
   {
    amount=viewport_width/cell_width;
    if ((viewport_width%cell_width)!=0)
    {
     ++amount;
    }

   }
   return amount;
  }

  unsigned int Tilemap::get_column_amount(const unsigned int viewport_height) const
  {
   unsigned int amount;
   amount=0;
   if (viewport_height>0)
   {
    amount=viewport_height/cell_height;
    if ((viewport_height%cell_height)!=0)
    {
     ++amount;
    }

   }
   return amount;
  }

  unsigned int Tilemap::get_tile_amount(const unsigned int viewport_width,const unsigned int viewport_height) const
  {
   return this->get_row_amount(viewport_width)*this->get_column_amount(viewport_height);
  }

  bool Tilemap::check_row(const unsigned int row,const unsigned int viewport_width) const
  {
   return row<this->get_row_amount(viewport_width);
  }

  bool Tilemap::check_column(const unsigned int column,const unsigned int viewport_height) const
  {
   return column<this->get_column_amount(viewport_height);
  }

  BIRD2D::BOX Tilemap::get_box(const unsigned int row,const unsigned int column) const
  {
   BIRD2D::BOX collision;
   collision.x=this->get_x(row);
   collision.y=this->get_y(column);
   collision.width=cell_width;
   collision.height=cell_height;
   return collision;
  }

  World::World()
  {
   half_tile_width=1;
   full_tile_height=1;
  }

  World::~World()
  {

  }

  World* World::get_handle()
  {
   return this;
  }

  void World::initialize(const unsigned int tile_width,const unsigned int tile_height)
  {
   if (tile_width>1)
   {
    half_tile_width=tile_width/2;
   }
   if (tile_height>1)
   {
    full_tile_height=tile_height;
   }

  }

  unsigned int World::get_row_amount(const unsigned int screen_height) const
  {
   return screen_height/full_tile_height;
  }

  unsigned int World::get_column_amount(const unsigned int screen_width) const
  {
   return screen_width/half_tile_width;
  }

  unsigned int World::get_tile_amount(const unsigned int screen_width,const unsigned int screen_height) const
  {
   return this->get_row_amount(screen_height)*this->get_column_amount(screen_width);
  }

 }

 namespace Common
 {

  Timer::Timer()
  {
   start=time(NULL);
   interval=0.0;
  }

  Timer::~Timer()
  {

  }

  Timer* Timer::get_handle()
  {
   return this;
  }

  void Timer::set_timer(const double seconds)
  {
   interval=seconds;
   start=time(NULL);
  }

  double Timer::get_interval() const
  {
   return interval;
  }

  bool Timer::check_timer()
  {
   bool check;
   check=difftime(time(NULL),start)>=interval;
   if (check==true)
   {
    start=time(NULL);
   }
   return check;
  }

  Collision::Collision()
  {
   first.x=0;
   first.y=0;
   first.width=0;
   first.height=0;
   second=first;
  }

  Collision::~Collision()
  {

  }

  Collision* Collision::get_handle()
  {
   return this;
  }

  bool Collision::check_horizontal_collision() const
  {
   return ((first.x+first.width)>=second.x) && (first.x<=(second.x+second.width));
  }

  bool Collision::check_vertical_collision() const
  {
   return ((first.y+first.height)>=second.y) && (first.y<=(second.y+second.height));
  }

  void Collision::set_target(const BIRD2D::BOX first_target,const BIRD2D::BOX second_target)
  {
   first=first_target;
   second=second_target;
  }

  bool Collision::check_collision() const
  {
   return this->check_horizontal_collision() && this->check_vertical_collision();
  }

  bool Collision::check_collision(const BIRD2D::BOX first_target,const BIRD2D::BOX second_target)
  {
   this->set_target(first_target,second_target);
   return this->check_collision();
  }

 }

 namespace Filesystem
 {

  bool delete_file(const char *name)
  {
   return remove(name)==0;
  }

  bool file_exist(const char *name)
  {
   FILE *target;
   bool exist;
   exist=false;
   target=fopen(name,"rb");
   if (target!=NULL)
   {
    exist=true;
    fclose(target);
   }
   return exist;
  }

 }

 namespace Tools
 {

  BIRD2D::BOX generate_box(const unsigned int x,const unsigned int y,const unsigned int width,const unsigned int height)
  {
   BIRD2D::BOX collision;
   collision.x=x;
   collision.y=y;
   collision.width=width;
   collision.height=height;
   return collision;
  }

  void quit()
  {
   exit(EXIT_SUCCESS);
  }

  bool enable_logging(const char *name)
  {
   return freopen(name,"wt",stdout)!=NULL;
  }

  void randomize()
  {
   srand(clock()/CLOCKS_PER_SEC);
  }

  unsigned int get_random(const unsigned int number)
  {
   return rand()%(number+1);
  }

  unsigned int get_texture_size()
  {
   return MAXIMUM_TEXTURE_SIZE;
  }

 }

}