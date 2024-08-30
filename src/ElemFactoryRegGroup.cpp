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
#include "zsp/parser/impl/TaskEvalExpr.h"
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
        vsc::dm::ITypeExpr      *init) {
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

vsc::dm::ITypeField *ElemFactoryRegGroup::mkTypeFieldArr(
        IAst2ArlContext         *ctx,
        const std::string       &name,
        ast::IScopeChild        *type,
        ast::IScopeChild        *elem_type,
        ast::IExpr              *size,
        vsc::dm::TypeFieldAttr  attr,
        vsc::dm::ITypeExpr      *init) {
    DEBUG_ENTER("mkTypeFieldArr %s", name.c_str());
    int32_t size_i = 0;

    zsp::parser::IValUP size_v(zsp::parser::TaskEvalExpr(
            ctx->factory(), ctx->getRoot()).eval(size));
    if (size_v->getKind() == zsp::parser::ValKind::Int) {
        size_i = dynamic_cast<zsp::parser::IValInt *>(size_v.get())->getValU();
        DEBUG("size_i=%d", size_i);
    } else {
        DEBUG_ERROR("Array size is not integral");
    }

    vsc::dm::ITypeField *ret = ctx->ctxt()->mkTypeFieldRegGroupArr(
        name,
        TaskBuildDataType(ctx).build(type),
        false,
        TaskBuildDataType(ctx).build(elem_type),
        false,
        size_i);

    DEBUG_LEAVE("mkTypeFieldArr %s %p", name.c_str(), ret);
    return ret;
}

vsc::dm::IDataType *ElemFactoryRegGroup::mkDataType(
        IAst2ArlContext         *ctx,
        const std::string       &name,
        ast::IScopeChild        *type) {
    arl::dm::IDataTypeRegGroup *t = ctx->ctxt()->mkDataTypeRegGroup(name);
    return t;
}

}
}
}
