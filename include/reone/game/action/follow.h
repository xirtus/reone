/*
 * Copyright (c) 2020-2023 The reone project contributors
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

#include "objectaction.h"

namespace reone {

namespace game {

class FollowAction : public ObjectAction {
public:
    FollowAction(Game &game, ServicesView &services, std::shared_ptr<Object> object, float distance) :
        ObjectAction(game, services, ActionType::Follow, std::move(object)),
        _distance(distance) {
    }

    void execute(std::shared_ptr<Action> self, Object &actor, float dt) override;

    float distance() const { return _distance; }

private:
    float _distance {0.0f};
};

} // namespace game

} // namespace reone