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

#include <memory>
#include <string>
#include <unordered_map>

#include "lipanimation.h"

namespace reone {

namespace render {

class Lips {
public:
    static Lips &instance();

    void invalidateCache();

    std::shared_ptr<LipAnimation> get(const std::string &resRef);

private:
    std::unordered_map<std::string, std::shared_ptr<LipAnimation>> _cache;

    Lips() = default;
};

} // namespace render

} // namespace reone