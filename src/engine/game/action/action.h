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

#include <boost/noncopyable.hpp>

#include "../types.h"

namespace reone {

namespace game {

class Action : boost::noncopyable {
public:
    Action(ActionType type, bool userAction = false) :
        _type(type),
        _userAction(userAction) {
    }

    virtual ~Action() = default;

    void complete() { _completed = true; }

    bool isCompleted() const { return _completed; }
    bool isUserAction() const { return _userAction; }

    ActionType type() const { return _type; }

protected:
    ActionType _type { ActionType::QueueEmpty };
    bool _completed { false };
    bool _userAction { false };
};

} // namespace game

} // namespace reone
