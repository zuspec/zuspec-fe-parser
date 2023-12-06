/*
 * TaskBuildTypeExecs.cpp
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
#include "zsp/arl/dm/IContext.h"
#include "TaskBuildTypeExecs.h"
#include "TaskBuildTypeExecStmt.h"


namespace zsp {
namespace fe {
namespace parser {


TaskBuildTypeExecs::TaskBuildTypeExecs(IAst2ArlContext *ctxt) : m_ctxt(ctxt) {
    DEBUG_INIT("TaskBuildTypeExecs", m_ctxt->getDebugMgr());
}

TaskBuildTypeExecs::~TaskBuildTypeExecs() {

}

void TaskBuildTypeExecs::build(
        arl::dm::IDataTypeArlStruct *target,
        ast::ISymbolTypeScope       *src) {
    DEBUG_ENTER("build");
    m_target = target;
    m_depth = 0;
    src->accept(m_this);
    DEBUG_LEAVE("build");
}

static std::vector<ast::ExecKind> prv_kinds = {
    ast::ExecKind::ExecKind_Body,
    ast::ExecKind::ExecKind_PreSolve,
    ast::ExecKind::ExecKind_PostSolve
};

void TaskBuildTypeExecs::visitSymbolTypeScope(ast::ISymbolTypeScope *i) {
    DEBUG_ENTER("visitSymbolTypeScope");

    if (!m_depth) {
        m_depth++;
        for (std::vector<ast::ExecKind>::const_iterator
            k_it=prv_kinds.begin();
            k_it!=prv_kinds.end(); k_it++) {
            m_target_kind = *k_it;

            for (std::vector<ast::IScopeChild *>::const_iterator
                it=i->getChildren().begin();
                it!=i->getChildren().end(); it++) {
                (*it)->accept(m_this);
            }
        }
        m_depth--;
    } else {
        DEBUG("Skip recursion");
    }

    DEBUG_LEAVE("visitSymbolTypeScope");
}

void TaskBuildTypeExecs::visitSymbolExecScope(ast::ISymbolExecScope *i) {
    DEBUG_ENTER("visitSymbolExecScope target_kind=%d size=%d",
        m_target_kind,
        i->getChildren().size());

    /*
    if (dynamic_cast<ast::IExecBlock *>(i->getTarget())->getKind() != m_target_kind) {
        DEBUG_LEAVE("visitSymbolExecScope -- not target kind");
        return;
    }
     */

    arl::dm::ExecKindT kind;
    switch (m_target_kind) {
        case ast::ExecKind::ExecKind_Body: 
            kind = arl::dm::ExecKindT::Body; 
            break;
        case ast::ExecKind::ExecKind_PreSolve: 
            kind = arl::dm::ExecKindT::PreSolve; 
            break;
        case ast::ExecKind::ExecKind_PostSolve: 
            kind = arl::dm::ExecKindT::PostSolve; 
            break;
    }

    arl::dm::ITypeProcStmtScope *exec_s = m_ctxt->ctxt()->mkTypeProcStmtScope();

    for (std::vector<ast::IScopeChild *>::const_iterator
        it=i->getChildren().begin();
        it!=i->getChildren().end(); it++) {
        TaskBuildTypeExecStmt(m_ctxt).build(exec_s, *it);
    }

    m_target->addExec(m_ctxt->ctxt()->mkTypeExecProc(kind, exec_s));

    DEBUG_LEAVE("visitSymbolExecScope");
}

dmgr::IDebug *TaskBuildTypeExecs::m_dbg = 0;

}
}
}
