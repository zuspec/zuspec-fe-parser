/*
 * TaskBuildTypeExecStmt.cpp
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
#include "TaskBuildTypeExecStmt.h"
#include "TaskBuildDataType.h"
#include "TaskBuildExpr.h"


namespace zsp {
namespace fe {
namespace parser {


TaskBuildTypeExecStmt::TaskBuildTypeExecStmt(IAst2ArlContext *ctxt) : m_ctxt(ctxt) {
    DEBUG_INIT("zsp::fe::parser::TaskBuildTypeExecStmt", ctxt->getDebugMgr());
    m_root = 0;
}

TaskBuildTypeExecStmt::~TaskBuildTypeExecStmt() {

}

arl::dm::ITypeProcStmt *TaskBuildTypeExecStmt::build(ast::IScopeChild *stmt) {
    DEBUG_ENTER("build");
    m_stmt = 0;
    m_wrapper_s = 0;
    stmt->accept(m_this);
    DEBUG_LEAVE("build");
    return m_stmt;
}

static std::map<ast::AssignOp, arl::dm::TypeProcStmtAssignOp> assign_op_m = {
    { ast::AssignOp::AssignOp_Eq, arl::dm::TypeProcStmtAssignOp::Eq }
};

void TaskBuildTypeExecStmt::visitProceduralStmtAssignment(ast::IProceduralStmtAssignment *i) { 
    DEBUG_ENTER("visitProceduralStmtAssignment");
    vsc::dm::ITypeExpr *lhs = TaskBuildExpr(m_ctxt).build(i->getLhs());
    vsc::dm::ITypeExpr *rhs = TaskBuildExpr(m_ctxt).build(i->getRhs());
    arl::dm::TypeProcStmtAssignOp op = arl::dm::TypeProcStmtAssignOp::Eq;

    switch (i->getOp()) {
        case ast::AssignOp::AssignOp_AndEq: op = arl::dm::TypeProcStmtAssignOp::AndEq; break;
        case ast::AssignOp::AssignOp_Eq: op = arl::dm::TypeProcStmtAssignOp::Eq; break;
        case ast::AssignOp::AssignOp_MinusEq: op = arl::dm::TypeProcStmtAssignOp::MinusEq; break;
        case ast::AssignOp::AssignOp_OrEq: op = arl::dm::TypeProcStmtAssignOp::OrEq; break;
        case ast::AssignOp::AssignOp_PlusEq: op = arl::dm::TypeProcStmtAssignOp::PlusEq; break;
        case ast::AssignOp::AssignOp_ShlEq: op = arl::dm::TypeProcStmtAssignOp::ShlEq; break;
        case ast::AssignOp::AssignOp_ShrEq: op = arl::dm::TypeProcStmtAssignOp::ShrEq; break;
    }

    m_stmt = m_ctxt->ctxt()->mkTypeProcStmtAssign(lhs, op, rhs);

    DEBUG_LEAVE("visitProceduralStmtAssignment");
}
    
void TaskBuildTypeExecStmt::visitProceduralStmtExpr(ast::IProceduralStmtExpr *i) { 
    DEBUG_ENTER("visitProceduralStmtExpr");
    vsc::dm::ITypeExpr *expr = TaskBuildExpr(m_ctxt).build(i->getExpr());

    m_stmt = m_ctxt->ctxt()->mkTypeProcStmtExpr(expr);

    DEBUG_LEAVE("visitProceduralStmtExpr");
}
    
void TaskBuildTypeExecStmt::visitProceduralStmtFunctionCall(ast::IProceduralStmtFunctionCall *i) { 
    DEBUG_ENTER("visitProceduralStmtFunctionCall");
    DEBUG("TODO: visitProceduralStmtFunctionCall");
    DEBUG_LEAVE("visitProceduralStmtFunctionCall");
}
    
void TaskBuildTypeExecStmt::visitProceduralStmtReturn(ast::IProceduralStmtReturn *i) { 
    DEBUG_ENTER("visitProceduralStmtReturn");
    vsc::dm::ITypeExpr *expr = 0;
    if (i->getExpr()) {
        expr = TaskBuildExpr(m_ctxt).build(i->getExpr());
    }

    m_stmt = m_ctxt->ctxt()->mkTypeProcStmtReturn(expr);

    DEBUG_LEAVE("visitProceduralStmtReturn %p", m_stmt);
}
    
void TaskBuildTypeExecStmt::visitProceduralStmtRepeat(ast::IProceduralStmtRepeat *i) { 
    DEBUG_ENTER("visitProceduralStmtRepeat (%d)", i->getChildren().size());
    m_ctxt->pushSymScope(i);

    arl::dm::ITypeProcStmtRepeat *stmt = m_ctxt->ctxt()->mkTypeProcStmtRepeat(
        TaskBuildExpr(m_ctxt).build(i->getCount()),
        TaskBuildTypeExecStmt(m_ctxt).build(i->getBody())
    );

    // Build out any iteration variables
    for (std::vector<ast::IScopeChildUP>::const_iterator
        it=i->getChildren().begin();
        it!=i->getChildren().end(); it++) {
        m_stmt = 0;
        (*it)->accept(m_this);
        if (m_stmt) {
            stmt->addVariable(dynamic_cast<arl::dm::ITypeProcStmtVarDecl *>(m_stmt));
        }
    }

    // TODO: think 'cond' and 'body' have different scopes.
    // 

    // Three-step:
    // - Create repeat
    // - Create variables
    // - Elaborate body
    // arl::dm::ITypeProcStmtRepeat *stmt = m_ctxt->ctxt()->mkTypeProcStmtRepeat(
    //     TaskBuildExpr(m_ctxt).build(i->getCount()),
    //     TaskBuildTypeExecStmt(m_ctxt).build(i->getBody())
    // );
    m_ctxt->popSymScope();
    m_stmt = stmt;
    DEBUG_LEAVE("visitProceduralStmtRepeat");
}
    
void TaskBuildTypeExecStmt::visitProceduralStmtRepeatWhile(ast::IProceduralStmtRepeatWhile *i) { 
    DEBUG_ENTER("visitProceduralStmtRepeatWhile");
    arl::dm::ITypeProcStmtRepeatWhile *stmt = m_ctxt->ctxt()->mkTypeProcStmtRepeatWhile(
        TaskBuildExpr(m_ctxt).build(i->getExpr()),
        TaskBuildTypeExecStmt(m_ctxt).build(i->getBody())
    );

    m_stmt = stmt;
    DEBUG_LEAVE("visitProceduralStmtRepeatWhile");
}
    
void TaskBuildTypeExecStmt::visitProceduralStmtWhile(ast::IProceduralStmtWhile *i) { 
    DEBUG_ENTER("visitProceduralStmtWhile");
    arl::dm::ITypeProcStmtWhile *while_s = m_ctxt->ctxt()->mkTypeProcStmtWhile(
        TaskBuildExpr(m_ctxt).build(i->getExpr()),
        TaskBuildTypeExecStmt(m_ctxt).build(i->getBody())
    );
    m_stmt = while_s;
    DEBUG_LEAVE("visitProceduralStmtWhile");
}
    
void TaskBuildTypeExecStmt::visitProceduralStmtForeach(ast::IProceduralStmtForeach *i) { 

}
    
void TaskBuildTypeExecStmt::visitProceduralStmtIfElse(ast::IProceduralStmtIfElse *i) { 
    DEBUG_ENTER("visitProceduralStmtIfElse");
    // ast::ISymbolChildrenScope *sym_c = m_wrapper_s;
    arl::dm::ITypeProcStmt *else_s = 0;


    if (i->getElse_then()) {
        else_s = TaskBuildTypeExecStmt(m_ctxt).build(i->getElse_then());
    }

    // if (sym_c->getChildren().size() > i->getIf_then().size()) {
    //     // Have an 'else'
    //     else_s = TaskBuildTypeExecStmt(m_ctxt).build(sym_c->getChildren().back().get());
    // }

    arl::dm::ITypeProcStmtIfElse *if_else = m_ctxt->ctxt()->mkTypeProcStmtIfElse({}, else_s);

    for (std::vector<ast::IProceduralStmtIfClauseUP>::const_iterator
        it=i->getIf_then().begin();
        it!=i->getIf_then().end(); it++) {
        if_else->addIfClause(
            m_ctxt->ctxt()->mkTypeProcStmtIfClause(
                TaskBuildExpr(m_ctxt).build((*it)->getCond()),
                TaskBuildTypeExecStmt(m_ctxt).build((*it)->getBody())));
    }

    // for (uint32_t ii=0; ii<i->getIf_then().size(); ii++) {
    //     arl::dm::ITypeProcStmt *body = TaskBuildTypeExecStmt(m_ctxt).build(
    //         i->getIf_then().at(ii)->getBody());
    //     if (!body) {
    //         body = m_ctxt->ctxt()->mkTypeProcStmtScope();
    //     }
    //     if_else->addIfClause(m_ctxt->ctxt()->mkTypeProcStmtIfClause(
    //         TaskBuildExpr(m_ctxt).build(i->getIf_then().at(ii)->getCond()),
    //         body
    //     ));

    m_stmt = if_else;

    DEBUG_LEAVE("visitProceduralStmtIfElse");
}
    
void TaskBuildTypeExecStmt::visitProceduralStmtMatch(ast::IProceduralStmtMatch *i) { 

}
    
void TaskBuildTypeExecStmt::visitProceduralStmtMatchChoice(ast::IProceduralStmtMatchChoice *i) { 

}
    
void TaskBuildTypeExecStmt::visitProceduralStmtBreak(ast::IProceduralStmtBreak *i) { 
    DEBUG_ENTER("visitProceduralStmtBreak");
    m_stmt = m_ctxt->ctxt()->mkTypeProcStmtBreak();
    DEBUG_LEAVE("visitProceduralStmtBreak");
}
    
void TaskBuildTypeExecStmt::visitProceduralStmtContinue(ast::IProceduralStmtContinue *i) { 
    DEBUG_ENTER("visitProceduralStmtContinue");
    m_stmt = m_ctxt->ctxt()->mkTypeProcStmtContinue();
    DEBUG_LEAVE("visitProceduralStmtContinue");
}
    
void TaskBuildTypeExecStmt::visitProceduralStmtDataDeclaration(ast::IProceduralStmtDataDeclaration *i) { 
    DEBUG_ENTER("visitProceduralStmtDataDeclaration");
    ast::IDataType *ast_dt = i->getDatatype();
    vsc::dm::IDataType *dm_dt;

    if (ast_dt) {
        dm_dt = TaskBuildDataType(m_ctxt).build(ast_dt);
    } else {
        // TODO:
        dm_dt = m_ctxt->ctxt()->findDataTypeInt(true, 32);
    }

    m_stmt = m_ctxt->ctxt()->mkTypeProcStmtVarDecl(
        i->getName()->getId(),
        dm_dt,
        false,
        (i->getInit())?TaskBuildExpr(m_ctxt).build(i->getInit()):0
    );

    DEBUG_LEAVE("visitProceduralStmtDataDeclaration");
}

void TaskBuildTypeExecStmt::visitProceduralStmtYield(ast::IProceduralStmtYield *i) {
    DEBUG_ENTER("visitProcedurlStmtYield");
    m_stmt = m_ctxt->ctxt()->mkTypeProcStmtYield();
    DEBUG_LEAVE("visitProcedurlStmtYield");
}

void TaskBuildTypeExecStmt::visitSymbolChildrenScope(ast::ISymbolChildrenScope *i) {
    DEBUG_ENTER("visitSymbolChildrenScope");
    ast::ISymbolChildrenScope *wrapper_s = m_wrapper_s;
    m_wrapper_s = i;
    i->getTarget()->accept(m_this);
    m_wrapper_s = wrapper_s;
    DEBUG_LEAVE("visitSymbolChildrenScope");
}

void TaskBuildTypeExecStmt::visitExecScope(ast::IExecScope *i) {
    DEBUG_ENTER("visitExecScope");
    arl::dm::ITypeProcStmtScope *scope = m_ctxt->ctxt()->mkTypeProcStmtScope();

    m_ctxt->pushSymScope(i);
    for (std::vector<ast::IScopeChildUP>::const_iterator
        it=i->getChildren().begin();
        it!=i->getChildren().end(); it++) {
        arl::dm::ITypeProcStmt *stmt = TaskBuildTypeExecStmt(m_ctxt).build(it->get());
        if (dynamic_cast<arl::dm::ITypeProcStmtVarDecl *>(stmt)) {
            scope->addVariable(dynamic_cast<arl::dm::ITypeProcStmtVarDecl *>(stmt));
        } else if (stmt) {
            scope->addStatement(stmt);
        }
    }
    m_ctxt->popSymScope();

    m_stmt = scope;

    DEBUG_LEAVE("visitExecScope (%d items)", scope->getStatements().size());
}

dmgr::IDebug *TaskBuildTypeExecStmt::m_dbg = 0;

}
}
}
