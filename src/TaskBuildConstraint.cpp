/*
 * TaskBuildConstraint.cpp
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
#include "TaskBuildConstraint.h"
#include "TaskBuildExpr.h"


namespace zsp {
namespace fe {
namespace parser {


TaskBuildConstraint::TaskBuildConstraint(IAst2ArlContext *ctxt) : m_ctxt(ctxt) {
    DEBUG_INIT("zsp::fe::parser::TaskBuildConstraint", ctxt->getDebugMgr());
}

TaskBuildConstraint::~TaskBuildConstraint() {

}

vsc::dm::ITypeConstraint *TaskBuildConstraint::build(ast::IConstraintStmt *c) {
    m_ret = 0;
    c->accept(m_this);
    return m_ret;
}

void TaskBuildConstraint::visitConstraintBlock(ast::IConstraintBlock *i) { 
    DEBUG_ENTER("visitConstraintBlock");
    DEBUG_LEAVE("visitConstraintBlock");
}

void TaskBuildConstraint::visitConstraintScope(ast::IConstraintScope *i) { 
    DEBUG_ENTER("visitConstraintScope");
    vsc::dm::ITypeConstraintScope *scope = m_ctxt->ctxt()->mkTypeConstraintScope();
    for (std::vector<ast::IConstraintStmtUP>::const_iterator
        it=i->getConstraints().begin();
        it!=i->getConstraints().end(); it++) {
        m_ret = 0;
        (*it)->accept(m_this);
        if (m_ret) {
            scope->addConstraint(m_ret);
        }
    }
    m_ret = scope;
    DEBUG_LEAVE("visitConstraintScope");
}

void TaskBuildConstraint::visitConstraintStmtDefault(ast::IConstraintStmtDefault *i) { }

void TaskBuildConstraint::visitConstraintStmtDefaultDisable(ast::IConstraintStmtDefaultDisable *i) { }

void TaskBuildConstraint::visitConstraintStmtExpr(ast::IConstraintStmtExpr *i) { 
    DEBUG_ENTER("visitConstraintStmtExpr");
    vsc::dm::ITypeExpr *expr = TaskBuildExpr(m_ctxt).build(i->getExpr());
    vsc::dm::ITypeConstraintExpr *expr_c = m_ctxt->ctxt()->mkTypeConstraintExpr(expr, true);
    m_ret = expr_c;
    DEBUG_LEAVE("visitConstraintStmtExpr");
}

void TaskBuildConstraint::visitConstraintStmtField(ast::IConstraintStmtField *i) { }

void TaskBuildConstraint::visitConstraintStmtIf(ast::IConstraintStmtIf *i) { 
    vsc::dm::ITypeExpr *cond = TaskBuildExpr(m_ctxt).build(i->getCond());
    m_ret = 0;
    i->getTrue_c()->accept(m_this);
    vsc::dm::ITypeConstraint *true_c = m_ret;
    vsc::dm::ITypeConstraint *false_c = 0;

    if (i->getFalse_c()) {
        m_ret = 0;
        i->getFalse_c()->accept(m_this);
        false_c = m_ret;
    }
    
    vsc::dm::ITypeConstraintIfElse *c = m_ctxt->ctxt()->mkTypeConstraintIfElse(
        cond,
        true_c,
        false_c);
    m_ret = c;
}

void TaskBuildConstraint::visitConstraintStmtImplication(ast::IConstraintStmtImplication *i) { 
    m_ret = 0;
    if (i->getConstraints().size() > 1) {
        vsc::dm::ITypeConstraintScope *cs = m_ctxt->ctxt()->mkTypeConstraintScope();
        for (std::vector<ast::IConstraintStmtUP>::const_iterator
            it=i->getConstraints().begin();
            it!=i->getConstraints().end(); it++) {
            (*it)->accept(m_this);
            if (m_ret) {
                cs->addConstraint(m_ret);
            }
        }
        m_ret = cs;
    } else {
        i->getConstraints().at(0)->accept(m_this);
    }

    vsc::dm::ITypeConstraintImplies *imp = m_ctxt->ctxt()->mkTypeConstraintImplies(
        TaskBuildExpr(m_ctxt).build(i->getCond()),
        m_ret);
    m_ret = imp;
}

void TaskBuildConstraint::visitConstraintStmtForeach(ast::IConstraintStmtForeach *i) { }

void TaskBuildConstraint::visitConstraintStmtForall(ast::IConstraintStmtForall *i) { }

void TaskBuildConstraint::visitConstraintStmtUnique(ast::IConstraintStmtUnique *i) { }

dmgr::IDebug *TaskBuildConstraint::m_dbg = 0;

}
}
}
