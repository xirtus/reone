/*
 * Copyright � 2020 Vsevolod Kremianskii
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "cameranode.h"

#include "glm/ext.hpp"

namespace reone {

namespace render {

CameraSceneNode::CameraSceneNode(SceneGraph *sceneGraph, float fovy, float aspect, float zNear, float zFar) :
    SceneNode(sceneGraph), _fovy(fovy), _aspect(aspect), _zNear(zNear), _zFar(zFar) {

    _projection = glm::perspective(fovy, aspect, zNear, zFar);
}

void CameraSceneNode::updateAbsoluteTransform() {
    SceneNode::updateAbsoluteTransform();
    updateView();
}

void CameraSceneNode::updateView() {
    _view = glm::inverse(_absoluteTransform);
}

const glm::mat4 &CameraSceneNode::projection() const {
    return _projection;
}

const glm::mat4 &CameraSceneNode::view() const {
    return _view;
}

} // namespace render

} // namespace reone
