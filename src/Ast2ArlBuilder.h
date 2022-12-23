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
#include "arl/IContext.h"
#include "vsc/IDataTypeStruct.h"
#include "zsp/IFactory.h"
#include "zsp/IMarker.h"
#include "zsp/ast/impl/VisitorBase.h"
#include "zsp/fe/parser/IAst2ArlBuilder.h"

namespace zsp {
namespace fe {
namespace parser {



class Ast2ArlBuilder : 
    public virtual IAst2ArlBuilder,
    public virtual ast::VisitorBase {
public:
    Ast2ArlBuilder(zsp::IFactory *factory);

    virtual ~Ast2ArlBuilder();

    virtual void build(
        IMarkerListener         *marker_l,
        ast::ISymbolScope       *root,
        arl::IContext           *ctxt) override;

    virtual void visitSymbolScope(ast::ISymbolScope *i) override;

    virtual void visitSymbolEnumScope(ast::ISymbolEnumScope *i) override;

    virtual void visitSymbolTypeScope(ast::ISymbolTypeScope *i) override;

private:

    vsc::IDataTypeStruct *findType(ast::IScopeChild *ast_t);

    std::string getNamespacePrefix();

    void visitSymbolScopeChildren(ast::ISymbolScope *i);

    ast::IScopeChild *resolvePath(ast::ISymbolRefPath *ref);

private:
    zsp::IFactory                                           *m_factory;
    IMarkerListener                                         *m_marker_l;
    arl::IContext                                           *m_ctxt;
    IMarkerUP                                               m_marker;
    std::vector<ast::ISymbolScope *>                        m_scope_s;
    std::map<ast::IScopeChild *, vsc::IDataTypeStruct *>    m_datatype_m;
//    std::vector<vsc::IDataTypeStruct *>                     m_type_s;

};

}
}
}


