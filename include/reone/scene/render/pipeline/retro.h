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

#include "../pipeline.h"

namespace reone {

namespace scene {

class RetroRenderPipeline : public RenderPipelineBase {
public:
    RetroRenderPipeline(glm::ivec2 targetSize,
                        graphics::GraphicsOptions &options,
                        graphics::Context &context,
                        graphics::MeshRegistry &meshRegistry,
                        graphics::ShaderRegistry &shaderRegistry,
                        graphics::TextureRegistry &textureRegistry,
                        graphics::Uniforms &uniforms) :
        RenderPipelineBase(
            std::move(targetSize),
            options,
            context,
            meshRegistry,
            shaderRegistry,
            textureRegistry,
            uniforms) {
    }

    void init() override;

    graphics::Texture &render() override;

private:
    struct RenderTargets {
        std::shared_ptr<graphics::Texture> outputColor;
        std::shared_ptr<graphics::Renderbuffer> outputDepth;
        std::shared_ptr<graphics::Framebuffer> output;
    };

    RenderTargets _targets;

    void initRenderTargets();
};

} // namespace scene

} // namespace reone
