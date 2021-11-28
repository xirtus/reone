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

#include "soundhandle.h"

using namespace std;

namespace reone {

namespace audio {

void SoundHandle::stop() {
    _state = State::Stopped;
}

void SoundHandle::resetPositionDirty() {
    _positionDirty = false;
}

bool SoundHandle::isNotInited() const {
    return _state == State::NotInited;
}

bool SoundHandle::isStopped() const {
    return _state == State::Stopped;
}

void SoundHandle::setState(State state) {
    _state = state;
}

void SoundHandle::setPosition(glm::vec3 position) {
    if (_position == position) {
        return;
    }
    _position = move(position);
    _positionDirty = true;
}

} // namespace audio

} // namespace reone