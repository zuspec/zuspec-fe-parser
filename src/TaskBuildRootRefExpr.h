/**
 * TaskBuildRootRefExpr.h
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
#include "dmgr/IDebugMgr.h"
#include "vsc/dm/ITypeExpr.h"
#include "zsp/ast/impl/VisitorBase.h"
#include "zsp/fe/parser/IAst2ArlContext.h"

namespace zsp {
namespace fe {
namespace parser {



class TaskBuildRootRefExpr :
    public virtual ast::VisitorBase {
public:
    TaskBuildRootRefExpr(IAst2ArlContext *ctxt);

    virtual ~TaskBuildRootRefExpr();

    vsc::dm::ITypeExpr *build(
        ast::IExprMemberPathElem    *root,
        ast::IScopeChild            *scope,
        int32_t                     field_idx);

    virtual void visitSymbolTypeScope(ast::ISymbolTypeScope *i) override;

    virtual void visitComponent(ast::IComponent *i) override;

private:
    static dmgr::IDebug         *m_dbg;
    IAst2ArlContext             *m_ctxt;
    int32_t                     m_field_idx;    
    vsc::dm::ITypeExpr          *m_ret;

};

}
}
}


