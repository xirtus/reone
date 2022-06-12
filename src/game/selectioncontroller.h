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

#pragma once

namespace reone {

namespace scene {

class ISceneGraph;

}

namespace game {

class IMainInterface;

class Creature;
class Object;

class SelectionController : boost::noncopyable {
public:
    SelectionController(
        IMainInterface &mainInterface,
        scene::ISceneGraph &sceneGraph) :
        _mainInterface(mainInterface),
        _sceneGraph(sceneGraph) {
    }

    bool handle(const SDL_Event &e);

    Object *hoveredObject() {
        return _hoveredObject;
    }

    Object *selectedObject() {
        return _selectedObject;
    }

    void setPC(Creature *pc) {
        _pc = pc;
    }

    void setHoveredObject(Object *object) {
        _hoveredObject = object;
    }

    void setSelectedObject(Object *object) {
        _selectedObject = object;
    }

private:
    IMainInterface &_mainInterface;
    scene::ISceneGraph &_sceneGraph;

    Creature *_pc {nullptr};

    Object *_hoveredObject {nullptr};
    Object *_selectedObject {nullptr};
};

} // namespace game

} // namespace reone