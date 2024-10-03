/*
 * TaskLinkBuiltinTypeElemFactories.cpp
 *
 * Copyright 2023 Matthew Ballance and Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may 
 * not use this file except in compliance with the License.  
 * You may obtain a copy of the License at:
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
 * See the License for the specific language governing permissions and 
 * limitations under the License.
 *
 * Created on:
 *     Author:
 */
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include "dmgr/impl/DebugMacros.h"
#include "zsp/ast/ISymbolScope.h"
#include "zsp/ast/ISymbolTypeScope.h"
#include "zsp/fe/parser/IElemFactoryAssocData.h"
#include "ElemFactoryAddrClaim.h"
#include "ElemFactoryTransparentAddrClaim.h"
#include "ElemFactoryAddrHandle.h"
#include "ElemFactoryArray.h"
#include "ElemFactoryList.h"
#include "ElemFactoryPackedStruct.h"
#include "ElemFactoryPyObj.h"
#include "ElemFactoryReg.h"
#include "ElemFactoryRegGroup.h"
#include "ElemFactoryTransparentAddrSpace.h"
#include "TaskLinkBuiltinTypeElemFactories.h"


namespace zsp {
namespace fe {
namespace parser {

static struct LinkTabEntry {
    std::vector<std::string>                                    path;
    std::function<IElemFactoryAssocData *(dmgr::IDebugMgr *)>   ctor;
//    IElemFactoryAssocData *(*ctor)();
} LinkTab[] = {
    { {"array"}, &ElemFactoryArray::create },
    { {"list"}, &ElemFactoryArray::create },
    { {"pyobj"}, &ElemFactoryPyObj::create },
    { {"addr_reg_pkg", "addr_claim_s"}, &ElemFactoryAddrClaim::create },
    { {"addr_reg_pkg", "transparent_addr_claim_s"}, &ElemFactoryTransparentAddrClaim::create },
    { {"addr_reg_pkg", "reg_c"}, &ElemFactoryReg::create },
    { {"addr_reg_pkg", "reg_group_c"}, &ElemFactoryRegGroup::create },
    { {"addr_reg_pkg", "transparent_addr_space_c"}, &ElemFactoryTransparentAddrSpace::create },
    { {"addr_reg_pkg", "addr_handle_t"}, &ElemFactoryAddrHandle::create },
    { {"std_pkg", "packed_s"}, &ElemFactoryPackedStruct::create }

};


TaskLinkBuiltinTypeElemFactories::TaskLinkBuiltinTypeElemFactories(
    IAst2ArlContext     *ctxt) : m_ctxt(ctxt) {
    DEBUG_INIT("zsp::fe::parser::TaskLinkBuiltinTypeElemFactories", 
        ctxt->getDebugMgr());

}

TaskLinkBuiltinTypeElemFactories::~TaskLinkBuiltinTypeElemFactories() {

}

void TaskLinkBuiltinTypeElemFactories::link(ast::ISymbolScope *root) {
    DEBUG_ENTER("link");

    for (uint32_t i=0; i<sizeof(LinkTab)/sizeof(LinkTabEntry); i++) {
        ast::IScopeChild *elem = findElem(root, LinkTab[i].path, 0);
        IElemFactoryAssocData *ad = LinkTab[i].ctor(m_ctxt->getDebugMgr());
        if (elem) {
            ast::ISymbolTypeScope *ts = dynamic_cast<ast::ISymbolTypeScope *>(elem);
            if (ts) {
                DEBUG("Is a Symbol TypeScope");
                DEBUG("Set associated data for type %s", ts->getName().c_str());
                ts->setAssocData(ad);

                for (std::vector<ast::ISymbolTypeScopeUP>::const_iterator
                    it=ts->getSpec_types().begin();
                    it!=ts->getSpec_types().end(); it++) {
                    DEBUG("Set associated data for specialization %s", (*it)->getName().c_str());
                    (*it)->setAssocData(ad, false);
                }
            }

        }
    }
    DEBUG_LEAVE("link");
}

ast::IScopeChild *TaskLinkBuiltinTypeElemFactories::findElem(
        ast::ISymbolScope               *root, 
        const std::vector<std::string>  &path,
        int32_t                         idx) {
    DEBUG_ENTER("findElem: %s", path.at(idx).c_str());
    std::unordered_map<std::string, int32_t>::const_iterator it = 
        root->getSymtab().find(path.at(idx));
    ast::IScopeChild *child = 0;

    if (it != root->getSymtab().end()) {
        DEBUG("Found");
        child = root->getChildren().at(it->second).get();
    } else {
        DEBUG("Failed to find");
    }

    if (idx+1 < path.size() && child) {
        child = findElem(
            dynamic_cast<ast::ISymbolScope *>(child),
            path,
            idx+1);
    }

    DEBUG_LEAVE("findElem: %s %p", path.at(idx).c_str(), child);
    return child;
}

dmgr::IDebug *TaskLinkBuiltinTypeElemFactories::m_dbg = 0;

}
}
}
