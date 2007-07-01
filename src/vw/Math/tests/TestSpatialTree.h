// __BEGIN_LICENSE__
// 
// Copyright (C) 2006 United States Government as represented by the
// Administrator of the National Aeronautics and Space Administration
// (NASA).  All Rights Reserved.
// 
// Copyright 2006 Carnegie Mellon University. All rights reserved.
// 
// This software is distributed under the NASA Open Source Agreement
// (NOSA), version 1.3.  The NOSA has been approved by the Open Source
// Initiative.  See the file COPYING at the top of the distribution
// directory tree for the complete NOSA document.
// 
// THE SUBJECT SOFTWARE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY OF ANY
// KIND, EITHER EXPRESSED, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT
// LIMITED TO, ANY WARRANTY THAT THE SUBJECT SOFTWARE WILL CONFORM TO
// SPECIFICATIONS, ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR
// A PARTICULAR PURPOSE, OR FREEDOM FROM INFRINGEMENT, ANY WARRANTY THAT
// THE SUBJECT SOFTWARE WILL BE ERROR FREE, OR ANY WARRANTY THAT
// DOCUMENTATION, IF PROVIDED, WILL CONFORM TO THE SUBJECT SOFTWARE.
// 
// __END_LICENSE__

#include <sstream>

#include <cxxtest/TestSuite.h>
#include <vw/Math/Vector.h>
#include <vw/Math/BBox.h>
#include <vw/Math/SpatialTree.h>

using namespace std;
using namespace vw;
using namespace vw::math;

#define TEST_SPATIAL_TREE_PRINT_RESULT_1D "+ Min[Vector1(0)] Max[Vector1(16)]\n  + Min[Vector1(8)] Max[Vector1(16)]\n    + Min[Vector1(12)] Max[Vector1(16)]\n    + Min[Vector1(8)] Max[Vector1(12)]\n      + Min[Vector1(10)] Max[Vector1(12)]\n      + Min[Vector1(8)] Max[Vector1(10)]\n        + Min[Vector1(9)] Max[Vector1(10)]\n          + Min[Vector1(9.5)] Max[Vector1(10)]\n          + Min[Vector1(9)] Max[Vector1(9.5)]\n            + Min[Vector1(9.25)] Max[Vector1(9.5)]\n            + Min[Vector1(9)] Max[Vector1(9.25)]\n              + Min[Vector1(9.125)] Max[Vector1(9.25)]\n              + Min[Vector1(9)] Max[Vector1(9.125)]\n                Min[Vector1(9)] Max[Vector1(9.1)]\n        + Min[Vector1(8)] Max[Vector1(9)]\n  + Min[Vector1(0)] Max[Vector1(8)]\n    + Min[Vector1(4)] Max[Vector1(8)]\n    + Min[Vector1(0)] Max[Vector1(4)]\n      + Min[Vector1(2)] Max[Vector1(4)]\n      + Min[Vector1(0)] Max[Vector1(2)]\n        + Min[Vector1(1)] Max[Vector1(2)]\n          Min[Vector1(1)] Max[Vector1(1.75)]\n          + Min[Vector1(1.5)] Max[Vector1(2)]\n            Min[Vector1(1.5)] Max[Vector1(2)]\n          + Min[Vector1(1)] Max[Vector1(1.5)]\n        + Min[Vector1(0)] Max[Vector1(1)]\n          + Min[Vector1(0.5)] Max[Vector1(1)]\n          + Min[Vector1(0)] Max[Vector1(0.5)]\n            + Min[Vector1(0.25)] Max[Vector1(0.5)]\n            + Min[Vector1(0)] Max[Vector1(0.25)]\n              Min[Vector1(0.1)] Max[Vector1(0.2)]\n"

#define TEST_SPATIAL_TREE_PRINT_RESULT_2D "+ Min[Vector2(0,0)] Max[Vector2(16,16)]\n  + Min[Vector2(8,8)] Max[Vector2(16,16)]\n    + Min[Vector2(12,12)] Max[Vector2(16,16)]\n    + Min[Vector2(12,8)] Max[Vector2(16,12)]\n    + Min[Vector2(8,12)] Max[Vector2(12,16)]\n    + Min[Vector2(8,8)] Max[Vector2(12,12)]\n      + Min[Vector2(10,10)] Max[Vector2(12,12)]\n      + Min[Vector2(10,8)] Max[Vector2(12,10)]\n      + Min[Vector2(8,10)] Max[Vector2(10,12)]\n      + Min[Vector2(8,8)] Max[Vector2(10,10)]\n        + Min[Vector2(9,9)] Max[Vector2(10,10)]\n          + Min[Vector2(9.5,9.5)] Max[Vector2(10,10)]\n          + Min[Vector2(9.5,9)] Max[Vector2(10,9.5)]\n          + Min[Vector2(9,9.5)] Max[Vector2(9.5,10)]\n          + Min[Vector2(9,9)] Max[Vector2(9.5,9.5)]\n            + Min[Vector2(9.25,9.25)] Max[Vector2(9.5,9.5)]\n            + Min[Vector2(9.25,9)] Max[Vector2(9.5,9.25)]\n            + Min[Vector2(9,9.25)] Max[Vector2(9.25,9.5)]\n            + Min[Vector2(9,9)] Max[Vector2(9.25,9.25)]\n              + Min[Vector2(9.125,9.125)] Max[Vector2(9.25,9.25)]\n              + Min[Vector2(9.125,9)] Max[Vector2(9.25,9.125)]\n              + Min[Vector2(9,9.125)] Max[Vector2(9.125,9.25)]\n              + Min[Vector2(9,9)] Max[Vector2(9.125,9.125)]\n                Min[Vector2(9,9)] Max[Vector2(9.1,9.1)]\n        + Min[Vector2(9,8)] Max[Vector2(10,9)]\n        + Min[Vector2(8,9)] Max[Vector2(9,10)]\n        + Min[Vector2(8,8)] Max[Vector2(9,9)]\n  + Min[Vector2(8,0)] Max[Vector2(16,8)]\n  + Min[Vector2(0,8)] Max[Vector2(8,16)]\n  + Min[Vector2(0,0)] Max[Vector2(8,8)]\n    Min[Vector2(1.5,3)] Max[Vector2(2,5)]\n    + Min[Vector2(4,4)] Max[Vector2(8,8)]\n    + Min[Vector2(4,0)] Max[Vector2(8,4)]\n    + Min[Vector2(0,4)] Max[Vector2(4,8)]\n    + Min[Vector2(0,0)] Max[Vector2(4,4)]\n      + Min[Vector2(2,2)] Max[Vector2(4,4)]\n      + Min[Vector2(2,0)] Max[Vector2(4,2)]\n      + Min[Vector2(0,2)] Max[Vector2(2,4)]\n        Min[Vector2(1,2)] Max[Vector2(1.75,4)]\n      + Min[Vector2(0,0)] Max[Vector2(2,2)]\n        + Min[Vector2(1,1)] Max[Vector2(2,2)]\n        + Min[Vector2(1,0)] Max[Vector2(2,1)]\n        + Min[Vector2(0,1)] Max[Vector2(1,2)]\n        + Min[Vector2(0,0)] Max[Vector2(1,1)]\n          + Min[Vector2(0.5,0.5)] Max[Vector2(1,1)]\n          + Min[Vector2(0.5,0)] Max[Vector2(1,0.5)]\n          + Min[Vector2(0,0.5)] Max[Vector2(0.5,1)]\n          + Min[Vector2(0,0)] Max[Vector2(0.5,0.5)]\n            + Min[Vector2(0.25,0.25)] Max[Vector2(0.5,0.5)]\n            + Min[Vector2(0.25,0)] Max[Vector2(0.5,0.25)]\n            + Min[Vector2(0,0.25)] Max[Vector2(0.25,0.5)]\n            + Min[Vector2(0,0)] Max[Vector2(0.25,0.25)]\n              Min[Vector2(0.1,0.1)] Max[Vector2(0.2,0.2)]\n"

#define TEST_SPATIAL_TREE_PRINT_RESULT_3D ""

#define TEST_SPATIAL_TREE_PRINT_RESULT_4D ""

#define TEST_SPATIAL_TREE_VRML_RESULT "#VRML V2.0 utf8\n#\nTransform {\n  translation -8 -8 0\n  children [\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0 0 0,\n            0 16 0,\n            16 16 0,\n            16 0 0,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0.5 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            8 8 -0.5,\n            8 16 -0.5,\n            16 16 -0.5,\n            16 8 -0.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            12 12 -1,\n            12 16 -1,\n            16 16 -1,\n            16 12 -1,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            12 8 -1,\n            12 12 -1,\n            16 12 -1,\n            16 8 -1,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            8 12 -1,\n            8 16 -1,\n            12 16 -1,\n            12 12 -1,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            8 8 -1,\n            8 12 -1,\n            12 12 -1,\n            12 8 -1,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            10 10 -1.5,\n            10 12 -1.5,\n            12 12 -1.5,\n            12 10 -1.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            10 8 -1.5,\n            10 10 -1.5,\n            12 10 -1.5,\n            12 8 -1.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            8 10 -1.5,\n            8 12 -1.5,\n            10 12 -1.5,\n            10 10 -1.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            8 8 -1.5,\n            8 10 -1.5,\n            10 10 -1.5,\n            10 8 -1.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0.5 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            9 9 -2,\n            9 10 -2,\n            10 10 -2,\n            10 9 -2,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0.5 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            9.5 9.5 -2.5,\n            9.5 10 -2.5,\n            10 10 -2.5,\n            10 9.5 -2.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0.5 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            9.5 9 -2.5,\n            9.5 9.5 -2.5,\n            10 9.5 -2.5,\n            10 9 -2.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0.5 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            9 9.5 -2.5,\n            9 10 -2.5,\n            9.5 10 -2.5,\n            9.5 9.5 -2.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0.5 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            9 9 -2.5,\n            9 9.5 -2.5,\n            9.5 9.5 -2.5,\n            9.5 9 -2.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0.5 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            9.25 9.25 -3,\n            9.25 9.5 -3,\n            9.5 9.5 -3,\n            9.5 9.25 -3,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0.5 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            9.25 9 -3,\n            9.25 9.25 -3,\n            9.5 9.25 -3,\n            9.5 9 -3,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0.5 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            9 9.25 -3,\n            9 9.5 -3,\n            9.25 9.5 -3,\n            9.25 9.25 -3,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0.5 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            9 9 -3,\n            9 9.25 -3,\n            9.25 9.25 -3,\n            9.25 9 -3,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            9.125 9.125 -3.5,\n            9.125 9.25 -3.5,\n            9.25 9.25 -3.5,\n            9.25 9.125 -3.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            9.125 9 -3.5,\n            9.125 9.125 -3.5,\n            9.25 9.125 -3.5,\n            9.25 9 -3.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            9 9.125 -3.5,\n            9 9.25 -3.5,\n            9.125 9.25 -3.5,\n            9.125 9.125 -3.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            9 9 -3.5,\n            9 9.125 -3.5,\n            9.125 9.125 -3.5,\n            9.125 9 -3.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 1 0 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            9 9 -3.5,\n            9 9.1 -3.5,\n            9.1 9.1 -3.5,\n            9.1 9 -3.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0.5 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            9 8 -2,\n            9 9 -2,\n            10 9 -2,\n            10 8 -2,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0.5 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            8 9 -2,\n            8 10 -2,\n            9 10 -2,\n            9 9 -2,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0.5 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            8 8 -2,\n            8 9 -2,\n            9 9 -2,\n            9 8 -2,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0.5 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            8 0 -0.5,\n            8 8 -0.5,\n            16 8 -0.5,\n            16 0 -0.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0.5 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0 8 -0.5,\n            0 16 -0.5,\n            8 16 -0.5,\n            8 8 -0.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0.5 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0 0 -0.5,\n            0 8 -0.5,\n            8 8 -0.5,\n            8 0 -0.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 1 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            1.5 3 -0.5,\n            1.5 5 -0.5,\n            2 5 -0.5,\n            2 3 -0.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            4 4 -1,\n            4 8 -1,\n            8 8 -1,\n            8 4 -1,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            4 0 -1,\n            4 4 -1,\n            8 4 -1,\n            8 0 -1,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0 4 -1,\n            0 8 -1,\n            4 8 -1,\n            4 4 -1,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0 0 -1,\n            0 4 -1,\n            4 4 -1,\n            4 0 -1,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            2 2 -1.5,\n            2 4 -1.5,\n            4 4 -1.5,\n            4 2 -1.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            2 0 -1.5,\n            2 2 -1.5,\n            4 2 -1.5,\n            4 0 -1.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0 2 -1.5,\n            0 4 -1.5,\n            2 4 -1.5,\n            2 2 -1.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 1 0 1\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            1 2 -1.5,\n            1 4 -1.5,\n            1.75 4 -1.5,\n            1.75 2 -1.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0 0 -1.5,\n            0 2 -1.5,\n            2 2 -1.5,\n            2 0 -1.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0.5 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            1 1 -2,\n            1 2 -2,\n            2 2 -2,\n            2 1 -2,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0.5 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            1 0 -2,\n            1 1 -2,\n            2 1 -2,\n            2 0 -2,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0.5 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0 1 -2,\n            0 2 -2,\n            1 2 -2,\n            1 1 -2,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0 0.5 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0 0 -2,\n            0 1 -2,\n            1 1 -2,\n            1 0 -2,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0.5 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0.5 0.5 -2.5,\n            0.5 1 -2.5,\n            1 1 -2.5,\n            1 0.5 -2.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0.5 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0.5 0 -2.5,\n            0.5 0.5 -2.5,\n            1 0.5 -2.5,\n            1 0 -2.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0.5 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0 0.5 -2.5,\n            0 1 -2.5,\n            0.5 1 -2.5,\n            0.5 0.5 -2.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0.5 0\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0 0 -2.5,\n            0 0.5 -2.5,\n            0.5 0.5 -2.5,\n            0.5 0 -2.5,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0.5 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0.25 0.25 -3,\n            0.25 0.5 -3,\n            0.5 0.5 -3,\n            0.5 0.25 -3,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0.5 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0.25 0 -3,\n            0.25 0.25 -3,\n            0.5 0.25 -3,\n            0.5 0 -3,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0.5 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0 0.25 -3,\n            0 0.5 -3,\n            0.25 0.5 -3,\n            0.25 0.25 -3,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 0.5 0.5 0.5\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0 0 -3,\n            0 0.25 -3,\n            0.25 0.25 -3,\n            0.25 0 -3,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n    Shape {\n      appearance Appearance {\n        material Material {\n          emissiveColor 1 1 1\n        }\n      }\n      geometry IndexedLineSet {\n        coord Coordinate {\n          point [\n            0.1 0.1 -3,\n            0.1 0.2 -3,\n            0.2 0.2 -3,\n            0.2 0.1 -3,\n          ]\n        }\n        coordIndex [ 0, 1, 2, 3, 0, -1, ]\n      }\n    }\n  ]\n}\n"


class TestGeomPrimitive : public BBoxN, public GeomPrimitive
{
  bool contains(const Vector<double> &point) const {return BBoxN::contains(point);}
  const BBox<double> &bounding_box() const {return *this;}
};

int which_one(GeomPrimitive *p, GeomPrimitive *ps[])
{
  int i;
  for (i = 0; ps[i] != 0; i++)
  {
    if (ps[i] == p)
      break;
  }
  return i;
}

class TestSpatialTree : public CxxTest::TestSuite
{
public:

  void internal_test_spatial_tree(int dim) 
  {
    Vector<double,2> b_min(0, 0), b_max(1, 1);
    BBoxN b(subvector(b_min, 0, dim), subvector(b_max, 0, dim));
    SpatialTree t(b);
    std::list<GeomPrimitive*> l;
    std::list<GeomPrimitive*>::iterator i;
    std::list<std::pair<GeomPrimitive*, GeomPrimitive*> > overlaps;
    std::list<std::pair<GeomPrimitive*, GeomPrimitive*> >::iterator i2;
    int j;

    Vector<double,2> p0(0.1, 0.1), p0_(0.2, 0.2);
    TestGeomPrimitive g0;
    g0.grow(subvector(p0, 0, dim));
    g0.grow(subvector(p0_, 0, dim));
    t.add(&g0);

    Vector<double,2> p1(1, 2), p2(1.75, 4);
    TestGeomPrimitive g1;
    g1.grow(subvector(p1, 0, dim));
    g1.grow(subvector(p2, 0, dim));
    t.add(&g1);

    Vector<double,2> p3(1.5, 3), p4(2, 5);
    TS_ASSERT_EQUALS( t.contains(subvector(p3, 0, dim)), &g1 );
    l.clear();
    t.contains(subvector(p3, 0, dim), l);
    TS_ASSERT_EQUALS( l.size(), 1 );
    TS_ASSERT_EQUALS( *(l.begin()), &g1 );
    TS_ASSERT_EQUALS( t.contains(subvector(p4, 0, dim)), (GeomPrimitive*)0 );
    l.clear();
    t.contains(subvector(p4, 0, dim), l);
    TS_ASSERT_EQUALS( l.size(), 0 );

    TestGeomPrimitive g2;
    g2.grow(subvector(p3, 0, dim));
    g2.grow(subvector(p4, 0, dim));
    t.add(&g2);

    Vector<double, 2> p5(1.25, 3.5), p6(1.6, 3.5), p7(1.75, 4.5), p8(1.25, 4.5), p9(8, 8);
    GeomPrimitive *gt1;
    GeomPrimitive *gt2;
    TS_ASSERT_EQUALS( t.contains(subvector(p5, 0, dim)), &g1 );
    l.clear();
    t.contains(subvector(p5, 0, dim), l);
    TS_ASSERT_EQUALS( l.size(), 1 );
    TS_ASSERT_EQUALS( *(l.begin()), &g1 );
    gt1 = t.contains(subvector(p6, 0, dim));
    TS_ASSERT( gt1 == &g1 || gt1 == &g2 );
    l.clear();
    t.contains(subvector(p6, 0, dim), l);
    TS_ASSERT_EQUALS( l.size(), 2 );
    i = l.begin(); gt1 = *i; i++; gt2 = *i;
    TS_ASSERT( gt1 == &g1 || gt1 == &g2 );
    TS_ASSERT( gt2 == &g1 || gt2 == &g2 );
    TS_ASSERT( gt1 != gt2 );
    TS_ASSERT_EQUALS( t.contains(subvector(p7, 0, dim)), &g2 );
    l.clear();
    t.contains(subvector(p7, 0, dim), l);
    TS_ASSERT_EQUALS( l.size(), 1 );
    TS_ASSERT_EQUALS( *(l.begin()), &g2 );
    if (dim == 1)
    {
      TS_ASSERT_EQUALS( t.contains(subvector(p8, 0, dim)), &g1 );
      l.clear();
      t.contains(subvector(p8, 0, dim), l);
      TS_ASSERT_EQUALS( l.size(), 1 );
      TS_ASSERT_EQUALS( *(l.begin()), &g1 );
    }
    else
    {
      TS_ASSERT_EQUALS( t.contains(subvector(p8, 0, dim)), (GeomPrimitive*)0 );
      l.clear();
      t.contains(subvector(p8, 0, dim), l);
      TS_ASSERT_EQUALS( l.size(), 0 );
    }
    TS_ASSERT_EQUALS( t.contains(subvector(p9, 0, dim)), (GeomPrimitive*)0 );
    l.clear();
    t.contains(subvector(p9, 0, dim), l);
    TS_ASSERT_EQUALS( l.size(), 0 );

    Vector<double,2> p10(9, 9), p10_(9.1, 9.1);
    TestGeomPrimitive g3;
    g3.grow(subvector(p10, 0, dim));
    g3.grow(subvector(p10_, 0, dim));
    t.add(&g3);

    overlaps.clear();
    t.overlap_pairs(overlaps);
    TS_ASSERT_EQUALS( overlaps.size(), 1 );
    i2 = overlaps.begin(); gt1 = (*i2).first; gt2 = (*i2).second;
    TS_ASSERT( gt1 == &g1 || gt1 == &g2 );
    TS_ASSERT( gt2 == &g1 || gt2 == &g2 );
    TS_ASSERT( gt1 != gt2 );

    if (dim <= 2)
    {
      ostringstream os;
      const char *print_results[5] = {0, TEST_SPATIAL_TREE_PRINT_RESULT_1D, TEST_SPATIAL_TREE_PRINT_RESULT_2D, TEST_SPATIAL_TREE_PRINT_RESULT_3D, TEST_SPATIAL_TREE_PRINT_RESULT_4D};
      std::string printstr(print_results[dim]);
      t.print(os);
      TS_ASSERT_EQUALS( os.str(), printstr );
    }

    if (dim >= 2)
    {
      ostringstream os2;
      std::string vrmlstr(TEST_SPATIAL_TREE_VRML_RESULT);
      t.write_vrml(os2);
      TS_ASSERT_EQUALS( os2.str(), vrmlstr );
    }

    Vector<double,2> p11(0.01, 0.01), p12(6, 6);
    TestGeomPrimitive g4;
    g4.grow(subvector(p11, 0, dim));
    g4.grow(subvector(p12, 0, dim));
    t.add(&g4);

    overlaps.clear();
    t.overlap_pairs(overlaps);
    int num_overlaps = 4;
    GeomPrimitive *overlaps_truth[4][2] = {{&g4, &g2}, {&g4, &g1}, {&g4, &g0}, {&g2, &g1}};
    int overlaps_found[4] = {0, 0, 0, 0};
    //GeomPrimitive *prims[6] = {&g0, &g1, &g2, &g3, &g4, 0};
    //for (i2 = overlaps.begin(); i2 != overlaps.end(); i2++)
    //  std::cout << which_one((*i2).first, prims) << " overlaps " << which_one((*i2).second, prims) << std::endl;
    TS_ASSERT_EQUALS( overlaps.size(), num_overlaps );
    for (i2 = overlaps.begin(); i2 != overlaps.end(); i2++)
    {
      gt1 = (*i2).first; gt2 = (*i2).second;
      for (j = 0; j < num_overlaps; j++)
      {
        if ((gt1 == overlaps_truth[j][0] && gt2 == overlaps_truth[j][1]) || (gt1 == overlaps_truth[j][1] && gt2 == overlaps_truth[j][0]))
        {
          overlaps_found[j] = 1;
          break;
        }
      }
    }
    for (j = 0; j < num_overlaps; j++)
    {
      TS_ASSERT_EQUALS( overlaps_found[j], 1 );
    }
  }

  void test_spatial_tree_1d() {
    internal_test_spatial_tree(1);
  }

  void test_spatial_tree_2d() {
    internal_test_spatial_tree(2);
  }

}; // class TestSpatialTree
