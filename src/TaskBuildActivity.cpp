/*
 * TaskBuildActivity.cpp
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
#include "TaskBuildActivity.h"
#include "TaskBuildExpr.h"
#include "zsp/arl/dm/IDataTypeActivityTraverseType.h"
#include "zsp/ast/IActivityDecl.h"
#include "zsp/parser/impl/TaskResolveSymbolPathRef.h"


namespace zsp {
namespace fe {
namespace parser {


TaskBuildActivity::TaskBuildActivity(IAst2ArlContext *ctxt) : m_ctxt(ctxt) {
    DEBUG_INIT("zsp::fe::parser::TaskBuildActivity", ctxt->getDebugMgr());
}

TaskBuildActivity::~TaskBuildActivity() {

}

arl::dm::IDataTypeActivity *TaskBuildActivity::build(ast::IActivityDecl *activity) {
    DEBUG_ENTER("build");
    m_activity = m_ctxt->ctxt()->mkDataTypeActivitySequence();

    m_scope_s.push_back(m_activity);
    activity->accept(this);
    m_scope_s.pop_back();

    DEBUG_LEAVE("build");
    return m_activity;
}


void TaskBuildActivity::visitActivityDecl(ast::IActivityDecl *i) { 
    DEBUG_ENTER("visitActivityDecl");
    VisitorBase::visitActivityDecl(i);
    DEBUG_LEAVE("visitActivityDecl");
}

void TaskBuildActivity::visitActivityBindStmt(ast::IActivityBindStmt *i) { 
    DEBUG_ENTER("visitActivityBindStmt");
    DEBUG("TODO: visitActivityBindStmt");
    DEBUG_LEAVE("visitActivityBindStmt");
}

void TaskBuildActivity::visitActivityConstraint(ast::IActivityConstraint *i) { 
    DEBUG_ENTER("visitActivityConstraint");
    DEBUG("TODO: visitActivityConstraint");
    DEBUG_LEAVE("visitActivityConstraint");
}

void TaskBuildActivity::visitActivityLabeledStmt(ast::IActivityLabeledStmt *i) { 
    DEBUG_ENTER("visitActivityLabeledStmt");
    DEBUG("TODO: visitActivityLabeledStmt");
    DEBUG_LEAVE("visitActivityLabeledStmt");
}

void TaskBuildActivity::visitActivityLabeledScope(ast::IActivityLabeledScope *i) { 
    DEBUG_ENTER("visitActivityLabeledScope");
    DEBUG("TODO: visitActivityLabeledScope");
    DEBUG_LEAVE("visitActivityLabeledScope");
}

void TaskBuildActivity::visitActivityActionHandleTraversal(ast::IActivityActionHandleTraversal *i) { 
    DEBUG_ENTER("visitActivityActionHandleTraversal");
    vsc::dm::ITypeExprFieldRef *ref = 
        TaskBuildExpr(m_ctxt).buildT<vsc::dm::ITypeExprFieldRef>(i->getTarget());
    vsc::dm::ITypeConstraint *with_c = 0;
    DEBUG("  ref=%p with_c=%p", ref, with_c);

    arl::dm::IDataTypeActivityTraverse *t = 
        m_ctxt->ctxt()->mkDataTypeActivityTraverse(ref, with_c);
    m_scope_s.back()->addActivity(m_ctxt->ctxt()->mkTypeFieldActivity(
        "",
        t,
        true
    ));

    DEBUG_LEAVE("visitActivityActionHandleTraversal");
}

void TaskBuildActivity::visitActivityActionTypeTraversal(ast::IActivityActionTypeTraversal *i) { 
    DEBUG_ENTER("visitActivityActionTypeTraversal");
    arl::dm::IDataTypeActivitySequence *seq = 
        dynamic_cast<arl::dm::IDataTypeActivitySequence *>(m_scope_s.back());
    vsc::dm::ITypeConstraint *with_c = 0;

    ast::IScopeChild *t = m_ctxt->resolveRefPath(i->getTarget()->getType_id()->getTarget());
    ast::ISymbolTypeScope *ts = dynamic_cast<ast::ISymbolTypeScope *>(t);
    DEBUG("ts.name=%s", ts->getName().c_str());
    DEBUG("t=%p id=%s", t, i->getTarget()->getType_id()->getElems().at(0)->getId()->getId().c_str());
    for (uint32_t j=0; j<i->getTarget()->getType_id()->getElems().size(); j++) {
        DEBUG("  Elem: %s", i->getTarget()->getType_id()->getElems().at(j)->getId()->getId().c_str());
    }
    vsc::dm::IDataTypeStruct *dt = dynamic_cast<vsc::dm::IDataTypeStruct *>(m_ctxt->getType(t));

    if (dt) {
        arl::dm::IDataTypeAction *at = dynamic_cast<arl::dm::IDataTypeAction *>(dt);
        if (!at) {
            DEBUG("TODO: bad type (%s)", dt->name().c_str());
        }
        arl::dm::IDataTypeActivityTraverseType *dt_t = 
            m_ctxt->ctxt()->mkDataTypeActivityTraverseType(
                at,
                with_c
            );
        seq->addActivity(m_ctxt->ctxt()->mkTypeFieldActivity("", dt_t, true));
    } else {
        DEBUG("TODO: failed to find type");
    }
    DEBUG_LEAVE("visitActivityActionTypeTraversal");
}

void TaskBuildActivity::visitActivitySequence(ast::IActivitySequence *i) { 
    DEBUG_ENTER("visitActivitySequence");
    arl::dm::IDataTypeActivitySequence *seq = m_ctxt->ctxt()->mkDataTypeActivitySequence();
    m_scope_s.push_back(seq);
    m_scope_s.back()->addActivity(m_ctxt->ctxt()->mkTypeFieldActivity("", seq, true));
    for (std::vector<ast::IScopeChildUP>::const_iterator
        it=i->getChildren().begin();
        it!=i->getChildren().end(); it++) {
        (*it)->accept(m_this);
    }
    m_scope_s.pop_back();
    DEBUG_LEAVE("visitActivitySequence");
}

void TaskBuildActivity::visitActivityParallel(ast::IActivityParallel *i) { 
    DEBUG_ENTER("visitActivityParallel");
    arl::dm::IDataTypeActivityParallel *par = m_ctxt->ctxt()->mkDataTypeActivityParallel();
    m_scope_s.back()->addActivity(m_ctxt->ctxt()->mkTypeFieldActivity("", par, true));
    for (std::vector<ast::IScopeChildUP>::const_iterator
        it=i->getChildren().begin();
        it!=i->getChildren().end(); it++) {
        arl::dm::IDataTypeActivitySequence *branch = m_ctxt->ctxt()->mkDataTypeActivitySequence();

        m_scope_s.push_back(branch);

        (*it)->accept(m_this);

        par->addActivity(m_ctxt->ctxt()->mkTypeFieldActivity("", branch, true));
        m_scope_s.pop_back();
    }
    DEBUG_LEAVE("visitActivityParallel");
}

void TaskBuildActivity::visitActivitySchedule(ast::IActivitySchedule *i) { 
    DEBUG_ENTER("visitActivitySchedule");
    DEBUG("TODO: visitActivitySchedule");
    DEBUG_LEAVE("visitActivitySchedule");
}

void TaskBuildActivity::visitActivityRepeatCount(ast::IActivityRepeatCount *i) { }

void TaskBuildActivity::visitActivityRepeatWhile(ast::IActivityRepeatWhile *i) { }

void TaskBuildActivity::visitActivityForeach(ast::IActivityForeach *i) { }

void TaskBuildActivity::visitActivitySelect(ast::IActivitySelect *i) { }

void TaskBuildActivity::visitActivityIfElse(ast::IActivityIfElse *i) { }

void TaskBuildActivity::visitActivityMatch(ast::IActivityMatch *i) { }

void TaskBuildActivity::visitActivityReplicate(ast::IActivityReplicate *i) { }

void TaskBuildActivity::visitActivitySuper(ast::IActivitySuper *i) { }

dmgr::IDebug *TaskBuildActivity::m_dbg = 0;

}
}
}
