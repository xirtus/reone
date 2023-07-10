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

#include "../action.h"
#include "../location.h"

namespace reone {

namespace game {

class MoveToLocationAction : public Action {
public:
    MoveToLocationAction(Game &game,
                         ServicesView &services,
                         std::shared_ptr<Location> destination,
                         bool run,
                         bool force,
                         float timeout) :
        Action(game, services, ActionType::MoveToLocation),
        _destination(std::move(destination)),
        _run(run),
        _force(force),
        _timeout(timeout) {
    }

    void execute(std::shared_ptr<Action> self, Object &actor, float dt) override;

    bool isRun() const { return _run; }

private:
    std::shared_ptr<Location> _destination;
    bool _run;
    bool _force;
    float _timeout;
};

} // namespace game

} // namespace reone
