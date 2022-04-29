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

#include "aabb.h"

using namespace std;

namespace reone {

namespace graphics {

AABB::AABB(const glm::vec3 &min, const glm::vec3 &max) :
    _empty(false), _min(min), _max(max) {
    updateTransform();
}

AABB AABB::operator*(const glm::mat4 &m) const {
    AABB aabb;
    if (!_empty) {
        glm::vec3 a(m * glm::vec4(_min, 1.0f));
        glm::vec3 b(m * glm::vec4(_max, 1.0f));

        glm::vec3 min(
            glm::min(a.x, b.x),
            glm::min(a.y, b.y),
            glm::min(a.z, b.z));

        glm::vec3 max(
            glm::max(a.x, b.x),
            glm::max(a.y, b.y),
            glm::max(a.z, b.z));

        aabb = AABB(min, max);
    }

    return std::move(aabb);
}

void AABB::reset() {
    _min = glm::vec3(0.0f, 0.0f, 0.0f);
    _max = glm::vec3(0.0f, 0.0f, 0.0f);
    _empty = true;

    updateTransform();
}

void AABB::expand(const glm::vec3 &p) {
    if (_empty) {
        _min.x = p.x;
        _min.y = p.y;
        _min.z = p.z;
        _max = _min;
        _empty = false;
    } else {
        if (p.x < _min.x)
            _min.x = p.x;
        if (p.y < _min.y)
            _min.y = p.y;
        if (p.z < _min.z)
            _min.z = p.z;
        if (p.x > _max.x)
            _max.x = p.x;
        if (p.y > _max.y)
            _max.y = p.y;
        if (p.z > _max.z)
            _max.z = p.z;
    }

    updateTransform();
}

void AABB::expand(const AABB &aabb) {
    if (_empty) {
        _min = aabb._min;
        _max = aabb._max;
        _empty = false;

    } else {
        if (aabb._min.x < _min.x)
            _min.x = aabb._min.x;
        if (aabb._min.y < _min.y)
            _min.y = aabb._min.y;
        if (aabb._min.z < _min.z)
            _min.z = aabb._min.z;
        if (aabb._max.x > _max.x)
            _max.x = aabb._max.x;
        if (aabb._max.y > _max.y)
            _max.y = aabb._max.y;
        if (aabb._max.z > _max.z)
            _max.z = aabb._max.z;
    }
    updateTransform();
}

void AABB::updateTransform() {
    glm::vec3 size(glm::abs(_max - _min));
    _center = 0.5f * (_min + _max);

    _transform = glm::translate(glm::mat4(1.0f), _center);
    _transform = glm::scale(_transform, size);
}

bool AABB::contains(const glm::vec2 &p) const {
    return p.x >= _min.x && p.x <= _max.x &&
           p.y >= _min.y && p.y <= _max.y;
}

bool AABB::contains(const glm::vec3 &p) const {
    return p.x >= _min.x && p.x <= _max.x &&
           p.y >= _min.y && p.y <= _max.y &&
           p.z >= _min.z && p.z <= _max.z;
}

bool AABB::intersect(const AABB &other) const {
    return (_min.x <= other._max.x && _max.x >= other._min.x) &&
           (_min.y <= other._max.y && _max.y >= other._min.y) &&
           (_min.z <= other._max.z && _max.z >= other._min.z);
}

bool AABB::raycast(const glm::vec3 &origin, const glm::vec3 &dir, float maxDistance, float &outDistance) const {
    glm::vec3 invDir(1.0f / dir);
    glm::vec3 t1((_min - origin) * invDir);
    glm::vec3 t2((_max - origin) * invDir);

    float tmin = glm::min(t1.x, t2.x);
    float tmax = glm::max(t1.x, t2.x);

    tmin = glm::max(tmin, glm::min(t1.y, t2.y));
    tmax = glm::min(tmax, glm::max(t1.y, t2.y));
    tmin = glm::max(tmin, glm::min(t1.z, t2.z));
    tmax = glm::min(tmax, glm::max(t1.z, t2.z));

    if (tmax < glm::max(0.0f, tmin)) {
        return false;
    }
    if (tmin >= maxDistance) {
        return false;
    }

    outDistance = tmin;
    return true;
}

glm::vec3 AABB::size() const {
    return _max - _min;
}

} // namespace graphics

} // namespace reone
