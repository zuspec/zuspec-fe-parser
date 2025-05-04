/**
 * TaskBuildExpr.h
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
#include "zsp/ast/impl/VisitorBase.h"
#include "zsp/fe/parser/IAst2ArlContext.h"

namespace zsp {
namespace fe {
namespace parser {



class TaskBuildExpr : public ast::VisitorBase {
public:
    TaskBuildExpr(IAst2ArlContext *ctxt);

    virtual ~TaskBuildExpr();

    virtual vsc::dm::ITypeExpr *build(ast::IExpr *e);

    template <class T> T *buildT(ast::IExpr *e) {
        return dynamic_cast<T *>(build(e));
    }

    virtual void visitExprStaticRefPath(ast::IExprStaticRefPath *i) override;

    virtual void visitExprString(ast::IExprString *i) override;

    virtual void visitExprSubscript(ast::IExprSubscript *i) override;

    virtual void visitExprUnary(ast::IExprUnary *i) override;

    virtual void visitExprBin(ast::IExprBin *i) override;

    virtual void visitExprBitSlice(ast::IExprBitSlice *i) override;
    
    virtual void visitExprBool(ast::IExprBool *i) override;
    
    virtual void visitExprCast(ast::IExprCast *i) override;
    
    virtual void visitExprCond(ast::IExprCond *i) override;

    virtual void visitExprDomainOpenRangeList(ast::IExprDomainOpenRangeList *i) override;
    
    virtual void visitExprDomainOpenRangeValue(ast::IExprDomainOpenRangeValue *i) override;
    
    virtual void visitExprHierarchicalId(ast::IExprHierarchicalId *i) override;
    
    virtual void visitExprId(ast::IExprId *i) override;
    
    virtual void visitExprIn(ast::IExprIn *i) override;
    
    virtual void visitExprMemberPathElem(ast::IExprMemberPathElem *i) override;
    
    virtual void visitExprNull(ast::IExprNull *i) override;
    
    virtual void visitExprNumber(ast::IExprNumber *i) override;
    
    virtual void visitExprOpenRangeList(ast::IExprOpenRangeList *i) override;
    
    virtual void visitExprOpenRangeValue(ast::IExprOpenRangeValue *i) override;
    
    virtual void visitExprRefPath(ast::IExprRefPath *i) override;

    virtual void visitExprRefPathId(ast::IExprRefPathId *i) override;
    
    virtual void visitExprRefPathContext(ast::IExprRefPathContext *i) override;
    
    virtual void visitExprRefPathElem(ast::IExprRefPathElem *i) override;
    
    virtual void visitExprRefPathStaticRooted(ast::IExprRefPathStaticRooted *i) override;
    
    virtual void visitExprRefPathSuper(ast::IExprRefPathSuper *i) override;
    
    virtual void visitConstraintScope(ast::IConstraintScope *i) override;
    
    virtual void visitExprSignedNumber(ast::IExprSignedNumber *i) override;

    virtual void visitExprUnsignedNumber(ast::IExprUnsignedNumber *i) override;

    virtual void visitExprRefPathStatic(ast::IExprRefPathStatic *i) override;

    virtual void visitExprRefPathStaticFunc(ast::IExprRefPathStaticFunc *i) override;

    virtual void visitPyImportStmt(ast::IPyImportStmt *i) override;

private:

    using RefElemT=std::pair<vsc::dm::ITypeExpr *, ast::IScopeChild *>;
    using RootRefT=std::pair<vsc::dm::ITypeExpr *, int32_t>;

    vsc::dm::ITypeExpr *expr(ast::IExpr *e);

    RootRefT mkRootFieldRef(ast::IExprRefPathContext *i);

    void extendHierFieldRef(
        vsc::dm::ITypeExprFieldRef      *root_ref,
        ast::IExprRefPathContext        *i);

    vsc::dm::ITypeExpr *buildRefExpr(
        vsc::dm::ITypeExpr              *root,
        ast::IExprRefPathContext        *i,
        int32_t                         idx,
        ast::IScopeChild                *ast_scope);

    vsc::dm::ITypeExpr *buildCall(
        vsc::dm::ITypeExpr              *root,
        ast::IExprMemberPathElem        *elem,
        int32_t                         idx,
        ast::IScopeChild                *ast_scope);

private:
    static dmgr::IDebug             *m_dbg;
    IAst2ArlContext                 *m_ctxt;
    vsc::dm::ITypeExpr              *m_expr;
    vsc::dm::IModelValUP            m_val;

};

}
}
}


