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

#pragma once

#include "object/creature.h"
#include "object/door.h"
#include "object/object.h"
#include "object/placeable.h"
#include "types.h"

namespace reone {

namespace game {

struct ObjectConverter {
    static inline std::shared_ptr<Creature> toCreature(const std::shared_ptr<Object> &object) {
        return object && object->type() == ObjectType::Creature ? std::static_pointer_cast<Creature>(object) : nullptr;
    }

    static inline std::shared_ptr<Door> toDoor(const std::shared_ptr<Object> &object) {
        return object && object->type() == ObjectType::Door ? std::static_pointer_cast<Door>(object) : nullptr;
    }

    static inline std::shared_ptr<Placeable> toPlaceable(const std::shared_ptr<Object> &object) {
        return object && object->type() == ObjectType::Placeable ? std::static_pointer_cast<Placeable>(object) : nullptr;
    }
};

} // namespace game

} // namespace reone
