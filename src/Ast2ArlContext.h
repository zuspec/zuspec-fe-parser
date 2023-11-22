/**
 * Ast2ArlContext.h
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
#include <map>
#include "dmgr/IDebugMgr.h"
#include "vsc/dm/IDataTypeStruct.h"
#include "zsp/fe/parser/IAst2ArlContext.h"
#include "zsp/parser/IFactory.h"
#include "zsp/parser/IMarkerListener.h"
#include "zsp/arl/dm/IContext.h"
#include "zsp/ast/IScopeChild.h"

namespace zsp {
namespace fe {
namespace parser {



class Ast2ArlContext : public virtual IAst2ArlContext {
public:
    Ast2ArlContext(
        dmgr::IDebugMgr                     *dmgr,
        zsp::parser::IFactory               *factory,
        zsp::parser::IMarkerListener        *marker_l,
        arl::dm::IContext                   *ctxt,
        ast::ISymbolScope                   *root
    );

    virtual ~Ast2ArlContext();

    virtual dmgr::IDebugMgr *getDebugMgr() const override {
        return m_dmgr;
    }

    virtual zsp::parser::IFactory *factory() const override { 
        return m_factory; 
    }

    virtual arl::dm::IContext *ctxt() const override {
        return m_ctxt;
    }

    virtual ast::ISymbolScope *symScope() const override {
        return m_scope_s.back().back();
    }

    virtual const std::vector<ast::ISymbolScope *> &symScopes() const override {
        return m_scope_s.back();
    }

    virtual void pushSymScope(ast::ISymbolScope *s) override;

    virtual void popSymScope() override;

    virtual void pushSymScopeStack(ast::ISymbolScope *s=0) override;

    virtual void popSymScopeStack() override; 

    virtual ast::IScopeChild *resolveRefPath(const ast::ISymbolRefPath *ref) override;

    virtual int32_t findBottomUpScope(ast::ISymbolScope *scope) override;

    virtual ast::ISymbolScope *typeScope() const override {
        return (m_type_s_idx >= 0 && m_type_s_idx < m_scope_s.back().size())?
            m_scope_s.back().at(m_type_s_idx):0;
    }

    virtual vsc::dm::IDataTypeStruct *findType(ast::IScopeChild *t) override;

    virtual void addType(ast::IScopeChild *t, vsc::dm::IDataTypeStruct *dmt) override;

    vsc::dm::IDataTypeStruct *getType(ast::IScopeChild *t);
    
    virtual ast::ISymbolScope *getRoot() override {
        return m_root;
    }

    virtual std::string getQName(const std::string &name) override;

    virtual void pushIsPyRef(bool v) override;

    virtual void setIsPyRef(bool v) override;

    virtual bool isPyRef() override;

    virtual void popIsPyRef() override;

    virtual void pushBaseExpr(vsc::dm::ITypeExpr *expr) override;
    
    virtual vsc::dm::ITypeExpr *baseExpr() override;

    virtual vsc::dm::ITypeExpr *popBaseExpr() override;

private:


private:
    static dmgr::IDebug                                             *m_dbg;
    dmgr::IDebugMgr                                                 *m_dmgr;
    zsp::parser::IFactory                                           *m_factory;
    zsp::parser::IMarkerListener                                    *m_marker_l;
    arl::dm::IContext                                               *m_ctxt;
    ast::ISymbolScope                                               *m_root;
    zsp::parser::IMarkerUP                                          m_marker;
    std::vector<std::vector<ast::ISymbolScope *>>                   m_scope_s;
    std::map<ast::IScopeChild *, vsc::dm::IDataTypeStruct *>        m_type_m;
    int32_t                                                         m_type_s_idx;
    std::vector<bool>                                               m_pyref_s;
    std::vector<vsc::dm::ITypeExpr *>                               m_base_s;

};

}
}
}


