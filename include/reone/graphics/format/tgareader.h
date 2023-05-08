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

#include "reone/resource/format/binreader.h"

#include "../types.h"

namespace reone {

namespace graphics {

class Texture;

class TgaReader : public resource::BinaryResourceReader {
public:
    TgaReader(std::string resRef, TextureUsage usage) :
        _resRef(std::move(resRef)),
        _usage(usage) {
    }

    std::shared_ptr<graphics::Texture> texture() const { return _texture; }

private:
    std::string _resRef;
    TextureUsage _usage;

    TGADataType _dataType {TGADataType::RGBA};
    int _width {0};
    int _height {0};
    int _numLayers {0};
    bool _alpha {false};

    std::shared_ptr<Texture> _texture;

    void onLoad() override;

    void loadTexture();

    ByteArray readPixels(int w, int h);
    ByteArray readPixelsRLE(int w, int h);

    bool isRGBA() const;
    bool isGrayscale() const;
    bool isRLE() const;
};

} // namespace graphics

} // namespace reone