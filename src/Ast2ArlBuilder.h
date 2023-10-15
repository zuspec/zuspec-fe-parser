/**
 * Ast2ArlBuilder.h
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
#include "Ast2ArlContext.h"
#include "dmgr/IDebugMgr.h"
#include "zsp/arl/dm/IContext.h"
#include "vsc/dm/IDataTypeStruct.h"
#include "zsp/parser/IFactory.h"
#include "zsp/parser/IMarker.h"
#include "zsp/ast/impl/VisitorBase.h"
#include "zsp/fe/parser/IAst2ArlBuilder.h"

namespace zsp {
namespace fe {
namespace parser {



class Ast2ArlBuilder : 
    public virtual IAst2ArlBuilder,
    public virtual ast::VisitorBase {
public:
    Ast2ArlBuilder(
        dmgr::IDebugMgr             *dmgr,
        zsp::parser::IFactory       *factory);

    virtual ~Ast2ArlBuilder();

    virtual void build(
        ast::ISymbolScope               *root,
        IAst2ArlContext                 *ctxt) override;

    virtual void visitExtendType(ast::IExtendType *i) override { }

    virtual void visitSymbolScope(ast::ISymbolScope *i) override;

    virtual void visitSymbolEnumScope(ast::ISymbolEnumScope *i) override;

    virtual void visitSymbolExtendScope(ast::ISymbolExtendScope *i) override { }


    virtual void visitSymbolFunctionScope(ast::ISymbolFunctionScope *i) override;

    virtual void visitSymbolTypeScope(ast::ISymbolTypeScope *i) override;

private:

    std::string getNamespacePrefix();

    void visitSymbolScopeChildren(ast::ISymbolScope *i);

    ast::IScopeChild *resolvePath(ast::ISymbolRefPath *ref);

private:
    static dmgr::IDebug                                       *m_dbg;
    dmgr::IDebugMgr                                           *m_dmgr;
    IAst2ArlContext                                           *m_ctxt;

};

}
}
}


