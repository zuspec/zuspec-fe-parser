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
#include "ElemFactoryPyObj.h"
#include "ElemFactoryArray.h"
#include "TaskBuildDataType.h"


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
            ERROR("data-type width expression is %d, not int", size_v->getKind());
        }
    } else {
        ERROR("data-type width expression produced null result");
    }

    DEBUG_LEAVE("mkDataType %p", ret);
    return ret;
}

dmgr::IDebug *ElemFactoryArray::m_dbg = 0;


}
}
}
