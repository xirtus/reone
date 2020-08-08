/*
 * Copyright � 2020 Vsevolod Kremianskii
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

#include "templates.h"

#include <map>

#include "../core/log.h"
#include "../resources/manager.h"

using namespace std;

using namespace reone::resources;

namespace reone {

namespace game {

static map<string, shared_ptr<Item>> g_itemCache = map<string, shared_ptr<Item>>();

TemplateManager &TemplateManager::instance() {
    static TemplateManager instance;
    return instance;
}

shared_ptr<Item> TemplateManager::findItem(const string &resRef) {
    auto it = g_itemCache.find(resRef);
    if (it != g_itemCache.end()) {
        return it->second;
    }
    debug("Loading item blueprint " + resRef);

    ResourceManager &resources = ResourceManager::instance();
    shared_ptr<GffStruct> uti(resources.findGFF(resRef, ResourceType::ItemBlueprint));
    shared_ptr<Item> item;

    if (uti) {
        item.reset(new Item());
        item->load(resRef, *uti);
    } else {
        warn("Item blueprint not found: " + resRef);
    }

    auto pair = g_itemCache.insert(make_pair(resRef, item));

    return pair.first->second;
}

} // namespace game

} // namespace reone
