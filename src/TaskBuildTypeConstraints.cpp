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
    DEBUG_ENTER("visitConstraintBlock %s", i->getName().c_str());

    if (m_subtype_c.find(i->getName()) !=m_subtype_c.end()) {
        DEBUG_LEAVE("visitConstraintBlock -- already defined");
        return;
    }

    vsc::dm::ITypeConstraintBlock *cb = 
        m_ctxt->ctxt()->mkTypeConstraintBlock(i->getName());
    m_scope_s.push_back(cb);
    for (std::vector<ast::IConstraintStmtUP>::const_iterator
        it=i->getConstraints().begin();
        it!=i->getConstraints().end(); it++) {
        (*it)->accept(m_this);
    }
    m_scope_s.pop_back();
    m_arl_type->addConstraint(cb, true);

    DEBUG_LEAVE("visitConstraintBlock %s", i->getName().c_str());
}

void TaskBuildTypeConstraints::visitConstraintStmtDefault(ast::IConstraintStmtDefault *i) { }

void TaskBuildTypeConstraints::visitConstraintStmtDefaultDisable(ast::IConstraintStmtDefaultDisable *i) { }

void TaskBuildTypeConstraints::visitConstraintStmtExpr(ast::IConstraintStmtExpr *i) { 
    DEBUG_ENTER("visitConstraintStmtExpr");
    vsc::dm::ITypeExpr *expr = TaskBuildExpr(m_ctxt).build(i->getExpr());
    vsc::dm::ITypeConstraintExpr *expr_c = m_ctxt->ctxt()->mkTypeConstraintExpr(expr, true);
    m_scope_s.back()->addConstraint(expr_c, true);
    DEBUG_LEAVE("visitConstraintStmtExpr");
}

void TaskBuildTypeConstraints::visitConstraintStmtField(ast::IConstraintStmtField *i) { }

void TaskBuildTypeConstraints::visitConstraintStmtIf(ast::IConstraintStmtIf *i) { }

void TaskBuildTypeConstraints::visitConstraintStmtImplication(ast::IConstraintStmtImplication *i) { }

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
