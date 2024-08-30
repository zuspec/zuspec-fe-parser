/*
 * ElemFactoryReg.cpp
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
#include "ElemFactoryReg.h"
#include "TaskBuildDataTypeReg.h"

namespace zsp {
namespace fe {
namespace parser {


ElemFactoryReg::ElemFactoryReg(dmgr::IDebugMgr *dmgr) {
    DEBUG_INIT("zsp::fe::parser::ElemFactoryReg", dmgr);
}

ElemFactoryReg::~ElemFactoryReg() {

}

vsc::dm::ITypeField *ElemFactoryReg::mkTypeFieldPhy(
        IAst2ArlContext         *ctx,
        const std::string       &name,
        ast::IScopeChild        *type,
        vsc::dm::TypeFieldAttr  attr,
        vsc::dm::ITypeExpr      *init) {
    DEBUG_ENTER("mkTypeFieldPhy %s", name.c_str());
    ast::IDataTypeUserDefined *type_s = dynamic_cast<ast::IDataTypeUserDefined *>(type);
//    DEBUG("type_s name=%s", type_s->getName()->getId().c_str());

    vsc::dm::IDataType *dt = TaskBuildDataTypeReg(ctx).build(type);

    vsc::dm::ITypeField *ret = ctx->ctxt()->mkTypeFieldReg(
        name,
        dt,
        false);

    DEBUG_LEAVE("mkTypeFieldPhy %s", name.c_str());
    return ret;
}

}
}
}
