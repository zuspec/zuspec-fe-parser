/*
 * TaskBuildTypeConstraints.cpp
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
#include "TaskBuildTypeConstraints.h"
#include "TaskBuildExpr.h"


namespace zsp {
namespace fe {
namespace parser {


TaskBuildTypeConstraints::TaskBuildTypeConstraints(
    IAst2ArlContext                                 *ctxt,
    vsc::dm::IDataTypeStruct                        *arl_type) : 
    m_ctxt(ctxt), m_arl_type(arl_type) {
    DEBUG_INIT("zsp::fe::parser::TaskBuildTypeConstraintss", ctxt->getDebugMgr());
}

TaskBuildTypeConstraints::~TaskBuildTypeConstraints() {

}

void TaskBuildTypeConstraints::build(ast::ISymbolTypeScope *ast_type) {
    m_subtype_c.clear();
    m_depth = 0;
    ast_type->accept(m_this);
}

void TaskBuildTypeConstraints::visitAction(ast::IAction *i) {
    if (m_depth == 0) {
        m_depth++;
        VisitorBase::visitAction(i);
        m_depth--;
    }
}

void TaskBuildTypeConstraints::visitComponent(ast::IComponent *i) {
    if (m_depth == 0) {
        m_depth++;
        VisitorBase::visitComponent(i);
        m_depth--;
    }
}

void TaskBuildTypeConstraints::visitStruct(ast::IStruct *i) {
    if (m_depth == 0) {
        m_depth++;
        VisitorBase::visitStruct(i);
        m_depth--;
    }
}

void TaskBuildTypeConstraints::visitConstraintBlock(ast::IConstraintBlock *i) {
    DEBUG_ENTER("visitConstraintBlock %s (%d)", 
        i->getName().c_str(), i->getConstraints().size());

    if (m_subtype_c.find(i->getName()) != m_subtype_c.end()) {
        DEBUG_LEAVE("visitConstraintBlock -- already defined");
        return;
    }

    vsc::dm::ITypeConstraintBlock *cb = 
        m_ctxt->ctxt()->mkTypeConstraintBlock(i->getName());
    for (std::vector<ast::IConstraintStmtUP>::const_iterator
        it=i->getConstraints().begin();
        it!=i->getConstraints().end(); it++) {
        DEBUG("--> accept");
        m_cnstr = 0;
        (*it)->accept(m_this);
        DEBUG("<-- accept (%p)", m_cnstr);
        if (m_cnstr) {
            cb->addConstraint(m_cnstr);
        }
    }
    DEBUG("cb: %d", cb->getConstraints().size());
    m_arl_type->addConstraint(cb, true);

    DEBUG_LEAVE("visitConstraintBlock %s", i->getName().c_str());
}

void TaskBuildTypeConstraints::visitConstraintScope(ast::IConstraintScope *i) {
    DEBUG_ENTER("visitConstraintScope");
    VisitorBase::visitConstraintScope(i);
    DEBUG_LEAVE("visitConstraintScope");
}

void TaskBuildTypeConstraints::visitConstraintStmtDefault(ast::IConstraintStmtDefault *i) { }

void TaskBuildTypeConstraints::visitConstraintStmtDefaultDisable(ast::IConstraintStmtDefaultDisable *i) { }

void TaskBuildTypeConstraints::visitConstraintStmtExpr(ast::IConstraintStmtExpr *i) { 
    DEBUG_ENTER("visitConstraintStmtExpr");
    vsc::dm::ITypeExpr *expr = TaskBuildExpr(m_ctxt).build(i->getExpr());
    vsc::dm::ITypeConstraintExpr *expr_c = m_ctxt->ctxt()->mkTypeConstraintExpr(expr, true);
    m_cnstr = expr_c;
    DEBUG_LEAVE("visitConstraintStmtExpr");
}

void TaskBuildTypeConstraints::visitConstraintStmtField(ast::IConstraintStmtField *i) { }

void TaskBuildTypeConstraints::visitConstraintStmtIf(ast::IConstraintStmtIf *i) {
    DEBUG_ENTER("visitConstraintStmtIf");
    vsc::dm::ITypeExpr *cond = TaskBuildExpr(m_ctxt).build(i->getCond());
    m_cnstr = 0;
    i->getTrue_c()->accept(m_this);
    vsc::dm::ITypeConstraint *true_c = m_cnstr;
    vsc::dm::ITypeConstraint *false_c = 0;

    if (i->getFalse_c()) {
        m_cnstr = 0;
        i->getFalse_c()->accept(m_this);
        false_c = m_cnstr;
    }
    
    vsc::dm::ITypeConstraintIfElse *c = m_ctxt->ctxt()->mkTypeConstraintIfElse(
        cond,
        true_c,
        false_c);
    m_cnstr = c;

    DEBUG_LEAVE("visitConstraintStmtIf");
}

void TaskBuildTypeConstraints::visitConstraintStmtImplication(ast::IConstraintStmtImplication *i) { 
    DEBUG_ENTER("visitConstraintStmtImplication");

    m_cnstr = 0;
    if (i->getConstraints().size() > 1) {
        vsc::dm::ITypeConstraintScope *cs = m_ctxt->ctxt()->mkTypeConstraintScope();
        for (std::vector<ast::IConstraintStmtUP>::const_iterator
            it=i->getConstraints().begin();
            it!=i->getConstraints().end(); it++) {
            (*it)->accept(m_this);
            if (m_cnstr) {
                cs->addConstraint(m_cnstr);
            }
        }
        m_cnstr = cs;
    } else {
        i->getConstraints().at(0)->accept(m_this);
    }

    vsc::dm::ITypeConstraintImplies *imp = m_ctxt->ctxt()->mkTypeConstraintImplies(
        TaskBuildExpr(m_ctxt).build(i->getCond()),
        m_cnstr);
    m_cnstr = imp;

    DEBUG_LEAVE("visitConstraintStmtImplication");
}

void TaskBuildTypeConstraints::visitConstraintStmtForeach(ast::IConstraintStmtForeach *i) { }

void TaskBuildTypeConstraints::visitConstraintStmtForall(ast::IConstraintStmtForall *i) { }

void TaskBuildTypeConstraints::visitConstraintStmtUnique(ast::IConstraintStmtUnique *i) { }

void TaskBuildTypeConstraints::visitSymbolTypeScope(ast::ISymbolTypeScope *i) {
    if (!m_depth) {
        // TODO: consider super
        m_depth++;
        VisitorBase::visitSymbolScope(i);
        m_depth--;
    }
}

dmgr::IDebug *TaskBuildTypeConstraints::m_dbg = 0;

}
}
}
