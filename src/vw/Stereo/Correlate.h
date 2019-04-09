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


#ifndef __VW_STEREO_CORRELATE_H__
#define __VW_STEREO_CORRELATE_H__

#include <vw/Core/Exception.h>
#include <vw/Core/FundamentalTypes.h>
#include <vw/Core/Log.h>
#include <vw/Image/ImageView.h>
#include <vw/Image/ImageViewBase.h>
#include <vw/Image/Interpolation.h>
#include <vw/Image/Manipulation.h>
#include <vw/Image/PixelMask.h>
#include <vw/Math/Vector.h>
#include <vw/Math/Matrix.h>
#include <vw/Math/LinearAlgebra.h>

namespace vw {
namespace stereo {

  /// ???
  int
  adjust_weight_image(ImageView<float> &weight,
                      ImageView<PixelMask<Vector2f> > const& disparity_map_patch,
                      ImageView<float> const& weight_template);


  namespace detail {
    ImageView<float>
    compute_spatial_weight_image(int32 kern_width, int32 kern_height,
                                 float two_sigma_sqr);
  }

  /// Check for consistency between a left-to-right and right-to-left
  ///  pair of stereo results.  The results are expected to be opposite in
  ///  sign but equal in magnitude.
  template <class ImageT1, class ImageT2>
  void cross_corr_consistency_check( ImageViewBase<ImageT1> const& l2r,
                                     ImageViewBase<ImageT2> const& r2l,
                                     float cross_corr_threshold, 
                                     bool verbose = false );


  /// Fast affine-EM implementation
  /// In this version we don't keep around future research ideas
  /// since they are slow.
  template<class ChannelT> void
  subpixel_optimized_affine_2d_EM(ImageView<PixelMask<Vector2f> > &disparity_map,
                                  ImageView<ChannelT> const& left_image,
                                  ImageView<ChannelT> const& right_image,
                                  int32  kern_width, int32 kern_height,
                                  BBox2i region_of_interest,
                                  bool   do_horizontal_subpixel,
                                  bool   do_vertical_subpixel,
                                  bool /*verbose*/ );

  /// Research-y implementation of affine EM correlation.
  template<class ChannelT> void
  subpixel_correlation_affine_2d_EM(ImageView<PixelMask<Vector2f> > &disparity_map,
                                    ImageView<ChannelT> const& left_image,
                                    ImageView<ChannelT> const& right_image,
                                    int32 kern_width, int32 kern_height,
                                    BBox2i region_of_interest,
                                    bool do_horizontal_subpixel,
                                    bool do_vertical_subpixel,
                                    bool /*verbose*/ );

  /// Affine subpixel correlation function
  /// - Similar to the _EM function but about 5X faster and sometimes less accurate.
  template<class ChannelT> void
  subpixel_optimized_affine_2d(ImageView<PixelMask<Vector2f> > &disparity_map,
                              ImageView<ChannelT> const& left_image,
                              ImageView<ChannelT> const& right_image,
                              int32  kern_width, int32 kern_height,
                              BBox2i region_of_interest,
                              bool   do_horizontal_subpixel,
                              bool   do_vertical_subpixel,
                              bool /*verbose*/ );

  /// Lucas-Kanade subpixel correlation function
  /// - So far this one does not work that well.
  template<class ChannelT> void
  subpixel_optimized_LK_2d(ImageView<PixelMask<Vector2f> > &disparity_map,
                          ImageView<ChannelT> const& left_image,
                          ImageView<ChannelT> const& right_image,
                          int32  kern_width, int32 kern_height,
                          BBox2i region_of_interest,
                          bool   do_horizontal_subpixel,
                          bool   do_vertical_subpixel,
                          bool /*verbose*/ );
    
  
#include <vw/Stereo/Correlate.tcc>


}} // namespace vw::stereo

#endif // __VW_STEREO_CORRELATOR_H__
