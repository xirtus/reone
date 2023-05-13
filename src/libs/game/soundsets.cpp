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

#include "reone/game/soundsets.h"

#include "reone/audio/files.h"
#include "reone/system/stream/bytearrayinput.h"
#include "reone/resource/resources.h"
#include "reone/resource/strings.h"

#include "reone/game/format/ssfreader.h"

using namespace std;

using namespace reone::audio;
using namespace reone::resource;

namespace reone {

namespace game {

shared_ptr<SoundSet> SoundSets::doGet(string resRef) {
    auto data = _resources.get(resRef, ResourceType::Ssf);
    if (!data) {
        return nullptr;
    }
    auto stream = ByteArrayInputStream(*data);
    auto result = make_shared<SoundSet>();

    SsfReader ssf;
    ssf.load(stream);

    vector<uint32_t> sounds(ssf.soundSet());
    for (size_t i = 0; i < sounds.size(); ++i) {
        string soundResRef(boost::to_lower_copy(_strings.getSound(sounds[i])));
        shared_ptr<AudioStream> sound(_audioFiles.get(soundResRef));
        if (sound) {
            result->insert(make_pair(static_cast<SoundSetEntry>(i), sound));
        }
    }

    return move(result);
}

} // namespace game

} // namespace reone