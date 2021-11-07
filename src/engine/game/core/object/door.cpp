/*
 * Copyright (c) 2020-2021 The reone project contributors
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

#include "door.h"

#include "../../../common/streamutil.h"
#include "../../../graphics/model/models.h"
#include "../../../graphics/walkmesh/walkmeshes.h"
#include "../../../resource/2da.h"
#include "../../../resource/resources.h"
#include "../../../resource/strings.h"
#include "../../../scene/node/model.h"
#include "../../../scene/types.h"
#include "../../../script/scripts.h"

#include "../game.h"

using namespace std;

using namespace reone::graphics;
using namespace reone::resource;
using namespace reone::scene;
using namespace reone::script;

namespace reone {

namespace game {

void Door::loadFromGIT(const GffStruct &gffs) {
    string templateResRef(boost::to_lower_copy(gffs.getString("TemplateResRef")));
    loadFromBlueprint(templateResRef);

    _linkedToModule = boost::to_lower_copy(gffs.getString("LinkedToModule"));
    _linkedTo = boost::to_lower_copy(gffs.getString("LinkedTo"));
    _linkedToFlags = gffs.getInt("LinkedToFlags");
    _transitionDestin = _strings.get(gffs.getInt("TransitionDestin"));

    loadTransformFromGIT(gffs);
}

void Door::loadFromBlueprint(const string &resRef) {
    shared_ptr<GffStruct> utd(_resources.getGFF(resRef, ResourceType::Utd));
    if (!utd)
        return;

    loadUTD(*utd);

    shared_ptr<TwoDA> doors(_resources.get2DA("genericdoors"));
    string modelName(boost::to_lower_copy(doors->getString(_genericType, "modelname")));

    auto model = _sceneGraph.newModel(_models.get(modelName), ModelUsage::Door);
    model->setCullable(true);
    model->setDrawDistance(FLT_MAX);

    _sceneNode = move(model);

    _closedWalkmesh = _walkmeshes.get(modelName + "0", ResourceType::Dwk);
    _open1Walkmesh = _walkmeshes.get(modelName + "1", ResourceType::Dwk);
    _open2Walkmesh = _walkmeshes.get(modelName + "2", ResourceType::Dwk);
}

void Door::loadTransformFromGIT(const GffStruct &gffs) {
    _position[0] = gffs.getFloat("X");
    _position[1] = gffs.getFloat("Y");
    _position[2] = gffs.getFloat("Z");

    _orientation = glm::quat(glm::vec3(0.0f, 0.0f, gffs.getFloat("Bearing")));

    updateTransform();
}

bool Door::isSelectable() const {
    return !_static && !_open;
}

void Door::open(const shared_ptr<Object> &triggerrer) {
    auto model = static_pointer_cast<ModelSceneNode>(_sceneNode);
    if (model) {
        //model->setDefaultAnimation("opened1", AnimationProperties::fromFlags(AnimationFlags::loop));
        model->playAnimation("opening1");
    }
    _open = true;
}

void Door::close(const shared_ptr<Object> &triggerrer) {
    auto model = static_pointer_cast<ModelSceneNode>(_sceneNode);
    if (model) {
        //model->setDefaultAnimation("closed", AnimationProperties::fromFlags(AnimationFlags::loop));
        model->playAnimation("closing1");
    }
    _open = false;
}

shared_ptr<Walkmesh> Door::getWalkmesh() const {
    return _open ? _open1Walkmesh : _closedWalkmesh;
}

void Door::setLocked(bool locked) {
    _locked = locked;
}

void Door::loadUTD(const GffStruct &utd) {
    _tag = boost::to_lower_copy(utd.getString("Tag"));
    _name = _strings.get(utd.getInt("LocName"));
    _blueprintResRef = boost::to_lower_copy(utd.getString("TemplateResRef"));
    _autoRemoveKey = utd.getBool("AutoRemoveKey");
    _conversation = boost::to_lower_copy(utd.getString("Conversation"));
    _interruptable = utd.getBool("Interruptable");
    _faction = utd.getEnum("Faction", Faction::Invalid);
    _plot = utd.getBool("Plot");
    _minOneHP = utd.getBool("Min1HP");
    _keyRequired = utd.getBool("KeyRequired");
    _lockable = utd.getBool("Lockable");
    _locked = utd.getBool("Locked");
    _openLockDC = utd.getInt("OpenLockDC");
    _keyName = utd.getString("KeyName");
    _hitPoints = utd.getInt("HP");
    _currentHitPoints = utd.getInt("CurrentHP");
    _hardness = utd.getInt("Hardness");
    _fortitude = utd.getInt("Fort");
    _genericType = utd.getInt("GenericType");
    _static = utd.getBool("Static");

    _onClosed = utd.getString("OnClosed");   // always empty, but could be useful
    _onDamaged = utd.getString("OnDamaged"); // always empty, but could be useful
    _onDeath = utd.getString("OnDeath");
    _onHeartbeat = utd.getString("OnHeartbeat");
    _onLock = utd.getString("OnLock");                   // always empty, but could be useful
    _onMeleeAttacked = utd.getString("OnMeleeAttacked"); // always empty, but could be useful
    _onOpen = utd.getString("OnOpen");
    _onSpellCastAt = utd.getString("OnSpellCastAt"); // always empty, but could be useful
    _onUnlock = utd.getString("OnUnlock");           // always empty, but could be useful
    _onUserDefined = utd.getString("OnUserDefined");
    _onClick = utd.getString("OnClick");
    _onFailToOpen = utd.getString("OnFailToOpen");

    // Unused fields:
    //
    // - Description (always -1)
    // - CloseLockDC (always 0)
    // - PortraitId (not applicable, mostly 0)
    // - TrapDetectable (not applicable, always 1)
    // - TrapDetectDC (not applicable, always 0)
    // - TrapDisarmable (not applicable, always 1)
    // - DisarmDC (not applicable, mostly 28)
    // - TrapFlag (not applicable, always 0)
    // - TrapOneShot (not applicable, always 1)
    // - TrapType (not applicable)
    // - AnimationState (always 0)
    // - Appearance (always 0)
    // - Ref (always 0)
    // - Will (always 0)
    // - OnDisarm (not applicable, always empty)
    // - OnTrapTriggered (not applicable, always empty)
    // - LoadScreenID (always 0)
    // - PaletteID (toolset only)
    // - Comment (toolset only)
}

} // namespace game

} // namespace reone