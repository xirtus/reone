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

#include <gmock/gmock.h>

#include "reone/scene/di/services.h"

namespace reone {

namespace scene {

class MockSceneGraphs : public ISceneGraphs, boost::noncopyable {
public:
    MOCK_METHOD(void, reserve, (std::string name), (override));
    MOCK_METHOD(SceneGraph &, get, (const std::string &name), (override));
    MOCK_METHOD(std::set<std::string>, sceneNames, (), (const override));
};

class TestSceneModule : boost::noncopyable {
public:
    void init() {
        _graphs = std::make_unique<MockSceneGraphs>();

        _services = std::make_unique<SceneServices>(*_graphs);
    }

    MockSceneGraphs &graphs() {
        return *_graphs;
    }

    SceneServices &services() {
        return *_services;
    }

private:
    std::unique_ptr<MockSceneGraphs> _graphs;

    std::unique_ptr<SceneServices> _services;
};

} // namespace scene

} // namespace reone
