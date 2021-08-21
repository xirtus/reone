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

/** @file
 *  Implementation of action-related routines.
 */

#include "declarations.h"

#include "../../../../common/log.h"

#include "../../../game.h"

#include "argutil.h"

using namespace std;

using namespace reone::script;

namespace reone {

namespace game {

namespace routine {

Variable actionRandomWalk(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: add action to caller
    return Variable();
}

Variable actionMoveToLocation(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto destination = getLocationEngineType(args, 0);
    bool run = getBool(args, 1, false);

    if (destination) {
        auto action = game.services().actionFactory().newMoveToLocation(move(destination), run);
        getCaller(game, ctx)->addAction(move(action));
    } else {
        debug("Script: actionMoveToLocation: destination is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionMoveToObject(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto moveTo = getObject(game, args, 0, ctx);
    bool run = getBool(args, 1, false);
    float range = getFloat(args, 2, 1.0f);

    if (moveTo) {
        auto action = game.services().actionFactory().newMoveToObject(move(moveTo), run, range);
        getCaller(game, ctx)->addAction(move(action));
    } else {
        debug("Script: actionMoveToObject: moveTo is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionMoveAwayFromObject(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto fleeFrom = getObject(game, args, 0, ctx);
    bool run = getBool(args, 1, false);
    float range = getFloat(args, 2, 40.0f);

    if (fleeFrom) {
        // TODO: add action to caller
    } else {
        debug("Script: actionMoveAwayFromObject: fleeFrom is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionEquipItem(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto item = getItem(game, args, 0, ctx);
    int inventorySlot = getInt(args, 1);
    bool instant = getBool(args, 2, false);

    if (item) {
        // TODO: add action to caller
    } else {
        debug("Script: actionEquipItem: item is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionUnequipItem(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto item = getItem(game, args, 0, ctx);
    bool instant = getBool(args, 1, false);

    if (item) {
        // TODO: add action to caller
    } else {
        debug("Script: actionUnequipItem: item is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionPickUpItem(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto item = getItem(game, args, 0, ctx);
    if (item) {
        // TODO: add action to caller
    } else {
        debug("Script: actionPickUpItem: item is invalid", 1, DebugChannels::script);
    }
    return Variable();
}

Variable actionPutDownItem(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto item = getItem(game, args, 0, ctx);
    if (item) {
        // TODO: add action to caller
    } else {
        debug("Script: actionPutDownItem: item is invalid", 1, DebugChannels::script);
    }
    return Variable();
}

Variable actionAttack(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: pass all arguments to an action
    auto caller = getCallerAsCreature(game, ctx);
    auto attackee = getSpatialObject(game, args, 0, ctx);
    bool passive = getBool(args, 1, false);

    if (caller && attackee) {
        auto action = game.services().actionFactory().newAttack(attackee, caller->getAttackRange());
        caller->addAction(move(action));
    } else if (!caller) {
        debug("Script: actionAttack: caller is invalid", 1, DebugChannels::script);
    } else if (!attackee) {
        debug("Script: actionAttack: attackee is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionSpeakString(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    string toSpeak(getString(args, 0));
    auto talkVolume = getEnum(args, 1, TalkVolume::Talk);
    
    // TODO: add action to caller

    return Variable();
}

Variable actionPlayAnimation(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto animation = getEnum<AnimationType>(args, 0);
    float speed = getFloat(args, 1, 1.0f);
    float duration = getFloat(args, 2, 0.0f);

    auto action = game.services().actionFactory().newPlayAnimation(animation, speed, duration);
    getCaller(game, ctx)->addAction(move(action));

    return Variable();
}

Variable actionOpenDoor(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto door = getObject(game, args, 0, ctx);
    if (door) {
        auto action = game.services().actionFactory().newOpenDoor(door);
        getCaller(game, ctx)->addAction(move(action));
    } else {
        debug("Script: actionOpenDoor: door is invalid", 1, DebugChannels::script);
    }
    return Variable();
}

Variable actionCloseDoor(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto door = getObject(game, args, 0, ctx);
    if (door) {
        auto action = game.services().actionFactory().newCloseDoor(door);
        getCaller(game, ctx)->addAction(move(action));
    } else {
        debug("Script: actionCloseDoor: door is invalid", 1, DebugChannels::script);
    }
    return Variable();
}

Variable actionCastSpellAtObject(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto spell = getEnum<ForcePower>(args, 0);
    auto target = getObject(game, args, 1, ctx);
    int metaMagic = getInt(args, 2, 0);
    bool cheat = getBool(args, 3, false);
    int domainLevel = getInt(args, 4, 0);
    auto projectilePathType = getEnum(args, 5, ProjectilePathType::Default);
    bool instantSpell = getBool(args, 6, false);

    // TODO: add action to caller

    return Variable();
}

Variable actionGiveItem(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto item = getItem(game, args, 0, ctx);
    auto giveTo = getObject(game, args, 1, ctx);

    if (item && giveTo) {
        // TODO: add action to caller
    } else if (!item) {
        debug("Script: actionGiveItem: item is invalid", 1, DebugChannels::script);
    } else if (!giveTo) {
        debug("Script: actionGiveItem: giveTo is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionTakeItem(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto item = getItem(game, args, 0, ctx);
    auto takeFrom = getObject(game, args, 1, ctx);

    if (item && takeFrom) {
        // TODO: add action to caller
    } else if (!item) {
        debug("Script: actionTakeItem: item is invalid", 1, DebugChannels::script);
    } else if (!takeFrom) {
        debug("Script: actionTakeItem: takeFrom is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionForceFollowObject(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto follow = getObject(game, args, 0, ctx);
    float followDistance = getFloat(args, 1, 0.0f);

    if (follow) {
        // TODO: add action to caller
    } else {
        debug("Script: actionForceFollowObject: follow is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionJumpToObject(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: pass all arguments to an action
    auto jumpTo = getObject(game, args, 0, ctx);
    bool walkStraightLine = getBool(args, 1, true);

    if (jumpTo) {
        auto action = game.services().actionFactory().newJumpToObject(move(jumpTo));
        getCaller(game, ctx)->addAction(move(action));
    } else {
        debug("Script: actionJumpToObject: jumpTo is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionWait(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    float seconds = getFloat(args, 0);

    auto action = game.services().actionFactory().newWait(seconds);
    getCaller(game, ctx)->addAction(move(action));

    return Variable();
}

Variable actionStartConversation(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: figure out all arguments
    auto objectToConverse = getObject(game, args, 0, ctx);
    string dialogResRef(getString(args, 1, ""));
    bool ignoreStartRange = getBool(args, 4, false);

    if (objectToConverse) {
        auto caller = getCaller(game, ctx);
        if (dialogResRef.empty()) {
            dialogResRef = caller->conversation();
        }
        auto action = game.services().actionFactory().newStartConversation(move(objectToConverse), move(dialogResRef), ignoreStartRange);
        caller->addAction(move(action));
    } else {
        debug("Script: actionStartConversation: objectToConverse is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionPauseConversation(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto action = game.services().actionFactory().newPauseConversation();
    getCaller(game, ctx)->addAction(move(action));
    return Variable();
}

Variable actionResumeConversation(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto action = game.services().actionFactory().newResumeConversation();
    getCaller(game, ctx)->addAction(move(action));
    return Variable();
}

Variable actionJumpToLocation(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto location = getLocationEngineType(args, 0);
    if (location) {
        auto action = game.services().actionFactory().newJumpToLocation(move(location));
        getCaller(game, ctx)->addAction(move(action));
    } else {
        debug("Script: actionJumpToLocation: location is invalid", 1, DebugChannels::script);
    }
    return Variable();
}

Variable actionCastSpellAtLocation(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto spell = getEnum<ForcePower>(args, 0);
    auto targetLocation = getLocationEngineType(args, 1);
    int metaMagic = getInt(args, 2, 0);
    bool cheat = getBool(args, 3, false);
    auto projectilePathType = getEnum(args, 4, ProjectilePathType::Default);
    bool instantSpell = getBool(args, 5, false);

    if (targetLocation) {
        // TODO: add action to caller
    } else {
        debug("Script: actionCastSpellAtLocation: targetLocation is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionSpeakStringByStrRef(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    int strRef = getInt(args, 0);
    auto talkVolume = getEnum(args, 1, TalkVolume::Talk);

    // TODO: add action to caller

    return Variable();
}

Variable actionUseFeat(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: pass all arguments to an action
    auto feat = getEnum<FeatType>(args, 0);
    auto target = getObject(game, args, 1, ctx);

    if (target) {
        auto action = game.services().actionFactory().newUseFeat(target, feat);
        getCaller(game, ctx)->addAction(move(action));
    } else {
        debug("Script: actionUseFeat: target is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionUseSkill(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: pass all arguments to an action
    auto skill = getEnum<SkillType>(args, 0);
    auto target = getObject(game, args, 1, ctx);
    int subSkill = getInt(args, 2, 0);
    auto itemUsed = getObject(game, args, 3, ctx);

    if (target) {
        auto action = game.services().actionFactory().newUseSkill(target, skill);
        getCaller(game, ctx)->addAction(move(action));
    } else {
        debug("Script: actionUseSkill: target is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionDoCommand(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto action = getAction(args, 0);

    auto commandAction = game.services().actionFactory().newDoCommand(move(action));
    getCaller(game, ctx)->addAction(move(commandAction));

    return Variable();
}

Variable actionUseTalentOnObject(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto chosenTalen = getTalent(args, 0);
    auto target = getObject(game, args, 1, ctx);

    if (target) {
        // TODO: add action to caller
    } else {
        debug("Script: actionUseTalentOnObject: target is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionUseTalentAtLocation(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto chosenTalen = getTalent(args, 0);
    auto targetLocation = getLocationEngineType(args, 1);

    if (targetLocation) {
        // TODO: add action to caller
    } else {
        debug("Script: actionUseTalentAtLocation: targetLocation is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionInteractObject(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: add action to caller
    return Variable();
}

Variable actionMoveAwayFromLocation(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: add action to caller
    return Variable();
}

Variable actionSurrenderToEnemies(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: add action to caller
    return Variable();
}

Variable actionForceMoveToLocation(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: pass all arguments to an action
    auto destination = getLocationEngineType(args, 0);
    bool run = getBool(args, 1, false);
    float timeout = getFloat(args, 2, 30.0f);

    if (destination) {
        auto action = game.services().actionFactory().newMoveToLocation(move(destination));
        getCaller(game, ctx)->addAction(move(action));
    } else {
        debug("Script: actionForceMoveToLocation: destination is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionForceMoveToObject(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: pass all arguments to an action
    auto moveTo = getObject(game, args, 0, ctx);
    bool run = getBool(args, 1, false);
    float range = getFloat(args, 2, 1.0f);
    float timeout = getFloat(args, 3, 30.0f);

    if (moveTo) {
        auto action = game.services().actionFactory().newMoveToObject(move(moveTo), run, range);
        getCaller(game, ctx)->addAction(move(action));
    } else {
        debug("Script: actionForceMoveToObject: moveTo is invalid", 1, DebugChannels::script);
    }

    return Variable();
}

Variable actionEquipMostDamagingMelee(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: add action to caller
    return Variable();
}

Variable actionEquipMostDamagingRanged(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: add action to caller
    return Variable();
}

Variable actionEquipMostEffectiveArmor(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: add action to caller
    return Variable();
}

Variable actionUnlockObject(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: add action to caller
    return Variable();
}

Variable actionLockObject(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: add action to caller
    return Variable();
}

Variable actionCastFakeSpellAtObject(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: add action to caller
    return Variable();
}

Variable actionCastFakeSpellAtLocation(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: add action to caller
    return Variable();
}

Variable actionBarkString(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    int strRef = getInt(args, 0);
    // TODO: add action to caller
    return Variable();
}

Variable actionFollowLeader(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    auto action = game.services().actionFactory().newFollowLeader();
    getCaller(game, ctx)->addAction(move(action));
    return Variable();
}

Variable actionFollowOwner(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    float range = getFloat(args, 0, 2.5f);
    // TODO: add action to caller
    return Variable();
}

Variable actionSwitchWeapons(Game &game, const vector<Variable> &args, ExecutionContext &ctx) {
    // TODO: add action to caller
    return Variable();
}

} // namespace routine

} // namespace game

} // namespace reone
