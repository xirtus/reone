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

#include "reone/graphics/pipeline.h"

#include "reone/graphics/camera/perspective.h"
#include "reone/graphics/context.h"
#include "reone/graphics/mesh.h"
#include "reone/graphics/meshregistry.h"
#include "reone/graphics/renderbuffer.h"
#include "reone/graphics/shaderregistry.h"
#include "reone/graphics/texture.h"
#include "reone/graphics/textureregistry.h"
#include "reone/graphics/textureutil.h"
#include "reone/graphics/uniforms.h"
#include "reone/graphics/window.h"
#include "reone/scene/node/light.h"
#include "reone/system/checkutil.h"
#include "reone/system/exception/notimplemented.h"
#include "reone/system/randomutil.h"
#include "reone/system/threadutil.h"

#define R_GAUSSIAN_BLUR_HORIZONTAL false
#define R_GAUSSIAN_BLUR_VERTICAL true
#define R_GAUSSIAN_BLUR_STRONG true

#define R_MEDIAN_FILTER_STRONG true

namespace reone {

namespace graphics {

static constexpr GLenum kColorAttachments[] {
    GL_COLOR_ATTACHMENT0,
    GL_COLOR_ATTACHMENT1,
    GL_COLOR_ATTACHMENT2,
    GL_COLOR_ATTACHMENT3,
    GL_COLOR_ATTACHMENT4,
    GL_COLOR_ATTACHMENT5,
    GL_COLOR_ATTACHMENT6,
    GL_COLOR_ATTACHMENT7,
    GL_COLOR_ATTACHMENT8};

void Pipeline::init() {
    checkMainThread();

    // SSAO
    _uniforms.setScreenEffect([](auto &screenEffect) {
        for (int i = 0; i < kNumSSAOSamples; ++i) {
            float scale = i / static_cast<float>(kNumSSAOSamples);
            scale = glm::mix(0.1f, 1.0f, scale * scale);
            auto sample = glm::vec3(randomFloat(-1.0f, 1.0f), randomFloat(-1.0f, 1.0f), randomFloat(0.0f, 1.0f));
            sample = glm::normalize(sample);
            sample *= scale;
            screenEffect.ssaoSamples[i] = glm::vec4(std::move(sample), 0.0f);
        }
    });
}

void Pipeline::initTargets(glm::ivec2 size) {
    RenderTargets targets;
    auto halfSize = size / 2;

    // Reusable targets

    targets.dbCommon = std::make_unique<Renderbuffer>();
    targets.dbCommon->configure(size.x, size.y, PixelFormat::Depth32F);
    targets.dbCommon->init();

    targets.dbCommonHalf = std::make_unique<Renderbuffer>();
    targets.dbCommonHalf->configure(halfSize.x, halfSize.y, PixelFormat::Depth32F);
    targets.dbCommonHalf->init();

    targets.cbPing = std::make_unique<Texture>("ping_color", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbPing->clear(size.x, size.y, PixelFormat::RGBA8);
    targets.cbPing->init();

    targets.fbPing = std::make_unique<Framebuffer>();
    targets.fbPing->attachColorDepth(targets.cbPing, targets.dbCommon);
    targets.fbPing->init();

    targets.cbPingHalf = std::make_unique<Texture>("ping_color_half", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbPingHalf->clear(halfSize.x, halfSize.y, PixelFormat::RGBA8);
    targets.cbPingHalf->init();

    targets.fbPingHalf = std::make_unique<Framebuffer>();
    targets.fbPingHalf->attachColorDepth(targets.cbPingHalf, targets.dbCommonHalf);
    targets.fbPingHalf->init();

    targets.cbPong = std::make_unique<Texture>("pong_color", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbPong->clear(size.x, size.y, PixelFormat::RGBA8);
    targets.cbPong->init();

    targets.fbPong = std::make_unique<Framebuffer>();
    targets.fbPong->attachColorDepth(targets.cbPong, targets.dbCommon);
    targets.fbPong->init();

    targets.cbPongHalf = std::make_unique<Texture>("pong_color_half", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbPongHalf->clear(halfSize.x, halfSize.y, PixelFormat::RGBA8);
    targets.cbPongHalf->init();

    targets.fbPongHalf = std::make_unique<Framebuffer>();
    targets.fbPongHalf->attachColorDepth(targets.cbPongHalf, targets.dbCommonHalf);
    targets.fbPongHalf->init();

    // Directional light shadows framebuffer

    targets.dbDirectionalLightShadows = std::make_unique<Texture>("point_light_shadows_color", getTextureProperties(TextureUsage::DepthBuffer));
    targets.dbDirectionalLightShadows->clear(_options.shadowResolution, _options.shadowResolution, PixelFormat::Depth32F, kNumShadowCascades);
    targets.dbDirectionalLightShadows->init();

    targets.fbDirLightShadows = std::make_shared<Framebuffer>();
    targets.fbDirLightShadows->attachDepth(targets.dbDirectionalLightShadows);
    targets.fbDirLightShadows->init();

    // Point light shadows framebuffer

    targets.dbPointLightShadows = std::make_unique<Texture>("directional_light_shadows_color", getTextureProperties(TextureUsage::DepthBuffer));
    targets.dbPointLightShadows->setCubemap(true);
    targets.dbPointLightShadows->clear(_options.shadowResolution, _options.shadowResolution, PixelFormat::Depth32F);
    targets.dbPointLightShadows->init();

    targets.fbPointLightShadows = std::make_shared<Framebuffer>();
    targets.fbPointLightShadows->attachDepth(targets.dbPointLightShadows);
    targets.fbPointLightShadows->init();

    // G-Buffer framebuffer

    targets.cbGBufferDiffuse = std::make_unique<Texture>("gbuffer_color_diffuse", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbGBufferDiffuse->clear(size.x, size.y, PixelFormat::RGBA8);
    targets.cbGBufferDiffuse->init();

    targets.cbGBufferLightmap = std::make_unique<Texture>("gbuffer_color_lightmap", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbGBufferLightmap->clear(size.x, size.y, PixelFormat::RGBA8);
    targets.cbGBufferLightmap->init();

    targets.cbGBufferEnvMap = std::make_unique<Texture>("gbuffer_color_envmap", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbGBufferEnvMap->clear(size.x, size.y, PixelFormat::RGBA8);
    targets.cbGBufferEnvMap->init();

    targets.cbGBufferSelfIllum = std::make_unique<Texture>("gbuffer_color_selfillum", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbGBufferSelfIllum->clear(size.x, size.y, PixelFormat::RGB8);
    targets.cbGBufferSelfIllum->init();

    targets.cbGBufferFeatures = std::make_unique<Texture>("gbuffer_color_features", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbGBufferFeatures->clear(size.x, size.y, PixelFormat::RG8);
    targets.cbGBufferFeatures->init();

    targets.cbGBufferEyePos = std::make_unique<Texture>("gbuffer_color_eyepos", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbGBufferEyePos->clear(size.x, size.y, PixelFormat::RGB16F);
    targets.cbGBufferEyePos->init();

    targets.cbGBufferEyeNormal = std::make_unique<Texture>("gbuffer_color_eyenormal", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbGBufferEyeNormal->clear(size.x, size.y, PixelFormat::RGB8);
    targets.cbGBufferEyeNormal->init();

    targets.dbGBuffer = std::make_shared<Renderbuffer>();
    targets.dbGBuffer->configure(size.x, size.y, PixelFormat::Depth32F);
    targets.dbGBuffer->init();

    targets.fbOpaqueGeometry = std::make_shared<Framebuffer>();
    targets.fbOpaqueGeometry->attachColorsDepth(
        {targets.cbGBufferDiffuse,
         targets.cbGBufferLightmap,
         targets.cbGBufferEnvMap,
         targets.cbGBufferSelfIllum,
         targets.cbGBufferFeatures,
         targets.cbGBufferEyePos,
         targets.cbGBufferEyeNormal},
        targets.dbGBuffer);
    targets.fbOpaqueGeometry->init();

    // Opaque geometry framebuffer

    targets.cbDeferredOpaque1 = std::make_unique<Texture>("deferred_opaque_color1", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbDeferredOpaque1->clear(size.x, size.y, PixelFormat::RGBA8);
    targets.cbDeferredOpaque1->init();

    targets.cbDeferredOpaque2 = std::make_unique<Texture>("deferred_opaque_color2", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbDeferredOpaque2->clear(size.x, size.y, PixelFormat::RGBA8);
    targets.cbDeferredOpaque2->init();

    targets.fbDeferredCombine = std::make_shared<Framebuffer>();
    targets.fbDeferredCombine->attachColorsDepth({targets.cbDeferredOpaque1, targets.cbDeferredOpaque2}, targets.dbCommon);
    targets.fbDeferredCombine->init();

    // Transparent geometry framebuffer

    targets.cbTransparentGeometry1 = std::make_unique<Texture>("transparent_geometry_color1", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbTransparentGeometry1->clear(size.x, size.y, PixelFormat::RGBA16F);
    targets.cbTransparentGeometry1->init();

    targets.cbTransparentGeometry2 = std::make_unique<Texture>("transparent_geometry_color2", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbTransparentGeometry2->clear(size.x, size.y, PixelFormat::R16F);
    targets.cbTransparentGeometry2->init();

    targets.fbTransparentGeometry = std::make_shared<Framebuffer>();
    targets.fbTransparentGeometry->attachColorsDepth(
        {targets.cbTransparentGeometry1, targets.cbTransparentGeometry2},
        targets.dbCommon);
    targets.fbTransparentGeometry->init();

    // SSAO framebuffer

    targets.cbSSAO = std::make_shared<Texture>("ssao_color", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbSSAO->clear(halfSize.x, halfSize.y, PixelFormat::R8);
    targets.cbSSAO->init();

    targets.fbSSAO = std::make_shared<Framebuffer>();
    targets.fbSSAO->attachColorDepth(targets.cbSSAO, targets.dbCommonHalf);
    targets.fbSSAO->init();

    // SSR framebuffer

    targets.cbSSR = std::make_unique<Texture>("ssr_color", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbSSR->clear(halfSize.x, halfSize.y, PixelFormat::RGBA8);
    targets.cbSSR->init();

    targets.fbSSR = std::make_shared<Framebuffer>();
    targets.fbSSR->attachColorDepth(targets.cbSSR, targets.dbCommonHalf);
    targets.fbSSR->init();

    // Output framebuffer

    targets.cbOutput = std::make_unique<Texture>("output_color", getTextureProperties(TextureUsage::ColorBuffer));
    targets.cbOutput->clear(size.x, size.y, PixelFormat::RGBA8);
    targets.cbOutput->init();

    targets.fbOutput = std::make_unique<Framebuffer>();
    targets.fbOutput->attachColorDepth(targets.cbOutput, targets.dbCommon);
    targets.fbOutput->init();

    // Register attachments

    _sizeToTargets.insert({std::move(size), std::move(targets)});
}

Pipeline::RenderTargets &Pipeline::targetsForSize(const glm::ivec2 &size) {
    if (_sizeToTargets.count(size) == 0) {
        initTargets(size);
    }
    return _sizeToTargets.at(size);
}

void Pipeline::drawSSAO(const glm::ivec2 &dim, RenderTargets &targets, float sampleRadius, float bias) {
    _uniforms.setScreenEffect([&dim, &sampleRadius, &bias](auto &screenEffect) {
        screenEffect.screenResolution = glm::vec2(static_cast<float>(dim.x), static_cast<float>(dim.y));
        screenEffect.ssaoSampleRadius = sampleRadius;
        screenEffect.ssaoBias = bias;
    });

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targets.fbSSAO->nameGL());
    glDrawBuffer(kColorAttachments[0]);
    _context.useProgram(_shaderRegistry.get(ShaderProgramId::deferredSSAO));
    _context.bind(*targets.cbGBufferEyePos, TextureUnits::eyePos);
    _context.bind(*targets.cbGBufferEyeNormal, TextureUnits::eyeNormal);
    _context.bind(_textureRegistry.get(TextureName::noiseRg), TextureUnits::noise);
    _context.clearColorDepth();
    _meshRegistry.get(MeshName::quadNDC).draw();
}

void Pipeline::drawSSR(const glm::ivec2 &dim, RenderTargets &targets, float bias, float pixelStride, float maxSteps) {
    _uniforms.setScreenEffect([&](auto &screenEffect) {
        screenEffect.screenResolution = glm::vec2(dim.x, dim.y);
        screenEffect.screenResolutionRcp = glm::vec2(1.0f / static_cast<float>(dim.x), 1.0f / static_cast<float>(dim.y));
        screenEffect.ssrBias = bias;
        screenEffect.ssrPixelStride = pixelStride;
        screenEffect.ssrMaxSteps = maxSteps;
    });

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targets.fbSSR->nameGL());
    glDrawBuffer(kColorAttachments[0]);
    _context.useProgram(_shaderRegistry.get(ShaderProgramId::deferredSSR));
    _context.bind(*targets.cbGBufferDiffuse);
    _context.bind(*targets.cbGBufferLightmap, TextureUnits::lightmap);
    _context.bind(*targets.cbGBufferEnvMap, TextureUnits::envmapColor);
    _context.bind(*targets.cbGBufferEyePos, TextureUnits::eyePos);
    _context.bind(*targets.cbGBufferEyeNormal, TextureUnits::eyeNormal);
    _context.clearColorDepth();
    _meshRegistry.get(MeshName::quadNDC).draw();
}

void Pipeline::drawOITBlend(RenderTargets &attachments, Framebuffer &dst) {
    _uniforms.setGlobals([](auto &globals) {
        globals.reset();
    });
    _uniforms.setLocals([](auto &locals) {
        locals.reset();
    });

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst.nameGL());
    _context.useProgram(_shaderRegistry.get(ShaderProgramId::oitBlend));
    _context.bind(*attachments.cbDeferredOpaque1);
    _context.bind(*attachments.cbDeferredOpaque2, TextureUnits::hilights);
    _context.bind(*attachments.cbTransparentGeometry1, TextureUnits::oitAccum);
    _context.bind(*attachments.cbTransparentGeometry2, TextureUnits::oitRevealage);
    _context.clearColorDepth();
    _meshRegistry.get(MeshName::quadNDC).draw();
}

void Pipeline::drawBoxBlur(const glm::ivec2 &dim, Texture &srcTexture, Framebuffer &dst) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst.nameGL());
    _context.useProgram(_shaderRegistry.get(ShaderProgramId::postBoxBlur4));
    _context.bind(srcTexture);
    _context.clearColorDepth();
    _meshRegistry.get(MeshName::quadNDC).draw();
}

void Pipeline::drawGaussianBlur(const glm::ivec2 &dim, Texture &srcTexture, Framebuffer &dst, bool vertical, bool strong) {
    _uniforms.setScreenEffect([&dim, &vertical](auto &screenEffect) {
        screenEffect.screenResolutionRcp = glm::vec2(1.0f / static_cast<float>(dim.x), 1.0f / static_cast<float>(dim.y));
        screenEffect.blurDirection = vertical ? glm::vec2(0.0f, 1.0f) : glm::vec2(1.0f, 0.0f);
    });

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst.nameGL());
    _context.useProgram(_shaderRegistry.get(strong ? ShaderProgramId::postGaussianBlur13 : ShaderProgramId::postGaussianBlur9));
    _context.bind(srcTexture);
    _context.clearColorDepth();
    _meshRegistry.get(MeshName::quadNDC).draw();
}

void Pipeline::drawMedianFilter(const glm::ivec2 &dim, Texture &srcTexture, Framebuffer &dst, bool strong) {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst.nameGL());
    _context.useProgram(_shaderRegistry.get(strong ? ShaderProgramId::postMedianFilter5 : ShaderProgramId::postMedianFilter3));
    _context.bind(srcTexture);
    _context.clearColorDepth();
    _meshRegistry.get(MeshName::quadNDC).draw();
}

void Pipeline::drawFXAA(const glm::ivec2 &dim, Texture &srcTexture, Framebuffer &dst) {
    _uniforms.setGlobals([](auto &globals) {
        globals.reset();
    });
    _uniforms.setLocals([](auto &locals) {
        locals.reset();
    });
    _uniforms.setScreenEffect([&dim](auto &screenEffect) {
        screenEffect.screenResolutionRcp = glm::vec2(1.0f / static_cast<float>(dim.x), 1.0f / static_cast<float>(dim.y));
    });

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst.nameGL());
    _context.useProgram(_shaderRegistry.get(ShaderProgramId::postFXAA));
    _context.bind(srcTexture);
    _context.clearColorDepth();
    _meshRegistry.get(MeshName::quadNDC).draw();
}

void Pipeline::drawSharpen(const glm::ivec2 &dim, Texture &srcTexture, Framebuffer &dst, float amount) {
    _uniforms.setScreenEffect([&dim, &amount](auto &screenEffect) {
        screenEffect.screenResolutionRcp = glm::vec2(1.0f / static_cast<float>(dim.x), 1.0f / static_cast<float>(dim.y));
        screenEffect.sharpenAmount = amount;
    });

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst.nameGL());
    _context.useProgram(_shaderRegistry.get(ShaderProgramId::postSharpen));
    _context.bind(srcTexture);
    _context.clearColorDepth();
    _meshRegistry.get(MeshName::quadNDC).draw();
}

void Pipeline::blitFramebuffer(const glm::ivec2 &dim, Framebuffer &src, int srcColorIdx, Framebuffer &dst, int dstColorIdx, int flags) {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, src.nameGL());
    glReadBuffer(kColorAttachments[srcColorIdx]);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dst.nameGL());
    glDrawBuffer(kColorAttachments[dstColorIdx]);

    int flagsGL = 0;
    if (flags & BlitFlags::color) {
        flagsGL |= GL_COLOR_BUFFER_BIT;
    }
    if (flags & BlitFlags::depth) {
        flagsGL |= GL_DEPTH_BUFFER_BIT;
    }
    glBlitFramebuffer(0, 0, dim.x, dim.y, 0, 0, dim.x, dim.y, flagsGL, GL_NEAREST);
}

void Pipeline::setTargetSize(glm::ivec2 size) {
    if (_targetSize == size) {
        return;
    }
    if (_sizeToTargets.count(size) == 0) {
        initTargets(size);
    }
    _targetSize = std::move(size);
}

void Pipeline::beginPass(RenderPass pass) {
    checkEqual(_pass, RenderPass::None, "Must end previous render pass");
    switch (pass) {
    case RenderPass::DirLightShadowsPass:
        beginDirLightShadowsPass();
        break;
    case RenderPass::PointLightShadows:
        beginPointLightShadowsPass();
        break;
    case RenderPass::OpaqueGeometry:
        beginOpaqueGeometryPass();
        break;
    case RenderPass::TransparentGeometry:
        beginTransparentGeometryPass();
        break;
    case RenderPass::PostProcessing:
        beginPostProcessingPass();
        break;
    default:
        throw std::invalid_argument("Unexpected new pass: " + std::to_string(static_cast<int>(pass)));
    }
    _pass = pass;
}

void Pipeline::beginDirLightShadowsPass() {
    auto &targets = targetsForSize(_targetSize);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targets.fbDirLightShadows->nameGL());
    glDrawBuffer(GL_NONE);
    _context.pushViewport(glm::ivec4(0, 0, _options.shadowResolution, _options.shadowResolution));
    _context.clearDepth();
}

void Pipeline::beginPointLightShadowsPass() {
    auto &targets = targetsForSize(_targetSize);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targets.fbPointLightShadows->nameGL());
    glDrawBuffer(GL_NONE);
    _context.pushViewport(glm::ivec4(0, 0, _options.shadowResolution, _options.shadowResolution));
    _context.clearDepth();
}

void Pipeline::beginOpaqueGeometryPass() {
    auto &targets = targetsForSize(_targetSize);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targets.fbOpaqueGeometry->nameGL());
    glDrawBuffers(7, kColorAttachments);
    _context.clearColorDepth();
}

void Pipeline::beginTransparentGeometryPass() {
    auto &targets = targetsForSize(_targetSize);
    blitFramebuffer(_targetSize, *targets.fbOpaqueGeometry, 0, *targets.fbTransparentGeometry, 0, BlitFlags::depth);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targets.fbTransparentGeometry->nameGL());
    glDrawBuffers(2, kColorAttachments);
    glDepthMask(GL_FALSE);
    _context.clearColor({0.0f, 0.0f, 0.0f, 1.0f});
    _context.pushBlending(BlendMode::OIT_Transparent);
}

void Pipeline::beginPostProcessingPass() {
    auto &targets = targetsForSize(_targetSize);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targets.fbOutput->nameGL());
}

void Pipeline::endPass() {
    switch (_pass) {
    case RenderPass::None:
        return;
    case RenderPass::DirLightShadowsPass:
        endDirLightShadowsPass();
        break;
    case RenderPass::PointLightShadows:
        endPointLightShadowsPass();
        break;
    case RenderPass::OpaqueGeometry:
        endOpaqueGeometryPass();
        break;
    case RenderPass::TransparentGeometry:
        endTransparentGeometryPass();
        break;
    case RenderPass::PostProcessing:
        endPostProcessingPass();
        break;
    default:
        throw std::logic_error("Unexpected current pass: " + std::to_string(static_cast<int>(_pass)));
    }
    _pass = RenderPass::None;
}

void Pipeline::endDirLightShadowsPass() {
    _context.popViewport();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Pipeline::endPointLightShadowsPass() {
    _context.popViewport();
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Pipeline::endOpaqueGeometryPass() {
    auto &targets = targetsForSize(_targetSize);

    auto halfSize = _targetSize / 2;
    _context.withViewport(glm::ivec4(0, 0, halfSize.x, halfSize.y), [this, &halfSize, &targets]() {
        if (_options.ssao) {
            drawSSAO(halfSize, targets, 0.5f, 0.1f);
            drawBoxBlur(halfSize, *targets.cbSSAO, *targets.fbPingHalf);
            blitFramebuffer(halfSize, *targets.fbPingHalf, 0, *targets.fbSSAO, 0);
        }
        if (_options.ssr) {
            drawSSR(halfSize, targets, 0.25f, 4.0f, 64.0f);
            drawGaussianBlur(halfSize, *targets.cbSSR, *targets.fbPingHalf, R_GAUSSIAN_BLUR_HORIZONTAL, R_GAUSSIAN_BLUR_STRONG);
            drawGaussianBlur(halfSize, *targets.cbPingHalf, *targets.fbSSR, R_GAUSSIAN_BLUR_VERTICAL, R_GAUSSIAN_BLUR_STRONG);
        }
    });

    _context.bind(*targets.cbGBufferDiffuse);
    _context.bind(*targets.cbGBufferLightmap, TextureUnits::lightmap);
    _context.bind(*targets.cbGBufferEnvMap, TextureUnits::envmapColor);
    _context.bind(*targets.cbGBufferSelfIllum, TextureUnits::selfIllumColor);
    _context.bind(*targets.cbGBufferFeatures, TextureUnits::features);
    _context.bind(*targets.cbGBufferEyePos, TextureUnits::eyePos);
    _context.bind(*targets.cbGBufferEyeNormal, TextureUnits::eyeNormal);
    _context.bind(_options.ssao ? *targets.cbSSAO : _textureRegistry.get(TextureName::ssaoRgb), TextureUnits::ssao);
    _context.bind(_options.ssr ? *targets.cbSSR : _textureRegistry.get(TextureName::ssrRgb), TextureUnits::ssr);
    _context.bind(*targets.dbDirectionalLightShadows, TextureUnits::shadowMapArray);
    _context.bind(*targets.dbPointLightShadows, TextureUnits::shadowMapCube);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, targets.fbDeferredCombine->nameGL());
    glDrawBuffers(2, kColorAttachments);
    _context.clearColorDepth();
    _context.useProgram(_shaderRegistry.get(ShaderProgramId::deferredCombine));
    _meshRegistry.get(MeshName::quadNDC).draw();

    drawGaussianBlur(_targetSize, *targets.cbDeferredOpaque2, *targets.fbPing, R_GAUSSIAN_BLUR_HORIZONTAL, R_GAUSSIAN_BLUR_STRONG);
    drawGaussianBlur(_targetSize, *targets.cbPing, *targets.fbPong, R_GAUSSIAN_BLUR_VERTICAL, R_GAUSSIAN_BLUR_STRONG);
    blitFramebuffer(_targetSize, *targets.fbPong, 0, *targets.fbDeferredCombine, 1);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Pipeline::endTransparentGeometryPass() {
    auto &targets = targetsForSize(_targetSize);
    _context.popBlending();
    glDepthMask(GL_TRUE);
    drawOITBlend(targets, *targets.fbOutput);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void Pipeline::endPostProcessingPass() {
    auto &targets = targetsForSize(_targetSize);
    if (_options.fxaa && _options.sharpen) {
        drawFXAA(_targetSize, *targets.cbOutput, *targets.fbPing);
        drawSharpen(_targetSize, *targets.cbPing, *targets.fbPong, 0.25f);
        blitFramebuffer(_targetSize, *targets.fbPong, 0, *targets.fbOutput, 0);
    } else if (_options.fxaa) {
        drawFXAA(_targetSize, *targets.cbOutput, *targets.fbPing);
        blitFramebuffer(_targetSize, *targets.fbPing, 0, *targets.fbOutput, 0);
    } else if (_options.sharpen) {
        drawSharpen(_targetSize, *targets.cbOutput, *targets.fbPing, 0.25f);
        blitFramebuffer(_targetSize, *targets.fbPing, 0, *targets.fbOutput, 0);
    }
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

Texture &Pipeline::output() {
    return *targetsForSize(_targetSize).cbOutput;
}

} // namespace graphics

} // namespace reone
