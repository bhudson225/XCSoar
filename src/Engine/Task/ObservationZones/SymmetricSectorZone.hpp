/*
  Copyright_License {

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

#ifndef SYMMETRICSECTORZONE_HPP
#define SYMMETRICSECTORZONE_HPP

#include "SectorZone.hpp"

/** Segment centered on bisector of incoming/outgoing legs */
class SymmetricSectorZone: public SectorZone
{
  const Angle sector_angle;

protected:
  /**
   * Constructor.
   *
   * @param loc Tip of sector location
   * @param radius Radius (m) of sector
   * @param angle Angle subtended by start/end radials
   *
   * @return Initialised object
   */
  SymmetricSectorZone(Shape _shape, const GeoPoint &loc,
                      const fixed radius=fixed(10000.0),
                      const Angle angle=Angle::QuarterCircle())
    :SectorZone(_shape, loc, radius), sector_angle(angle) {}

  SymmetricSectorZone(const SymmetricSectorZone &other,
                      const GeoPoint &reference)
    :SectorZone((const SectorZone &)other, reference),
     sector_angle(other.sector_angle) {}

public:
  virtual ObservationZonePoint *Clone(const GeoPoint *_location=NULL) const = 0;

  /**
   * Update radials when previous/next legs are modified.
   *
   * @param previous Previous task point (origin of inbound leg)
   * @param current Taskpoint this is located at
   * @param next Following task point (destination of outbound leg)
   */
  virtual void SetLegs(const GeoPoint *previous, const GeoPoint *current,
                       const GeoPoint *next);

  /**
   * Test whether an OZ is equivalent to this one
   *
   * @param other OZ to compare to
   *
   * @return True if same type and OZ parameters
   */
  virtual bool Equals(const ObservationZonePoint &other) const;

  /** 
   * Accessor for angle of sector (angle between start/end radials)
   * 
   * @return Angle (deg) of sector
   */
  Angle GetSectorAngle() const {
    return sector_angle;
  }
};


#endif
