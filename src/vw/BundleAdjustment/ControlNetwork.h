// __BEGIN_LICENSE__
//  Copyright (c) 2006-2013, United States Government as represented by the
//  Administrator of the National Aeronautics and Space Administration. All
//  rights reserved.
//
//  The NASA Vision Workbench is licensed under the Apache License,
//  Version 2.0 (the "License"); you may not use this file except in
//  compliance with the License. You may obtain a copy of the License at
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
// __END_LICENSE__


/// \file ControlNetwork.h
///
/// This file contains useful classes storing bundle adjustment information.
/// The classes are more complicated to give them some compatibility with
/// the ISIS bundle adjustment tool.

#ifndef __VW_BUNDLEADJUSTMENT_CONTROL_NETWORK_H__
#define __VW_BUNDLEADJUSTMENT_CONTROL_NETWORK_H__

// STL
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>

// VW
#include <vw/Math/Vector.h>
#include <vw/Cartography/Datum.h>

// Boost
#include <boost/algorithm/string.hpp>

namespace vw {
namespace ba {

  class ControlNetwork;

  /// Possible ways to store a control network on disk.
  enum ControlStorageFmt { FmtBinary, FmtIsisPvl, FmtCsv };

  /// A ControlMeasure identifies a pixel in an image that corresponds
  /// to a control point.  In addition to the location of the pixel, the
  /// control measure also stores the uncertainty of the measurement,
  /// and a identifier for the image from which it was derived.
  class ControlMeasure {
    std::string m_serialNumber;
    float       m_col, m_row, m_col_sigma, m_row_sigma, m_diameter;
    std::string m_date_time, m_description, m_chooserName;
    double      m_focalplane_x, m_focalplane_y;
    double      m_ephemeris_time;
    uint64      m_image_id;
    bool        m_ignore, m_pixels_dominant;

  public:

    /// Control Measure Type
    ///
    /// Unmeasured         - This measure doesn't exist. It is a black void.
    /// Manual             - The measure was created by a human and is not
    ///                      quite perfect.
    /// Estimated          - Created by a computer but has not been sub
    ///                      pixel registered. Still subject to refinement
    /// Automatic          - Created by a computer and sub pixel registered.
    ///                      Unfortunately this is still subject to refinement
    /// ValidatedManual    - Created by a human and has been validated by a
    ///                      human.
    /// ValidatedAutomatic - Created by a computer and has been validated by
    ///                      a human.
    enum ControlMeasureType { Unmeasured=0, Manual=1, Estimated=2, Automatic=3,
                              ValidatedManual=4, ValidatedAutomatic=5 };
    ControlMeasureType m_type;

    /// Constructor
    ControlMeasure( float, float, float, float, uint64,
                    ControlMeasureType type = ControlMeasure::Automatic );
    ControlMeasure( ControlMeasureType type = ControlMeasure::Automatic );
    ControlMeasure( std::istream& f, ControlStorageFmt fmt) {
      switch (fmt) {
        case FmtBinary:  this->read_binary(f); break;
        case FmtIsisPvl: this->read_isis(f);   break;
        case FmtCsv:     this->read_csv(f);    break;
      }
    }

    /// Setting/Reading control measure type
    ControlMeasureType type() const { return m_type; }
    void set_type( ControlMeasureType type ) { m_type = type; }

    /// Setting/Reading the pixel location
    Vector2 position() const { return Vector2(m_col, m_row); }
    void set_position(float col, float row) {
      m_col = col;
      m_row = row;
    }
    void set_position(Vector2 position) {
      m_col = position[0];
      m_row = position[1];
    }

    /// Setting/Reading millimeter location
    Vector2 focalplane() const { return Vector2( m_focalplane_x,
                                                 m_focalplane_y ); }
    void set_focalplane( double x, double y ) {
      m_focalplane_x = x;
      m_focalplane_y = y;
    }
    void set_focalplane( Vector2 location ) {
      m_focalplane_x = location[0];
      m_focalplane_y = location[1];
    }

    /// Setting/Reading dominant location (used by BA, defaults to
    /// position)
    Vector2 dominant() const {
      return m_pixels_dominant ? Vector2(m_col,m_row) : Vector2(m_focalplane_x,m_focalplane_y);
    }
    void set_dominant( double x, double y ) {
      if ( m_pixels_dominant ) {
        m_col = x; m_row = y;
      } else {
        m_focalplane_x = x; m_focalplane_y = y;
      }
    }
    void set_dominant( Vector2 location ) {
      if ( m_pixels_dominant ) {
        m_col = location[0]; m_row = location[1];
      } else {
        m_focalplane_x = location[0]; m_focalplane_y = location[1];
      }
    }
    bool is_pixels_dominant() { return m_pixels_dominant; }
    void set_pixels_dominant( bool state ) { m_pixels_dominant = state; }

    /// Setting/Reading the pixel error for this point.
    Vector2 sigma() const { return Vector2(m_col_sigma, m_row_sigma); }
    float sigma_magnitude() const {
      return sqrt(m_col_sigma*m_col_sigma +
                  m_row_sigma*m_row_sigma );
    }
    void set_sigma(float col_sigma, float row_sigma) {
      m_col_sigma = col_sigma;
      m_row_sigma = row_sigma;
    }
    void set_sigma(Vector2 sigma) {
      m_col_sigma = sigma[0];
      m_row_sigma = sigma[1];
    }

    /// Setting/Reading the identifier for the image from which this
    /// control point was derived.
    uint64 image_id() const { return m_image_id; }
    void set_image_id(uint64 image_id) { m_image_id = image_id; }

    /// Setting/Reading the description
    std::string description() const { return m_description; }
    void set_description(std::string const& description) { m_description = description; }

    /// Setting/Reading the data & time
    std::string date_time() const { return m_date_time; }
    void set_date_time(std::string const& date_time) { m_date_time = date_time; }

    /// Setting/Reading the chooser's name
    std::string chooser() const { return m_chooserName; }
    void set_chooser(std::string const& chooser) { m_chooserName = chooser; }

    /// Setting/Reading the measure's serial number
    std::string serial() const { return m_serialNumber; }
    void set_serial(std::string const& serial) { m_serialNumber = serial; }

    /// Setting/Reading whether this control measurement should be
    /// ignored in a bundle adjustment.
    bool ignore() const { return m_ignore; }
    void set_ignore(bool state) { m_ignore = state; }

    /// Setting/Reading Ephemeris Time
    double ephemeris_time() const { return m_ephemeris_time; }
    void set_ephemeris_time( double const& time ) { m_ephemeris_time = time; }

    /// Get the image name associated with this point in a control network.
    /// - Returns an empty string if there is no associated image name.
    std::string get_image_name(ControlNetwork const& net) const;

    /// File I/O
    void read_binary ( std::istream& f );
    void read_isis   ( std::istream& f );
    void read_csv    ( std::istream& f );
    void write_binary( std::ostream& f ) const;
    void write_isis  ( std::ostream& f ) const;
    void write_csv   ( std::ostream& f ) const;

  }; // End class ControlMeasure

  /// Two control meaures are considered equal if their position,
  /// sigma, and image_id are equal.
  inline bool operator==( ControlMeasure const& m1, ControlMeasure const& m2 ) {
    if (m1.position() == m2.position() &&
        m1.sigma() == m2.sigma() &&
        m1.image_id() == m2.image_id() &&
        m1.ephemeris_time() == m2.ephemeris_time() )
      return true;
    //else
    return false;
  }

  std::ostream& operator<<( std::ostream& os, ControlMeasure const& measure);

  /// ControlPoints are 3D locations in geographic coordinates (lon,
  /// lat, radius) that are associated with a certain number of
  /// ControlMeasures.  Each ControlMeasure is an observation where
  /// this ControlPoint was located in an image.
  class ControlPoint {

    std::string m_id;
    std::vector<ControlMeasure> m_measures; ///< List of sightings of this point.
    bool    m_ignore;
    Vector3 m_position;
    Vector3 m_sigma;

  public:
    /// Iterators
    typedef std::vector<ControlMeasure>::iterator             iterator;
    typedef std::vector<ControlMeasure>::const_iterator const_iterator;
          iterator begin()       { return m_measures.begin(); }
          iterator end  ()       { return m_measures.end  (); }
    const_iterator begin() const { return m_measures.begin(); }
    const_iterator end  () const { return m_measures.end  (); }

    /// Control Point Type
    enum ControlPointType { GroundControlPoint, TiePoint };
    ControlPointType m_type;

    /// Constructor
    ControlPoint(ControlPointType type = ControlPoint::TiePoint);
    ControlPoint(std::istream& f, ControlStorageFmt fmt) {
      switch (fmt) {
        case FmtBinary:  this->read_binary(f); break;
        case FmtIsisPvl: this->read_isis(f);   break;
        case FmtCsv:     this->read_csv(f);    break;
      }
    }

    /// Setting/Reading Type
    ControlPointType type() const { return m_type; }
    void set_type(ControlPointType type) { m_type = type; }

    /// Setting/Reading ID
    std::string id() const { return m_id; }
    void set_id(std::string id) { m_id = id; }

    /// Setting/Reading Ignore
    bool ignore() const { return m_ignore; }
    void set_ignore(bool ignore) { m_ignore = ignore; }

    /// Returns the number of control measures associated with this
    /// control point.
    size_t size() const { return m_measures.size(); }

    /// Associate a single control measure with this ControlPoint
    void add_measure(ControlMeasure const& measure);

    /// Associate multiple control measures with this ControlPoint
    void add_measures(std::vector<ControlMeasure> const& measures);

    /// Remove the control point at the specified index.
    void delete_measure(size_t index);

    /// Access a specific control measure that is associated with this control point.
          ControlMeasure& operator[] (size_t index)       { return m_measures[index]; }
    const ControlMeasure& operator[] (size_t index) const { return m_measures[index]; }

    /// Accessors for the list of measures (sightings)
    void clear     ()                { m_measures.clear();           }
    void resize    (size_t sz)       { m_measures.resize( sz );      }
    size_t capacity()          const { return m_measures.capacity(); }
    bool empty     ()          const { return m_measures.empty();    }
    void reserve   (size_t sz)       { m_measures.reserve(sz);       }

    /// Locate a control measure that is equal to the query.  Returns
    /// this->size() if no match is found.
    size_t find(ControlMeasure const& query);

    /// Setting/Reading the position of the control point
    void set_position(double x, double y, double z) {
      m_position = Vector3(x,y,z);
    }
    void set_position(Vector3 position) { m_position = position; }
    Vector3 position() const { return m_position; }

    /// Setting/Reading the uncertainty of the control point
    void set_sigma(double lon_sigma, double lat_sigma, double radius_sigma) {
      m_sigma = Vector3(lon_sigma, lat_sigma, radius_sigma);
    }
    void set_sigma(Vector3 sigma) { m_sigma = sigma; }
    /// Returns the uncertainty of the control point as [longitude_sigma,
    /// latitude_sigma, radius_sigma]
    Vector3 sigma() const { return m_sigma; }

    /// File I/O
    void read_binary ( std::istream& f );
    void read_isis   ( std::istream& f );
    void read_csv    ( std::istream& f );
    void write_binary( std::ostream& f ) const;
    void write_isis  ( std::ostream& f ) const;
    void write_csv   ( std::ostream& f ) const;

  }; // End class ControlPoint

  std::ostream& operator<<( std::ostream& os, ControlPoint const& point);

  /// The control network contains a list of control points (either
  /// ground control points or tie points).
  ///
  /// Things to add:
  /// - various methods for computing error
  /// - assoc. with image list/serial number
  ///
  class ControlNetwork {
    /// 3D points, each with a list of observations.
    std::vector<ControlPoint> m_control_points;

    std::string m_targetName;         // Name of the target
    std::string m_networkId;          // Network Id
    std::string m_created;            // Creation Date
    mutable std::string m_modified;   // Data Last Modified
    std::string m_description;        // Text description of network
    std::string m_userName;           // The user who created the network

    // TODO: Incorporate these into the read/write functions!
    /// Stores a list of image names in the order matching the image
    ///  ID's stored in the control measures.
    std::vector<std::string> m_image_names;

  public:

    /// Iterators through the list of points.
    typedef std::vector<ControlPoint>::iterator             iterator;
    typedef std::vector<ControlPoint>::const_iterator const_iterator;
          iterator begin()       { return m_control_points.begin(); }
          iterator end  ()       { return m_control_points.end  (); }
    const_iterator begin() const { return m_control_points.begin(); }
    const_iterator end  () const { return m_control_points.end  (); }

    // Simple access to the image list.
    std::vector<std::string>      & get_image_list()       {return m_image_names;}
    std::vector<std::string> const& get_image_list() const {return m_image_names;}

    /// Add a new image and return the index associated with it.
    size_t add_image_name(std::string const& name) {
      m_image_names.push_back(name);
      return m_image_names.size()-1;
    }

    /// Control Network Type
    ///
    /// Singleton     - A Control network that just points out
    ///                 interesting points.
    /// ImageToImage  - A Control network lacking of all GCPs
    /// ImageToGround - A Control network with mixed control
    ///                 points (GCPs and nots)
    enum ControlNetworkType { Singleton, ImageToImage, ImageToGround };
    ControlNetworkType m_type;

    /// Constructor
    ControlNetwork(std::string id,
                   ControlNetworkType type = ControlNetwork::ImageToImage,
                   std::string target_name = "Unknown",
                   std::string descrip = "Null",
                   std::string user_name = "VW" );
    ControlNetwork(const std::string& f, ControlStorageFmt fmt) {
      switch (fmt) {
        case FmtBinary:  this->read_binary(f); break;
        case FmtIsisPvl: this->read_isis(f);   break;
        case FmtCsv:     this->read_csv(f);    break;
      }
    }

    /// Setting/Reading Type
    ControlNetworkType type() const { return m_type; }
    void set_type( ControlNetworkType type ) { m_type = type; }

    /// Returns the number of 3D points in the control network.
    size_t size() const { return m_control_points.size(); }

    /// Return the number of Control Points that are Ground Control Points (GCPs)
    size_t num_ground_control_points() const {
      if ( m_type != ControlNetwork::ImageToGround )
        return 0;

      size_t count=0;
      for (size_t i=0; i<this->size(); ++i) {
        if ((*this)[i].type() == ControlPoint::GroundControlPoint)
          ++count;
      }
      return count;
    }

    /// Return the number of Control Points that are of the generic
    /// image tie points (not GCP's)
    size_t num_tie_points() const {
      size_t count=0;
      for (size_t i=0; i<this->size(); ++i) {
        if ((*this)[i].type() == ControlPoint::TiePoint)
          ++count;
      }
      return count;
    }

    /// Add a single Control Point
    void add_control_point(ControlPoint const& point);

    /// Add a vector of Control Points
    void add_control_points(std::vector<ControlPoint> const& points);

    /// Remove the control point at the specified index.
    void delete_control_point(size_t index);

    /// Access a specific control point that is associated with this control network.
          ControlPoint& operator[] (size_t index)       { return m_control_points[index]; }
    const ControlPoint& operator[] (size_t index) const { return m_control_points[index]; }

    /// Vector access to the control points.
    void   clear   ()                { m_control_points.clear();           }
    void   resize  (size_t sz)       { m_control_points.resize(sz);        }
    size_t capacity()          const { return m_control_points.capacity(); }
    bool   empty   ()          const { return m_control_points.empty();    }
    void   reserve (size_t sz)       { m_control_points.reserve(sz);       }

    /// Locate a control point that contains the control measure (sighting) that
    /// is equal to the query.  Returns this->size() if no match is found.
    size_t find_measure(ControlMeasure const& query);

    /// File I/O
    void read_binary ( std::string const& filename );
    void read_isis   ( std::string const& filename );
    void read_csv    ( std::string const& filename );
    void write_binary( std::string        filename ) const;
    void write_isis  ( std::string        filename );
    void write_csv   ( std::string        filename ) const;

    /// Write to a csv file in the same format used for ground control points.
    void write_in_gcp_format( std::string const& filename, cartography::Datum const& d) const;

  }; // End class ControlNetwork

  std::ostream& operator<<( std::ostream& os, ControlNetwork const& cnet);

  /// I/O for ISIS Pvl file
  void read_pvl_property( std::ostringstream& ostr,
                          std::vector< std::string >& tokens );

}} // namespace vw::ba




#endif // __VW_BUNDLEADJUSTMENT_CONTROL_NETWORK_H__
