/*
 * ElemFactoryArray.cpp
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
#include "ElemFactoryPyObj.h"
#include "ElemFactoryArray.h"
#include "TaskBuildDataType.h"
#include "TaskGetDataTypeAssocData.h"


namespace zsp {
namespace fe {
namespace parser {


ElemFactoryArray::ElemFactoryArray(dmgr::IDebugMgr *dmgr) {
    DEBUG_INIT("zsp::fe::parser::ElemFactoryArray", dmgr);
}

ElemFactoryArray::~ElemFactoryArray() {

}

vsc::dm::IDataType *ElemFactoryArray::mkDataType(
        IAst2ArlContext         *ctx,
        const std::string       &name,
        ast::IScopeChild        *type) {
    DEBUG_ENTER("mkDataType");
    ast::ITypeScope *ts = dynamic_cast<ast::ITypeScope *>(type);
    const std::vector<ast::ITemplateParamDeclUP> &params = ts->getParams()->getParams();
    ast::ITemplateGenericTypeParamDecl *Tp = 
        dynamic_cast<ast::ITemplateGenericTypeParamDecl *>(params.at(0).get());
    ast::ITemplateValueParamDecl *SZp = 
        dynamic_cast<ast::ITemplateValueParamDecl *>(params.at(1).get());

    // TODO: 
    vsc::dm::IDataType *elem_t = TaskBuildDataType(ctx).build(Tp->getDflt());

    
    zsp::parser::IVal *size_v = zsp::parser::TaskEvalExpr(
        ctx->factory(),
        ctx->getRoot()).eval(SZp->getDflt());

    vsc::dm::IDataTypeArray *ret = 0;
    if (size_v) {
        if (size_v->getKind() == zsp::parser::ValKind::Int) {
            ret = ctx->ctxt()->findDataTypeArray(
                elem_t,
                dynamic_cast<zsp::parser::IValInt *>(size_v)->getValU(),
                true);
         } else {
            DEBUG_ERROR("data-type width expression is %d, not int", size_v->getKind());
        }
    } else {
        DEBUG_ERROR("data-type width expression produced null result");
    }

    DEBUG_LEAVE("mkDataType %p", ret);
    return ret;
}

vsc::dm::ITypeField *ElemFactoryArray::mkTypeFieldPhy(
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

                ret = elem_f->mkTypeFieldArr(
                    ctx, 
                    name, 
                    type, 
                    elem_ts,
                    size,
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

dmgr::IDebug *ElemFactoryArray::m_dbg = 0;


}
}
}
