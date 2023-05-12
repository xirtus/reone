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

#include "reone/game/resourcelayout.h"

#include "reone/system/exception/validation.h"
#include "reone/system/pathutil.h"
#include "reone/graphics/types.h"
#include "reone/resource/2das.h"
#include "reone/resource/gffs.h"
#include "reone/resource/resources.h"
#include "reone/resource/services.h"

#include "reone/game/options.h"
#include "reone/game/types.h"

using namespace std;

using namespace reone::graphics;

namespace reone {

namespace game {

static constexpr char kKeyFilename[] = "chitin.key";
static constexpr char kPatchFilename[] = "patch.erf";
static constexpr char kTexturePackDirectoryName[] = "texturepacks";
static constexpr char kMusicDirectoryName[] = "streammusic";
static constexpr char kSoundsDirectoryName[] = "streamsounds";
static constexpr char kWavesDirectoryName[] = "streamwaves";
static constexpr char kVoiceDirectoryName[] = "streamvoice";
static constexpr char kModulesDirectoryName[] = "modules";
static constexpr char kLipsDirectoryName[] = "lips";
static constexpr char kLocalizationLipFilename[] = "localization";
static constexpr char kOverrideDirectoryName[] = "override";

static constexpr char kTexturePackFilenameGUI[] = "swpc_tex_gui.erf";
static constexpr char kTexturePackFilenameHigh[] = "swpc_tex_tpa.erf";
static constexpr char kTexturePackFilenameMedium[] = "swpc_tex_tpb.erf";
static constexpr char kTexturePackFilenameLow[] = "swpc_tex_tpc.erf";

static constexpr char kExeFilenameKotor[] = "swkotor.exe";
static constexpr char kExeFilenameTsl[] = "swkotor2.exe";

static const vector<string> g_nonTransientLipFiles {"global.mod", "localization.mod"};

static const unordered_map<TextureQuality, string> texPackByQuality {
    {TextureQuality::High, kTexturePackFilenameHigh},
    {TextureQuality::Medium, kTexturePackFilenameMedium},
    {TextureQuality::Low, kTexturePackFilenameLow}};

void ResourceLayout::init() {
    if (_gameId == GameID::KotOR) {
        initForKotOR();
    } else if (_gameId == GameID::TSL) {
        initForTSL();
    } else {
        throw logic_error("Unsupported game ID: " + to_string(static_cast<int>(_gameId)));
    }
}

void ResourceLayout::initForKotOR() {
    _resourceSvc.defaultResources().indexKeyFile(getPathIgnoreCase(_options.game.path, kKeyFilename));
    _resourceSvc.defaultResources().indexErfFile(getPathIgnoreCase(_options.game.path, kPatchFilename));

    boost::filesystem::path texPacksPath(getPathIgnoreCase(_options.game.path, kTexturePackDirectoryName));
    _resourceSvc.defaultResources().indexErfFile(getPathIgnoreCase(texPacksPath, kTexturePackFilenameGUI));

    auto texPack = texPackByQuality.find(_options.graphics.textureQuality)->second;
    _resourceSvc.defaultResources().indexErfFile(getPathIgnoreCase(texPacksPath, texPack));

    _resourceSvc.defaultResources().indexDirectory(getPathIgnoreCase(_options.game.path, kMusicDirectoryName));
    _resourceSvc.defaultResources().indexDirectory(getPathIgnoreCase(_options.game.path, kSoundsDirectoryName));
    _resourceSvc.defaultResources().indexDirectory(getPathIgnoreCase(_options.game.path, kWavesDirectoryName));

    boost::filesystem::path lipsPath(getPathIgnoreCase(_options.game.path, kLipsDirectoryName));
    for (auto &filename : g_nonTransientLipFiles) {
        _resourceSvc.defaultResources().indexErfFile(getPathIgnoreCase(lipsPath, filename));
    }

    _resourceSvc.defaultResources().indexDirectory(getPathIgnoreCase(_options.game.path, kOverrideDirectoryName));
    _resourceSvc.defaultResources().indexExeFile(getPathIgnoreCase(_options.game.path, kExeFilenameKotor));
}

void ResourceLayout::initForTSL() {
    _resourceSvc.defaultResources().indexKeyFile(getPathIgnoreCase(_options.game.path, kKeyFilename));

    boost::filesystem::path texPacksPath(getPathIgnoreCase(_options.game.path, kTexturePackDirectoryName));
    _resourceSvc.defaultResources().indexErfFile(getPathIgnoreCase(texPacksPath, kTexturePackFilenameGUI));

    auto texPack = texPackByQuality.find(_options.graphics.textureQuality)->second;
    _resourceSvc.defaultResources().indexErfFile(getPathIgnoreCase(texPacksPath, texPack));

    _resourceSvc.defaultResources().indexDirectory(getPathIgnoreCase(_options.game.path, kMusicDirectoryName));
    _resourceSvc.defaultResources().indexDirectory(getPathIgnoreCase(_options.game.path, kSoundsDirectoryName));
    _resourceSvc.defaultResources().indexDirectory(getPathIgnoreCase(_options.game.path, kVoiceDirectoryName));

    boost::filesystem::path lipsPath(getPathIgnoreCase(_options.game.path, kLipsDirectoryName));
    _resourceSvc.defaultResources().indexErfFile(getPathIgnoreCase(lipsPath, kLocalizationLipFilename));

    _resourceSvc.defaultResources().indexDirectory(getPathIgnoreCase(_options.game.path, kOverrideDirectoryName));
    _resourceSvc.defaultResources().indexExeFile(getPathIgnoreCase(_options.game.path, kExeFilenameTsl));
}

void ResourceLayout::loadModuleResources(const string &moduleName) {
    _resourceSvc.defaultTwoDas().invalidate();
    _resourceSvc.defaultGffs().invalidate();
    _resourceSvc.defaultResources().clearTransientProviders();

    boost::filesystem::path modulesPath(getPathIgnoreCase(_options.game.path, kModulesDirectoryName));
    if (modulesPath.empty()) {
        throw ValidationException("Modules directory not found");
    }

    boost::filesystem::path modPath(getPathIgnoreCase(modulesPath, moduleName + ".mod", false));
    if (!modPath.empty()) {
        _resourceSvc.defaultResources().indexErfFile(getPathIgnoreCase(modulesPath, moduleName + ".mod", false), true);
    } else {
        _resourceSvc.defaultResources().indexRimFile(getPathIgnoreCase(modulesPath, moduleName + ".rim"), true);
        _resourceSvc.defaultResources().indexRimFile(getPathIgnoreCase(modulesPath, moduleName + "_s.rim"), true);
    }

    boost::filesystem::path lipsPath(getPathIgnoreCase(_options.game.path, kLipsDirectoryName));
    if (!lipsPath.empty()) {
        _resourceSvc.defaultResources().indexErfFile(getPathIgnoreCase(lipsPath, moduleName + "_loc.mod"), true);
    }

    if (_gameId == GameID::TSL) {
        _resourceSvc.defaultResources().indexErfFile(getPathIgnoreCase(modulesPath, moduleName + "_dlg.erf"), true);
    }
}

} // namespace game

} // namespace reone
