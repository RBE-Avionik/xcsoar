/* Copyright_License {

  XCSoar Glide Computer - http://www.xcsoar.org/
  Copyright (C) 2000-2012 The XCSoar Project
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

#ifndef TASK_DIJKSTRA_MIN_HPP
#define TASK_DIJKSTRA_MIN_HPP

#include "TaskDijkstra.hpp"

/**
 * Specialisation of TaskDijkstra for minimum distance search
 */
class TaskDijkstraMin : public TaskDijkstra {
public:
  TaskDijkstraMin(OrderedTask& _task)
    :TaskDijkstra(_task, true) {}

  /**
   * Search task points for targets within OZs to produce the
   * minimum-distance task.  Saves the minimum-distance solution
   * in the corresponding task points for later accurate distance
   * measurement.
   *
   * Note that the minimum distance task is the minimum distance
   * remaining and is therefore sensitive to the specified aircraft
   * location.
   *
   * @param location Location of aircraft
   * @return True if succeeded
   */
  bool DistanceMin(const SearchPoint& location);
};

#endif
