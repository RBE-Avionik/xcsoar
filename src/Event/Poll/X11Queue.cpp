/*
Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2015 The XCSoar Project
  A detailed list of copyright holders can be found in the file "AUTHORS".

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
}
*/

#include "X11Queue.hpp"
#include "Queue.hpp"
#include "../Shared/Event.hpp"
#include "IO/Async/IOLoop.hpp"

/* kludges to work around namespace collisions with X11 headers */

#define Font X11Font
#define Window X11Window
#define Display X11Display

#include <X11/Xlib.h>

#undef Font
#undef Window
#undef Display

#include <stdio.h>
#include <stdlib.h>

X11EventQueue::X11EventQueue(IOLoop &_io_loop, EventQueue &_queue)
  :io_loop(_io_loop), queue(_queue),
   display(XOpenDisplay(nullptr))
{
  if (display == nullptr) {
    fprintf(stderr, "XOpenDisplay() failed\n");
    exit(EXIT_FAILURE);
  }

  wm_delete_window = XInternAtom(display, "WM_DELETE_WINDOW", false);

  io_loop.Add(FileDescriptor(ConnectionNumber(display)), io_loop.READ, *this);
}

X11EventQueue::~X11EventQueue()
{
  io_loop.Remove(FileDescriptor(ConnectionNumber(display)));
  XCloseDisplay(display);
}

inline void
X11EventQueue::HandleEvent(_XEvent &event)
{
  switch (event.type) {
  case KeyPress:
    queue.Push(Event(Event::KEY_DOWN,
                     XLookupKeysym(&event.xkey, 0)));
    break;

  case KeyRelease:
    queue.Push(Event(Event::KEY_UP,
                     XLookupKeysym(&event.xkey, 0)));
    break;

  case ButtonPress:
    queue.Push(Event(Event::MOUSE_DOWN, event.xbutton.x, event.xbutton.y));
    break;

  case ButtonRelease:
    queue.Push(Event(Event::MOUSE_UP, event.xbutton.x, event.xbutton.y));
    break;

  case MotionNotify:
    queue.Push(Event(Event::MOUSE_MOTION, event.xmotion.x, event.xmotion.y));
    break;

  case ClientMessage:
    if ((Atom)event.xclient.data.l[0] == wm_delete_window)
      queue.Push(Event::CLOSE);
    break;
  }
}

bool
X11EventQueue::OnFileEvent(FileDescriptor fd, unsigned mask)
{
  while(XPending(display)) {
    XEvent event;
    XNextEvent(display, &event);
    HandleEvent(event);
  }

  return true;
}