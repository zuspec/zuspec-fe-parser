/*
 * ElemFactoryTransparentAddrSpace.cpp
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
#include "vsc/dm/impl/ValRef.h"
#include "ElemFactoryTransparentAddrSpace.h"
#include "TaskBuildDataType.h"
#include "TaskGetAddrClaimTrait.h"


namespace zsp {
namespace fe {
namespace parser {


ElemFactoryTransparentAddrSpace::ElemFactoryTransparentAddrSpace(dmgr::IDebugMgr *dmgr) {
    DEBUG_INIT("zsp::fe::parser::ElemFactoryTransparentAddrSpace", dmgr);

}

#ifdef UNDEFINED
vsc::dm::ITypeField *ElemFactoryTransparentAddrSpace::mkTypeFieldPhy(
        IAst2ArlContext         *ctx,
        const std::string       &name,
        ast::IScopeChild        *type,
        vsc::dm::TypeFieldAttr  attr,
        const vsc::dm::ValRef   &init) {
    DEBUG_ENTER("mkTypeFieldPhy %s", name.c_str());

    // Build field in terms of specified target type
    vsc::dm::IDataType *dt = TaskBuildDataType(ctx).build(type);

    vsc::dm::ITypeField *ret = ctx->ctxt()->mkTypeFieldPhy(
            name,
            dt,
            false,
            vsc::dm::TypeFieldAttr::NoAttr,
            vsc::dm::ValRef());

    DEBUG_LEAVE("mkTypeFieldPhy");
    return ret;
}
#endif /* UNDEFINED */

vsc::dm::IDataType *ElemFactoryTransparentAddrSpace::mkDataType(
        IAst2ArlContext         *ctx,
        const std::string       &name,
        ast::IScopeChild        *type) {
    DEBUG_ENTER("mkDataTtype %s", name.c_str());
    ast::IScopeChild *trait_t = TaskGetAddrClaimTrait(
        ctx->getDebugMgr(), ctx->getRoot()).get(type);
    vsc::dm::IDataType *trait_dt = TaskBuildDataType(ctx).build(trait_t);
    arl::dm::IDataTypeAddrSpaceTransparentC *t = 
        ctx->ctxt()->mkDataTypeAddrSpaceTransparentC(
            name, 
            dynamic_cast<vsc::dm::IDataTypeStruct *>(trait_dt));
    DEBUG_LEAVE("mkDataTtype %s", name.c_str());
    return t;
}

ElemFactoryTransparentAddrSpace::~ElemFactoryTransparentAddrSpace() {

}

}
}
}
