/*
 * Copyright (c) 2013-2015, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author(s): Jeongseok Lee <jslee02@gmail.com>
 *
 * Georgia Tech Graphics Lab and Humanoid Robotics Lab
 *
 * Directed by Prof. C. Karen Liu and Prof. Mike Stilman
 * <karenliu@cc.gatech.edu> <mstilman@cc.gatech.edu>
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#include "dart/dynamics/TranslationalJoint.h"

#include <string>

#include "dart/math/Geometry.h"
#include "dart/math/Helpers.h"

namespace dart {
namespace dynamics {

//==============================================================================
TranslationalJoint::TranslationalJoint(const std::string& _name)
  : MultiDofJoint(_name)
{
  updateDegreeOfFreedomNames();
  updateLocalJacobian();
}

//==============================================================================
TranslationalJoint::~TranslationalJoint()
{
}

//==============================================================================
void TranslationalJoint::updateDegreeOfFreedomNames()
{
  if(!mDofs[0]->isNamePreserved())
    mDofs[0]->setName(mName + "_x", false);
  if(!mDofs[1]->isNamePreserved())
    mDofs[1]->setName(mName + "_y", false);
  if(!mDofs[2]->isNamePreserved())
    mDofs[2]->setName(mName + "_z", false);
}

//==============================================================================
void TranslationalJoint::updateLocalTransform() const
{
  mT = mT_ParentBodyToJoint
       * Eigen::Translation3d(getPositionsStatic())
       * mT_ChildBodyToJoint.inverse();

  // Verification
  assert(math::verifyTransform(mT));
}

//==============================================================================
void TranslationalJoint::updateLocalJacobian(bool _mandatory) const
{
  if(_mandatory)
  {
    Eigen::Vector6d J0;
    Eigen::Vector6d J1;
    Eigen::Vector6d J2;

    J0 << 0, 0, 0, 1, 0, 0;
    J1 << 0, 0, 0, 0, 1, 0;
    J2 << 0, 0, 0, 0, 0, 1;

    mJacobian.col(0) = math::AdT(mT_ChildBodyToJoint, J0);
    mJacobian.col(1) = math::AdT(mT_ChildBodyToJoint, J1);
    mJacobian.col(2) = math::AdT(mT_ChildBodyToJoint, J2);

    // Verification
    assert(!math::isNan(mJacobian));
  }
}

//==============================================================================
void TranslationalJoint::updateLocalJacobianTimeDeriv() const
{
  // Time derivative of translational joint is always zero
  assert(mJacobianDeriv == (Eigen::Matrix<double, 6, 3>::Zero()));
}

}  // namespace dynamics
}  // namespace dart
