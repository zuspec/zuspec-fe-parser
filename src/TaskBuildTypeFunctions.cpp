/*
 * TaskBuildTypeFunctions.cpp
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
#include "TaskBuildDataTypeFunction.h"
#include "TaskBuildTypeFunctions.h"
#include "TaskBuildExpr.h"


namespace zsp {
namespace fe {
namespace parser {


TaskBuildTypeFunctions::TaskBuildTypeFunctions(
    IAst2ArlContext                                 *ctxt,
    vsc::dm::IDataTypeStruct                        *arl_type) : 
    m_ctxt(ctxt), m_arl_type(arl_type) {
    DEBUG_INIT("zsp::fe::parser::TaskBuildTypeFunctions", ctxt->getDebugMgr());
}

TaskBuildTypeFunctions::~TaskBuildTypeFunctions() {

}

void TaskBuildTypeFunctions::build(ast::ISymbolTypeScope *ast_type) {
    m_subtype_c.clear();
    m_depth = 0;
    ast_type->accept(m_this);
}

void TaskBuildTypeFunctions::visitAction(ast::IAction *i) {
    if (m_depth == 0) {
        m_depth++;
        VisitorBase::visitAction(i);
        m_depth--;
    }
}

void TaskBuildTypeFunctions::visitComponent(ast::IComponent *i) {
    if (m_depth == 0) {
        m_depth++;
        VisitorBase::visitComponent(i);
        m_depth--;
    }
}

void TaskBuildTypeFunctions::visitStruct(ast::IStruct *i) {
    if (m_depth == 0) {
        m_depth++;
        VisitorBase::visitStruct(i);
        m_depth--;
    }
}

void TaskBuildTypeFunctions::visitSymbolFunctionScope(ast::ISymbolFunctionScope *i) {
    DEBUG_ENTER("visitSymbolFunctionScope");
    TaskBuildDataTypeFunction(m_ctxt).build(i, true);
    DEBUG_LEAVE("visitSymbolFunctionScope");
}

void TaskBuildTypeFunctions::visitConstraintBlock(ast::IConstraintBlock *i) {
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

void TaskBuildTypeFunctions::visitConstraintScope(ast::IConstraintScope *i) {
    DEBUG_ENTER("visitConstraintScope");
    if (i->getConstraints().size() == 1) {
        VisitorBase::visitConstraintScope(i);
    } else {
        vsc::dm::ITypeConstraintScope *cnstr_s = m_ctxt->ctxt()->mkTypeConstraintScope();
        for (std::vector<ast::IConstraintStmtUP>::const_iterator
            it=i->getConstraints().begin();
            it!=i->getConstraints().end(); it++) {
            m_cnstr = 0;
            (*it)->accept(m_this);
            if (m_cnstr) {
                cnstr_s->addConstraint(m_cnstr);
            }
        }
        m_cnstr = cnstr_s;
    }
    DEBUG_LEAVE("visitConstraintScope");
}

void TaskBuildTypeFunctions::visitConstraintStmtDefault(ast::IConstraintStmtDefault *i) { }

void TaskBuildTypeFunctions::visitConstraintStmtDefaultDisable(ast::IConstraintStmtDefaultDisable *i) { }

void TaskBuildTypeFunctions::visitConstraintStmtExpr(ast::IConstraintStmtExpr *i) { 
    DEBUG_ENTER("visitConstraintStmtExpr");
    vsc::dm::ITypeExpr *expr = TaskBuildExpr(m_ctxt).build(i->getExpr());
    vsc::dm::ITypeConstraintExpr *expr_c = m_ctxt->ctxt()->mkTypeConstraintExpr(expr, true);
    m_cnstr = expr_c;
    DEBUG_LEAVE("visitConstraintStmtExpr");
}

void TaskBuildTypeFunctions::visitConstraintStmtField(ast::IConstraintStmtField *i) { }

void TaskBuildTypeFunctions::visitConstraintStmtIf(ast::IConstraintStmtIf *i) {
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

void TaskBuildTypeFunctions::visitConstraintStmtImplication(ast::IConstraintStmtImplication *i) { 
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

void TaskBuildTypeFunctions::visitConstraintStmtForeach(ast::IConstraintStmtForeach *i) { 
    DEBUG_ENTER("visitConstraintStmtForeach");

    DEBUG_LEAVE("visitConstraintStmtForeach");
}

void TaskBuildTypeFunctions::visitConstraintStmtForall(ast::IConstraintStmtForall *i) { }

void TaskBuildTypeFunctions::visitConstraintStmtUnique(ast::IConstraintStmtUnique *i) { }

void TaskBuildTypeFunctions::visitSymbolTypeScope(ast::ISymbolTypeScope *i) {
    if (!m_depth) {
        // TODO: consider super
        m_depth++;
        VisitorBase::visitSymbolScope(i);
        m_depth--;
    }
}

dmgr::IDebug *TaskBuildTypeFunctions::m_dbg = 0;

}
}
}
