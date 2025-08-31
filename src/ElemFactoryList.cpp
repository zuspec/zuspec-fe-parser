/*
 * ElemFactoryList.cpp
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
#include "zsp/parser/impl/TaskResolveExprRef.h"
#include "zsp/parser/impl/TaskResolveTypeRef.h"
#include "zsp/parser/impl/TaskGetTemplateParamDeclDefault.h"
#include "ElemFactoryPyObj.h"
#include "ElemFactoryList.h"
#include "TaskBuildDataType.h"
#include "TaskBuildTypeFunctions.h"
#include "TaskGetDataTypeAssocData.h"


namespace zsp {
namespace fe {
namespace parser {


ElemFactoryList::ElemFactoryList(dmgr::IDebugMgr *dmgr) {
    DEBUG_INIT("zsp::fe::parser::ElemFactoryList", dmgr);
}

ElemFactoryList::~ElemFactoryList() {

}

vsc::dm::IDataType *ElemFactoryList::mkDataType(
        IAst2ArlContext         *ctx,
        const std::string       &name,
        ast::IScopeChild        *type) {
    DEBUG_ENTER("mkDataType %s", name.c_str());
    ast::ITypeScope *ts = dynamic_cast<ast::ITypeScope *>(type);
    const std::vector<ast::ITemplateParamDeclUP> &params = ts->getParams()->getParams();
    std::pair<ast::IDataType *, ast::IExpr *> Tp = zsp::parser::TaskGetTemplateParamDeclDefault(
        ctx->getDebugMgr()).default_val(params.at(0).get());

    vsc::dm::IDataType *elem_t = 0;
    if (Tp.first) {
        elem_t = TaskBuildDataType(ctx).buildT<vsc::dm::IDataType>(Tp.first);
    } else {
        elem_t = TaskBuildDataType(ctx).buildT<vsc::dm::IDataType>(Tp.second);
    }

    vsc::dm::IDataTypeList *ret = ctx->ctxt()->findDataTypeList(elem_t);

    // While we're here, define functions
    ast::ISymbolTypeScope *ss = dynamic_cast<ast::ISymbolTypeScope *>(ctx->symScope()); 
    TaskBuildTypeFunctions(ctx, 0).build(ss);

    DEBUG_LEAVE("mkDataType %p", ret);
    return ret;
}

vsc::dm::ITypeField *ElemFactoryList::mkTypeFieldPhy(
        IAst2ArlContext         *ctx,
        const std::string       &name,
        ast::IScopeChild        *type,
        vsc::dm::TypeFieldAttr  attr,
        vsc::dm::ITypeExpr      *init) {
    DEBUG_ENTER("mkTypeFieldPhy %s", name.c_str());
    vsc::dm::ITypeField *ret = 0;

    // Get the element type and check if an elem factory exist for that type
    // If it does, ask whether the type wants a special array-type field
    ast::IScopeChild *res = zsp::parser::TaskResolveTypeRef(
        ctx->getDebugMgr(),
        ctx->getRoot()).resolve(type);
    ast::ISymbolTypeScope *ts = dynamic_cast<ast::ISymbolTypeScope *>(res);

    if (ts) {
        DEBUG("SymbolTypeScope: %s", ts->getName().c_str());
        ast::ITypeScope *ts_t = dynamic_cast<ast::ITypeScope *>(ts->getTarget());

        ast::IScopeChild *elem_t = zsp::parser::TaskResolveTypeRef(
            ctx->getDebugMgr(),
            ctx->getRoot()).resolve(ts_t->getParams()->getParams().at(0).get());

        ast::ISymbolTypeScope *elem_ts = dynamic_cast<ast::ISymbolTypeScope *>(elem_t);

        if (elem_ts) {
            DEBUG("Element type: %s", elem_ts->getName().c_str());
            zsp::ast::IAssocData *assoc_d = TaskGetDataTypeAssocData(ctx).get(elem_ts);
            IElemFactoryAssocData *elem_f;



            if (assoc_d && (elem_f=dynamic_cast<IElemFactoryAssocData *>(assoc_d))) {
                DEBUG("Have associated data");
                ast::IExpr *size = zsp::parser::TaskResolveExprRef(
                    ctx->getDebugMgr(),
                    ctx->getRoot()).resolve(
                        ts_t->getParams()->getParams().at(1).get());
                
                DEBUG("size=%p", size);

                ret = elem_f->mkTypeFieldList(
                    ctx, 
                    name, 
                    type, 
                    elem_ts,
                    attr, 
                    init);
            }
        }
    } else {
        DEBUG_ERROR("Not a type scope");
    }

    DEBUG_LEAVE("mkTypeFieldPhy %p", ret);
    return ret;
}

dmgr::IDebug *ElemFactoryList::m_dbg = 0;


}
}
}
