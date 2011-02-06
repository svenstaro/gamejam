# - Try to find the Bullet physics engine
#
#  This module defines the following variables
#
#  BULLET_FOUND - Was bullet found
#  BULLET_INCLUDE_DIRS - the Bullet include directories
#  BULLET_LIBRARIES - Link to this, by default it includes
#                     all bullet components (Dynamics,
#                     Collision, LinearMath, & SoftBody)

#=============================================================================
# Copyright 2011 Sven-Hendrik Haase <sh@lutzhaase.com>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distributed this file outside of CMake, substitute the full
#  License text for the above reference.)

FIND_PATH(BULLET_INCLUDE_DIR btBulletCollisionCommon.h
          PATH_SUFFIXES bullet)

FIND_LIBRARY(BULLET_COLLISION_LIBRARY NAMES BulletCollision )
FIND_LIBRARY(BULLET_DYNAMICS_LIBRARY NAMES BulletDynamics )
FIND_LIBRARY(BULLET_MULTITHREADED_LIBRARY NAMES BulletMultiThreaded )
FIND_LIBRARY(BULLET_SOFTBODY_LIBRARY NAMES BulletSoftBody )
FIND_LIBRARY(BULLET_SOFTBODYSOLVERS_CPU_LIBRARY NAMES BulletSoftBodySolvers_CPU )
FIND_LIBRARY(BULLET_CONVEXDECOMPOSITION_LIBRARY NAMES ConvexDecomposition )
FIND_LIBRARY(BULLET_GIMPACTUTILS_LIBRARY NAMES GIMPACTUtils )
FIND_LIBRARY(BULLET_LINEARMATH_LIBRARY NAMES LinearMath )
FIND_LIBRARY(BULLET_MINICL_LIBRARY NAMES MiniCL )
SET(BULLET_LIBRARIES ${BULLET_COLLISION_LIBRARY}
                   ${BULLET_DYNAMICS_LIBRARY}
                   ${BULLET_MULTITHREADED_LIBRARY}
                   ${BULLET_SOFTBODY_LIBRARY}
                   ${BULLET_SOFTBODYSOLVERS_CPU_LIBRARY}
                   ${BULLET_CONVEXDECOMPOSITION_LIBRARY}
                   ${BULLET_GIMPACTUTILS_LIBRARY}
                   ${BULLET_LINEARMATH_LIBRARY}
                   ${BULLET_MINICL_LIBRARY})

# handle the QUIETLY and REQUIRED arguments and set Bullet_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Bullet DEFAULT_MSG BULLET_COLLISION_LIBRARY BULLET_INCLUDE_DIR)

IF (BULLET_FOUND)
    SET(BULLET_INCLUDE_DIRS ${BULLET_INCLUDE_DIR})
ENDIF (BULLET_FOUND)

MARK_AS_ADVANCED(BULLET_INCLUDE_DIR BULLET_LIBRARIES)

