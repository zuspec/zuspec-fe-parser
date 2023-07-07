/*
 * ElemFactoryRegGroup.cpp
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
#include "ElemFactoryRegGroup.h"
#include "TaskBuildDataType.h"


namespace zsp {
namespace fe {
namespace parser {


ElemFactoryRegGroup::ElemFactoryRegGroup(dmgr::IDebugMgr *dmgr) {
    DEBUG_INIT("zsp::fe::parser::ElemFactoryRegGroup", dmgr);

}

ElemFactoryRegGroup::~ElemFactoryRegGroup() {

}

vsc::dm::ITypeField *ElemFactoryRegGroup::mkTypeFieldPhy(
        IAst2ArlContext         *ctx,
        const std::string       &name,
        ast::IScopeChild        *type,
        vsc::dm::TypeFieldAttr  attr,
        vsc::dm::IModelVal      *init) {
    DEBUG_ENTER("mkTypeFieldPhy %s", name.c_str());

    // Build field in terms of specified target type
    vsc::dm::IDataType *dt = TaskBuildDataType(ctx).build(type);

    vsc::dm::ITypeField *ret = ctx->ctxt()->mkTypeFieldRegGroup(
            name,
            dt,
            false);

    DEBUG_LEAVE("mkTypeFieldPhy");
    return ret;
}

}
}
}
