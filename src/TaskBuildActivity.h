/**
 * TaskBuildActivity.h
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
#pragma once
#include <vector>
#include "dmgr/IDebugMgr.h"
#include "zsp/ast/impl/VisitorBase.h"
#include "zsp/arl/dm/IContext.h"
#include "zsp/arl/dm/IDataTypeAction.h"
#include "zsp/fe/parser/IAst2ArlContext.h"

namespace zsp {
namespace fe {
namespace parser {



class TaskBuildActivity : public ast::VisitorBase {
public:
    TaskBuildActivity(
        IAst2ArlContext         *ctxt
    );

    virtual ~TaskBuildActivity();

    arl::dm::IDataTypeActivity *build(ast::IActivityDecl *activity);

    virtual void visitActivityDecl(ast::IActivityDecl *i) override;

    virtual void visitActivityBindStmt(ast::IActivityBindStmt *i) override;

    virtual void visitActivityConstraint(ast::IActivityConstraint *i) override;

    virtual void visitActivityLabeledStmt(ast::IActivityLabeledStmt *i) override;

    virtual void visitActivityLabeledScope(ast::IActivityLabeledScope *i) override;

    virtual void visitActivityActionHandleTraversal(ast::IActivityActionHandleTraversal *i) override;

    virtual void visitActivityActionTypeTraversal(ast::IActivityActionTypeTraversal *i) override;

    virtual void visitActivitySequence(ast::IActivitySequence *i) override;

    virtual void visitActivityParallel(ast::IActivityParallel *i) override;

    virtual void visitActivitySchedule(ast::IActivitySchedule *i) override;

    virtual void visitActivityRepeatCount(ast::IActivityRepeatCount *i) override;

    virtual void visitActivityRepeatWhile(ast::IActivityRepeatWhile *i) override;

    virtual void visitActivityForeach(ast::IActivityForeach *i) override;

    virtual void visitActivitySelect(ast::IActivitySelect *i) override;

    virtual void visitActivityIfElse(ast::IActivityIfElse *i) override;

    virtual void visitActivityMatch(ast::IActivityMatch *i) override;

    virtual void visitActivityReplicate(ast::IActivityReplicate *i) override;

    virtual void visitActivitySuper(ast::IActivitySuper *i) override;

private:
    static dmgr::IDebug                             *m_dbg;
    IAst2ArlContext                                 *m_ctxt;    
    arl::dm::IDataTypeActivityScope                 *m_activity;
    std::vector<arl::dm::IDataTypeActivityScope *>   m_scope_s;

};

}
}
}

