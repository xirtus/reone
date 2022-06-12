/*
 * Copyright (c) 2020-2022 The reone project contributors
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

#include "object.h"

#include "../graphics/services.h"
#include "../graphics/window.h"
#include "../scene/graph.h"

#include "options.h"

namespace reone {

namespace game {

glm::ivec3 Object::screenCoords() const {
    auto camera = _sceneGraph->activeCamera();
    auto viewport = glm::ivec4(0, 0, _graphicsOpt.width, _graphicsOpt.height);
    auto screenCoords = glm::project(
        _position,
        camera->camera()->view(),
        camera->camera()->projection(),
        viewport);

    return glm::ivec3(screenCoords.x, _graphicsOpt.height - screenCoords.y, screenCoords.z);
}

} // namespace game

} // namespace reone