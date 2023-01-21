/*
 * TaskBuildExecBody.cpp
 *
 * Copyright 2022 Matthew Ballance and Contributors
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
#include "TaskBuildExecBody.h"


namespace zsp {
namespace fe {
namespace parser {


TaskBuildExecBody::TaskBuildExecBody(dmgr::IDebugMgr *dmgr) {
    DEBUG_INIT("TaskBuildExecBody", dmgr);
    m_root = 0;
}

TaskBuildExecBody::~TaskBuildExecBody() {

}

void TaskBuildExecBody::build(
        IAst2ArlContext                 *ctxt,
        arl::dm::ITypeProcStmtScope     *scope,
        ast::IExecStmt                  *stmt) {
    m_ctxt = ctxt;
    m_root = stmt;
    m_scope_s.push_back(scope);
    stmt->accept(m_this);
    m_scope_s.pop_back();
}

void TaskBuildExecBody::visitProceduralStmtAssignment(ast::IProceduralStmtAssignment *i) { }
    
void TaskBuildExecBody::visitProceduralStmtExpr(ast::IProceduralStmtExpr *i) { }
    
void TaskBuildExecBody::visitProceduralStmtFunctionCall(ast::IProceduralStmtFunctionCall *i) { }
    
void TaskBuildExecBody::visitProceduralStmtReturn(ast::IProceduralStmtReturn *i) { }
    
void TaskBuildExecBody::visitProceduralStmtRepeat(ast::IProceduralStmtRepeat *i) { }
    
void TaskBuildExecBody::visitProceduralStmtRepeatWhile(ast::IProceduralStmtRepeatWhile *i) { }
    
void TaskBuildExecBody::visitProceduralStmtWhile(ast::IProceduralStmtWhile *i) { }
    
void TaskBuildExecBody::visitProceduralStmtForeach(ast::IProceduralStmtForeach *i) { }
    
void TaskBuildExecBody::visitProceduralStmtIfElse(ast::IProceduralStmtIfElse *i) { }
    
void TaskBuildExecBody::visitProceduralStmtMatch(ast::IProceduralStmtMatch *i) { }
    
void TaskBuildExecBody::visitProceduralStmtMatchChoice(ast::IProceduralStmtMatchChoice *i) { }
    
void TaskBuildExecBody::visitProceduralStmtBreak(ast::IProceduralStmtBreak *i) { }
    
void TaskBuildExecBody::visitProceduralStmtContinue(ast::IProceduralStmtContinue *i) { }
    
void TaskBuildExecBody::visitProceduralStmtDataDeclaration(ast::IProceduralStmtDataDeclaration *i) { }

void TaskBuildExecBody::visitProceduralStmtSequenceBlock(ast::IProceduralStmtSequenceBlock *i) {
    if (m_root != i) {
        m_scope_s.push_back(m_ctxt->ctxt()->mkTypeProcStmtScope());
    }

    for (std::vector<ast::IExecStmtUP>::const_iterator
        it=i->getChildren().begin();
        it!=i->getChildren().end(); it++) {
        (*it)->accept(m_this);
    }

    if (m_root != i) {
        m_scope_s.pop_back();
    }
}

dmgr::IDebug *TaskBuildExecBody::m_dbg = 0;

}
}
}
