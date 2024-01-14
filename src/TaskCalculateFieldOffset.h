/**
 * TaskCalculateFieldOffset.h
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
#include "zsp/ast/impl/VisitorBase.h"
#include "zsp/fe/parser/IAst2ArlContext.h"

namespace zsp {
namespace fe {
namespace parser {



class TaskCalculateFieldOffset : public ast::VisitorBase {
public:
    struct Res {
        int32_t             index;
        ast::IScopeChild    *target;
    };

    TaskCalculateFieldOffset(IAst2ArlContext *ctxt);

    virtual ~TaskCalculateFieldOffset();

    Res calculate(
        ast::IScopeChild    *scope,
        int32_t             field_idx,
        int32_t             super_idx);

    virtual void visitDataTypeUserDefined(ast::IDataTypeUserDefined *i) override;

    virtual void visitField(ast::IField *i) override;

    virtual void visitFieldCompRef(ast::IFieldCompRef *i) override;

    virtual void visitFieldRef(ast::IFieldRef *i) override;

    virtual void visitFieldClaim(ast::IFieldClaim *i) override;

    virtual void visitProceduralStmtDataDeclaration(ast::IProceduralStmtDataDeclaration *i) override;

    virtual void visitSymbolChildrenScope(ast::ISymbolChildrenScope *i) override;

    virtual void visitSymbolTypeScope(ast::ISymbolTypeScope *i) override;

private:
    static dmgr::IDebug             *m_dbg;
    IAst2ArlContext                 *m_ctxt;
    ast::IScopeChild                *m_target;
    bool                            m_hit_field;
    int32_t                         m_depth;
    int32_t                         m_super_depth; 
    int32_t                         m_field_idx;
    int32_t                         m_super_idx;
    int32_t                         m_field_cnt;

};

}
}
}


