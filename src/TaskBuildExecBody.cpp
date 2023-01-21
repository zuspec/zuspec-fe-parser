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
#include "TaskBuildExpr.h"


namespace zsp {
namespace fe {
namespace parser {


TaskBuildExecBody::TaskBuildExecBody(IAst2ArlContext *ctxt) : m_ctxt(ctxt) {
    DEBUG_INIT("TaskBuildExecBody", ctxt->getDebugMgr());
    m_root = 0;
}

TaskBuildExecBody::~TaskBuildExecBody() {

}

void TaskBuildExecBody::build(
        arl::dm::ITypeProcStmtScope     *scope,
        ast::IExecStmt                  *stmt) {
    DEBUG_ENTER("build");
    m_root = stmt;
    m_scope_s.push_back(scope);
    stmt->accept(m_this);
    m_scope_s.pop_back();
    DEBUG_LEAVE("build");
}

static std::map<ast::AssignOp, arl::dm::TypeProcStmtAssignOp> assign_op_m = {
    { ast::AssignOp::AssignOp_Eq, arl::dm::TypeProcStmtAssignOp::Eq }
};

void TaskBuildExecBody::visitProceduralStmtAssignment(ast::IProceduralStmtAssignment *i) { 
    DEBUG_ENTER("visitProceduralStmtAssignment");
    vsc::dm::ITypeExpr *lhs = TaskBuildExpr(m_ctxt).build(i->getLhs());
    vsc::dm::ITypeExpr *rhs = TaskBuildExpr(m_ctxt).build(i->getRhs());
    arl::dm::TypeProcStmtAssignOp op = arl::dm::TypeProcStmtAssignOp::Eq;

    vsc::dm::ITypeExprFieldRef *lhs_lv = dynamic_cast<vsc::dm::ITypeExprFieldRef *>(lhs);

    m_scope_s.back()->addStatement(m_ctxt->ctxt()->mkTypeProcStmtAssign(lhs_lv, op, rhs));

    DEBUG_LEAVE("visitProceduralStmtAssignment");
}
    
void TaskBuildExecBody::visitProceduralStmtExpr(ast::IProceduralStmtExpr *i) { 

}
    
void TaskBuildExecBody::visitProceduralStmtFunctionCall(ast::IProceduralStmtFunctionCall *i) { 

}
    
void TaskBuildExecBody::visitProceduralStmtReturn(ast::IProceduralStmtReturn *i) { 

}
    
void TaskBuildExecBody::visitProceduralStmtRepeat(ast::IProceduralStmtRepeat *i) { 

}
    
void TaskBuildExecBody::visitProceduralStmtRepeatWhile(ast::IProceduralStmtRepeatWhile *i) { 

}
    
void TaskBuildExecBody::visitProceduralStmtWhile(ast::IProceduralStmtWhile *i) { 

}
    
void TaskBuildExecBody::visitProceduralStmtForeach(ast::IProceduralStmtForeach *i) { 

}
    
void TaskBuildExecBody::visitProceduralStmtIfElse(ast::IProceduralStmtIfElse *i) { 
    DEBUG_ENTER("visitProceduralStmtIfElse");
    vsc::dm::ITypeExpr *cond = 0;

    arl::dm::ITypeProcStmtScope *true_s = m_ctxt->ctxt()->mkTypeProcStmtScope();
    m_scope_s.push_back(true_s);
    i->getTrue_s()->accept(m_this);
    m_scope_s.pop_back();

    arl::dm::ITypeProcStmtScope *false_s = 0;
    if (i->getFalse_s()) {
        false_s = m_ctxt->ctxt()->mkTypeProcStmtScope();
        m_scope_s.push_back(false_s);
        i->getFalse_s()->accept(m_this);
        m_scope_s.pop_back();
    }

    arl::dm::ITypeProcStmtIfElse *stmt = m_ctxt->ctxt()->mkTypeProcStmtIfElse(
        cond,
        true_s,
        false_s);

    m_scope_s.back()->addStatement(stmt);

    DEBUG_LEAVE("visitProceduralStmtIfElse");
}
    
void TaskBuildExecBody::visitProceduralStmtMatch(ast::IProceduralStmtMatch *i) { 

}
    
void TaskBuildExecBody::visitProceduralStmtMatchChoice(ast::IProceduralStmtMatchChoice *i) { 

}
    
void TaskBuildExecBody::visitProceduralStmtBreak(ast::IProceduralStmtBreak *i) { 
    DEBUG_ENTER("visitProceduralStmtBreak");
    m_scope_s.back()->addStatement(m_ctxt->ctxt()->mkTypeProcStmtBreak());
    DEBUG_LEAVE("visitProceduralStmtBreak");
}
    
void TaskBuildExecBody::visitProceduralStmtContinue(ast::IProceduralStmtContinue *i) { 
    DEBUG_ENTER("visitProceduralStmtContinue");
    m_scope_s.back()->addStatement(m_ctxt->ctxt()->mkTypeProcStmtContinue());
    DEBUG_LEAVE("visitProceduralStmtContinue");
}
    
void TaskBuildExecBody::visitProceduralStmtDataDeclaration(ast::IProceduralStmtDataDeclaration *i) { 

}

void TaskBuildExecBody::visitProceduralStmtSequenceBlock(ast::IProceduralStmtSequenceBlock *i) {
    // If this *is* the root block, don't create a duplicate scope
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
