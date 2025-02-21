/*
 * ElemFactoryAddrClaim.cpp
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
#include "ElemFactoryAddrClaim.h"
#include "TaskBuildDataType.h"
#include "TaskGetAddrClaimTrait.h"


namespace zsp {
namespace fe {
namespace parser {


ElemFactoryAddrClaim::ElemFactoryAddrClaim(dmgr::IDebugMgr *dmgr) {
    DEBUG_INIT("zsp::fe::parser::ElemFactoryAddrClaim", dmgr);
}

ElemFactoryAddrClaim::~ElemFactoryAddrClaim() {

}

vsc::dm::IDataType *ElemFactoryAddrClaim::mkDataType(
        IAst2ArlContext         *ctx,
        const std::string       &name,
        ast::IScopeChild        *type) {
    DEBUG_ENTER("mkDataType");
    vsc::dm::IDataType *ret = ctx->ctxt()->mkDataTypeAddrClaim(name);
    DEBUG_LEAVE("mkDataType");
    return ret;
}

vsc::dm::ITypeField *ElemFactoryAddrClaim::mkTypeFieldPhy(
        IAst2ArlContext         *ctx,
        const std::string       &name,
        ast::IScopeChild        *type,
        vsc::dm::TypeFieldAttr  attr,
        vsc::dm::ITypeExpr      *init) {
    DEBUG_ENTER("mkTypeFieldPhy %s", name.c_str());
    vsc::dm::IDataType *type_dt = TaskBuildDataType(ctx).buildT<vsc::dm::IDataType>(type);
    ast::IScopeChild *trait_t = TaskGetAddrClaimTrait(
        ctx->getDebugMgr(), ctx->getRoot()).get(type);
    vsc::dm::IDataType *trait_dt = TaskBuildDataType(ctx).buildT<vsc::dm::IDataType>(trait_t);

    vsc::dm::ITypeField *ret = ctx->ctxt()->mkTypeFieldAddrClaim(
        name,
        type_dt,
        false,
        dynamic_cast<vsc::dm::IDataTypeStruct *>(trait_dt));
    
    DEBUG_LEAVE("mkTypeFieldPhy %s", name.c_str());
    return ret;
}

}
}
}
