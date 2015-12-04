/* Copyright_License {

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
#ifndef AIRSPACESINTERFACE_HPP
#define AIRSPACESINTERFACE_HPP

#include "Util/SliceAllocator.hpp"
#include "Airspace.hpp"
#include "Geo/Flat/BoundingBoxDistance.hpp"

#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry/geometries/register/point.hpp>
#include <boost/geometry/geometries/register/box.hpp>

#include <assert.h>

BOOST_GEOMETRY_REGISTER_POINT_2D(FlatGeoPoint, int,
                                 boost::geometry::cs::cartesian, x, y)

BOOST_GEOMETRY_REGISTER_BOX(FlatBoundingBox, FlatGeoPoint,
                            lower_left, upper_right)

/**
 * Abstract class for interface to #Airspaces database.
 * Allows references to the #Airspaces to be substituted for a
 * facade protected class where locking is required.
 */
class AirspacesInterface {
  /** Function object used by kd-tree to index coordinates */
  struct kd_get_bounds
  {
    /** Used by kd-tree */
    typedef int result_type;

    /**
     * Retrieve coordinate value given coordinate index and object
     *
     * @param d Object being stored in kd-tree
     * @param k Index of coordinate
     *
     * @return Coordinate value
     */
    int operator()(const FlatBoundingBox &d, const unsigned k) const {
      switch(k) {
      case 0:
        return d.GetLeft();
      case 1:
        return d.GetBottom();
      case 2:
        return d.GetRight();
      case 3:
        return d.GetTop();
      default:
        assert(false);
        gcc_unreachable();
      };
    };
  };

  struct AirspaceIndexable {
    typedef FlatBoundingBox result_type;

    result_type operator()(const Airspace &airspace) const {
      return airspace;
    }
  };

  /**
   * Distance metric function object used by kd-tree.  This specialisation
   * allows for overlap; distance is zero with overlap, otherwise the minimum
   * distance between two regions.
   */
  struct kd_distance
  {
    /** Distance operator for overlap functionality */
    typedef BBDist distance_type;

    /**
     * \todo document this!
     *
     * @param a
     * @param b
     * @param dim
     *
     * @return Distance on axis
     */
    distance_type operator()(const int a, const int b, const size_t dim) const {
      return BBDist(dim, std::max((dim < 2) ? (b - a) : (a - b), 0));
    }
  };

public:
  typedef std::vector<Airspace> AirspaceVector; /**< Vector of airspaces (used internally) */

  /**
   * Type of KD-tree data structure for airspace container
   */
  typedef boost::geometry::index::rtree<Airspace, boost::geometry::index::rstar<16>,
                                        AirspaceIndexable> AirspaceTree;
};

#endif
