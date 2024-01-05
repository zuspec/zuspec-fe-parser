/*
 * ElemFactoryAddrHandle.cpp
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
#include "dmgr/impl/DebugMacros.h"
#include "ElemFactoryAddrHandle.h"


namespace zsp {
namespace fe {
namespace parser {


ElemFactoryAddrHandle::ElemFactoryAddrHandle(dmgr::IDebugMgr *dmgr) {
    DEBUG_INIT("zsp::fe::parser::ElemFactoryAddrHandle", dmgr);

}

ElemFactoryAddrHandle::~ElemFactoryAddrHandle() {

}

vsc::dm::IDataType *ElemFactoryAddrHandle::mkDataType(
        IAst2ArlContext         *ctx,
        const std::string       &name,
        ast::IScopeChild        *type) {
    DEBUG_ENTER("mkDataType %s", name.c_str());
    arl::dm::IDataTypeAddrHandle *t = ctx->ctxt()->mkDataTypeAddrHandle(name);
    DEBUG_LEAVE("mkDataType %s", name.c_str());
    return t;
}

dmgr::IDebug *ElemFactoryAddrHandle::m_dbg = 0;

}
}
}
