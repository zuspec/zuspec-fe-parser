/*
 * TaskBuildRootRefExpr.cpp
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
#include "zsp/parser/impl/TaskGetName.h"
#include "TaskBuildExpr.h"
#include "TaskBuildRootRefExpr.h"


namespace zsp {
namespace fe {
namespace parser {


TaskBuildRootRefExpr::TaskBuildRootRefExpr(IAst2ArlContext *ctxt) : m_ctxt(ctxt) {
    DEBUG_INIT("zsp::fe::parser::TaskBuildRootRefExpr", ctxt->getDebugMgr());
}

TaskBuildRootRefExpr::~TaskBuildRootRefExpr() {

}

vsc::dm::ITypeExpr *TaskBuildRootRefExpr::build(
        ast::IExprMemberPathElem    *root,
        ast::IScopeChild            *scope,
        int32_t                     field_idx) {
    DEBUG_ENTER("build");
    m_ret = 0;

    m_field_idx = field_idx;
    scope->accept(this);

    if (!m_ret) {
        m_ret = m_ctxt->ctxt()->mkTypeExprSubField(
            m_ctxt->ctxt()->mkTypeExprRefTopDown(),
            true,
            m_field_idx);
    }

    DEBUG_LEAVE("build");
    return m_ret;
}

void TaskBuildRootRefExpr::visitSymbolTypeScope(ast::ISymbolTypeScope *i) {
    DEBUG_ENTER("visitSymbolTypeScope %s", i->getName().c_str());
    i->getTarget()->accept(this);
    DEBUG_LEAVE("visitSymbolTypeScope %s", i->getName().c_str());
}

void TaskBuildRootRefExpr::visitComponent(ast::IComponent *i) {
    DEBUG_ENTER("visitComponent %s", i->getName()->getId().c_str());
    if (!i->getSuper_t()) {
         m_field_idx--;
    }
    DEBUG_LEAVE("visitComponent %s", i->getName()->getId().c_str());
}

dmgr::IDebug *TaskBuildRootRefExpr::m_dbg = 0;

}
}
}
