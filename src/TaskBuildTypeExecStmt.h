/**
 * TaskBuildTypeExecStmt.h
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
#include "zsp/fe/parser/IAst2ArlContext.h"
#include "vsc/dm/ITypeExpr.h"
#include "zsp/arl/dm/ITypeProcStmt.h"
#include "zsp/arl/dm/ITypeProcStmtScope.h"
#include "zsp/ast/impl/VisitorBase.h"

namespace zsp {
namespace fe {
namespace parser {



class TaskBuildTypeExecStmt : public ast::VisitorBase {
public:
    TaskBuildTypeExecStmt(IAst2ArlContext *ctxt);

    virtual ~TaskBuildTypeExecStmt();

    virtual arl::dm::ITypeProcStmt *build(ast::IScopeChild *stmt);

    virtual void visitProceduralStmtAssignment(ast::IProceduralStmtAssignment *i) override;
    
    virtual void visitProceduralStmtExpr(ast::IProceduralStmtExpr *i) override;
    
    virtual void visitProceduralStmtFunctionCall(ast::IProceduralStmtFunctionCall *i) override;
    
    virtual void visitProceduralStmtReturn(ast::IProceduralStmtReturn *i) override;
    
    virtual void visitProceduralStmtRepeat(ast::IProceduralStmtRepeat *i) override;
    
    virtual void visitProceduralStmtRepeatWhile(ast::IProceduralStmtRepeatWhile *i) override;
    
    virtual void visitProceduralStmtWhile(ast::IProceduralStmtWhile *i) override;
    
    virtual void visitProceduralStmtForeach(ast::IProceduralStmtForeach *i) override;
    
    virtual void visitProceduralStmtIfElse(ast::IProceduralStmtIfElse *i) override;
    
    virtual void visitProceduralStmtMatch(ast::IProceduralStmtMatch *i) override;
    
    virtual void visitProceduralStmtMatchChoice(ast::IProceduralStmtMatchChoice *i) override;
    
    virtual void visitProceduralStmtBreak(ast::IProceduralStmtBreak *i) override;
    
    virtual void visitProceduralStmtContinue(ast::IProceduralStmtContinue *i) override;
    
    virtual void visitProceduralStmtDataDeclaration(ast::IProceduralStmtDataDeclaration *i) override;

    virtual void visitExecScope(ast::IExecScope *i) override;

    virtual void visitSymbolChildrenScope(ast::ISymbolChildrenScope *i) override;

private:
    static dmgr::IDebug                             *m_dbg;
    IAst2ArlContext                                 *m_ctxt;
    ast::IExecStmt                                  *m_root;
    ast::ISymbolChildrenScope                       *m_wrapper_s;
    arl::dm::ITypeProcStmt                          *m_stmt;

};

}
}
}


