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

#include "reone/graphics/context.h"
#include "reone/graphics/di/services.h"
#include "reone/graphics/framebuffer.h"
#include "reone/graphics/meshregistry.h"
#include "reone/graphics/shaderregistry.h"
#include "reone/graphics/textureregistry.h"
#include "reone/graphics/uniforms.h"
#include "reone/graphics/window.h"
#include "reone/system/exception/notimplemented.h"

namespace reone {

namespace graphics {

class MockContext : public IContext, boost::noncopyable {
public:
    MOCK_METHOD(void, clearColor, (glm::vec4 color), (override));
    MOCK_METHOD(void, clearDepth, (), (override));
    MOCK_METHOD(void, clearColorDepth, (glm::vec4 color), (override));

    MOCK_METHOD(void, resetProgram, (), (override));
    MOCK_METHOD(void, useProgram, (ShaderProgram &), (override));

    MOCK_METHOD(void, resetDrawFramebuffer, (), (override));
    MOCK_METHOD(void, resetReadFramebuffer, (), (override));
    MOCK_METHOD(void, bindDrawFramebuffer, (Framebuffer &, std::vector<int>), (override));
    MOCK_METHOD(void, bindReadFramebuffer, (Framebuffer &, std::optional<int>), (override));
    MOCK_METHOD(void, blitFramebuffer, (Framebuffer &, Framebuffer &, const glm::ivec4 &, const glm::ivec4 &, int, int, int, FramebufferBlitFilter), (override));

    MOCK_METHOD(void, bindUniformBuffer, (UniformBuffer &, int), (override));
    MOCK_METHOD(void, bindTexture, (Texture &, int), (override));

    MOCK_METHOD(FaceCullMode, faceCulling, (), (const override));
    MOCK_METHOD(BlendMode, blending, (), (const override));
    MOCK_METHOD(PolygonMode, polygonMode, (), (const override));

    MOCK_METHOD(void, pushFaceCulling, (FaceCullMode), (override));
    MOCK_METHOD(void, pushBlending, (BlendMode), (override));
    MOCK_METHOD(void, pushViewport, (glm::ivec4), (override));
    MOCK_METHOD(void, pushPolygonMode, (PolygonMode), (override));

    MOCK_METHOD(void, popFaceCulling, (), (override));
    MOCK_METHOD(void, popBlending, (), (override));
    MOCK_METHOD(void, popViewport, (), (override));
    MOCK_METHOD(void, popPolygonMode, (), (override));

    MOCK_METHOD(void, withDepthTest, (DepthTestMode mode, const std::function<void()> &), (override));
    MOCK_METHOD(void, withFaceCulling, (FaceCullMode mode, const std::function<void()> &), (override));
    MOCK_METHOD(void, withBlending, (BlendMode mode, const std::function<void()> &), (override));
    MOCK_METHOD(void, withPolygonMode, (PolygonMode mode, const std::function<void()> &), (override));
    MOCK_METHOD(void, withViewport, (glm::ivec4 viewport, const std::function<void()> &), (override));
    MOCK_METHOD(void, withScissorTest, (const glm::ivec4 &bounds, const std::function<void()> &), (override));
};

class MockMeshRegistry : public IMeshRegistry, boost::noncopyable {
public:
    MOCK_METHOD(Mesh &, get, (const std::string &), (override));
};

class MockShaderRegistry : public IShaderRegistry, boost::noncopyable {
public:
    MOCK_METHOD(ShaderProgram &, get, (const std::string &), (override));
};

class MockTextureRegistry : public ITextureRegistry, boost::noncopyable {
public:
    MOCK_METHOD(Texture &, get, (const std::string &), (override));
};

class MockUniforms : public IUniforms, boost::noncopyable {
public:
    MOCK_METHOD(void, setGlobals, (const std::function<void(GlobalUniforms &)> &), (override));
    MOCK_METHOD(void, setLocals, (const std::function<void(LocalUniforms &)> &), (override));
    MOCK_METHOD(void, setSceneGlobals, (const std::function<void(SceneGlobalUniforms &)> &), (override));
    MOCK_METHOD(void, setSceneLocals, (const std::function<void(SceneLocalUniforms &)> &), (override));
    MOCK_METHOD(void, setBones, (const std::function<void(BoneUniforms &)> &), (override));
    MOCK_METHOD(void, setParticles, (const std::function<void(ParticleUniforms &)> &), (override));
    MOCK_METHOD(void, setGrass, (const std::function<void(GrassUniforms &)> &), (override));
    MOCK_METHOD(void, setWalkmesh, (const std::function<void(WalkmeshUniforms &)> &), (override));
    MOCK_METHOD(void, setText, (const std::function<void(TextUniforms &)> &), (override));
    MOCK_METHOD(void, setScreenEffect, (const std::function<void(ScreenEffectUniforms &)> &), (override));
};

class MockWindow : public IWindow, boost::noncopyable {
public:
    MOCK_METHOD(void, processEvents, (bool &quit), (override));
    MOCK_METHOD(void, swapBuffers, (), (const override));
    MOCK_METHOD(bool, isInFocus, (), (const override));
    MOCK_METHOD(glm::mat4, getOrthoProjection, (float near, float far), (const override));
    MOCK_METHOD(void, setEventHandler, (IEventHandler * eventHandler), (override));
    MOCK_METHOD(void, setRelativeMouseMode, (bool enabled), (override));
    MOCK_METHOD(uint32_t, mouseState, (int *x, int *y), (override));
    MOCK_METHOD(void, showCursor, (bool show), (override));
};

class TestGraphicsModule : boost::noncopyable {
public:
    void init() {
        _context = std::make_unique<MockContext>();
        _meshRegistry = std::make_unique<MockMeshRegistry>();
        _shaderRegistry = std::make_unique<MockShaderRegistry>();
        _textureRegistry = std::make_unique<MockTextureRegistry>();
        _uniforms = std::make_unique<MockUniforms>();
        _window = std::make_unique<MockWindow>();

        _services = std::make_unique<GraphicsServices>(
            *_context,
            *_meshRegistry,
            *_shaderRegistry,
            *_textureRegistry,
            *_uniforms,
            *_window);
    }

    MockContext &context() {
        return *_context;
    }

    MockWindow &window() {
        return *_window;
    }

    MockShaderRegistry &shaderRegistry() {
        return *_shaderRegistry;
    }

    GraphicsServices &services() {
        return *_services;
    }

private:
    std::unique_ptr<MockContext> _context;
    std::unique_ptr<MockMeshRegistry> _meshRegistry;
    std::unique_ptr<MockShaderRegistry> _shaderRegistry;
    std::unique_ptr<MockTextureRegistry> _textureRegistry;
    std::unique_ptr<MockUniforms> _uniforms;
    std::unique_ptr<MockWindow> _window;

    std::unique_ptr<GraphicsServices> _services;
};

} // namespace graphics

} // namespace reone
