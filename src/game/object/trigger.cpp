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

#include "trigger.h"

#include "../../common/exception/validation.h"
#include "../../common/logutil.h"
#include "../../resource/gff.h"
#include "../../resource/gffs.h"
#include "../../resource/services.h"
#include "../../scene/graph.h"

using namespace std;

using namespace reone::resource;

namespace reone {

namespace game {

void Trigger::loadFromGit(const Gff &git) {
    // From GIT
    auto templateResRef = git.getString("TemplateResRef");
    auto xPosition = git.getFloat("XPosition");
    auto yPosition = git.getFloat("YPosition");
    auto zPosition = git.getFloat("ZPosition");
    auto geometry = git.getList("Geometry");
    for (auto &point : geometry) {
        auto pointX = point->getFloat("PointX");
        auto pointY = point->getFloat("PointY");
        auto pointZ = point->getFloat("PointZ");
        _geometry.push_back(glm::vec3(pointX, pointY, pointZ));
    }

    // From UTT
    auto utt = _resourceSvc.gffs.get(templateResRef, ResourceType::Utt);
    if (!utt) {
        throw ValidationException("UTT not found: " + templateResRef);
    }
    auto tag = utt->getString("Tag");
    auto scriptOnEnter = utt->getString("ScriptOnEnter");

    // Make scene node
    if (_geometry.size() >= 3ll) {
        auto triggerSceneNode = _sceneGraph->newTrigger(_geometry);
        triggerSceneNode->init();
        _sceneNode = triggerSceneNode.get();
    } else {
        warn("Invalid number of trigger points: " + to_string(_geometry.size()));
    }

    _tag = move(tag);
    _position = glm::vec3(xPosition, yPosition, zPosition);

    _scriptOnEnter = move(scriptOnEnter);

    //

    flushTransform();
}

} // namespace game

} // namespace reone
