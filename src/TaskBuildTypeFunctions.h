/**
 * TaskBuildTypeFunctions.h
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
#pragma once
#include <map>
#include <string>
#include "dmgr/IDebug.h"
#include "vsc/dm/IDataTypeStruct.h"
#include "zsp/fe/parser/IAst2ArlContext.h"
#include "zsp/ast/IConstraintScope.h"
#include "zsp/ast/impl/VisitorBase.h"

namespace zsp {
namespace fe {
namespace parser {



class TaskBuildTypeFunctions : public virtual ast::VisitorBase {
public:
    TaskBuildTypeFunctions(
        IAst2ArlContext                                 *ctxt,
        vsc::dm::IDataTypeStruct                        *arl_type);

    virtual ~TaskBuildTypeFunctions();

    void build(ast::IScopeChild *ast_type);
    
    virtual void visitAction(ast::IAction *i) override;

    virtual void visitActivityDecl(ast::IActivityDecl *i) override { }

    virtual void visitComponent(ast::IComponent *i) override;

    virtual void visitStruct(ast::IStruct *i) override;

    virtual void visitField(ast::IField *i) override { }

    virtual void visitSymbolFunctionScope(ast::ISymbolFunctionScope *i) override;
        
    virtual void visitConstraintBlock(ast::IConstraintBlock *i) override;

    virtual void visitConstraintScope(ast::IConstraintScope *i) override;

    virtual void visitConstraintStmtDefault(ast::IConstraintStmtDefault *i) override;

    virtual void visitConstraintStmtDefaultDisable(ast::IConstraintStmtDefaultDisable *i) override;

    virtual void visitConstraintStmtExpr(ast::IConstraintStmtExpr *i) override;

    virtual void visitConstraintStmtField(ast::IConstraintStmtField *i) override;

    virtual void visitConstraintStmtIf(ast::IConstraintStmtIf *i) override;

    virtual void visitConstraintStmtImplication(ast::IConstraintStmtImplication *i) override;

    virtual void visitConstraintStmtForeach(ast::IConstraintStmtForeach *i) override;

    virtual void visitConstraintStmtForall(ast::IConstraintStmtForall *i) override;

    virtual void visitConstraintStmtUnique(ast::IConstraintStmtUnique *i) override;

    virtual void visitSymbolTypeScope(ast::ISymbolTypeScope *i) override;


private:
    static dmgr::IDebug                                 *m_dbg;
    IAst2ArlContext                                     *m_ctxt;
    uint32_t                                            m_depth;
    std::map<std::string, ast::IConstraintScope *>      m_subtype_c;
    vsc::dm::IDataTypeStruct                            *m_arl_type;
//    std::vector<vsc::dm::ITypeConstraintScope *>        m_scope_s;
    vsc::dm::ITypeConstraint                            *m_cnstr;
};

}
}
}


