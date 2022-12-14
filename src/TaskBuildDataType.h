/**
 * TaskBuildDataType.h
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
#include <vector>
#include "arl/IContext.h"
#include "zsp/ast/IScopeChild.h"
#include "zsp/ast/ISymbolScope.h"
#include "zsp/ast/ISymbolTypeScope.h"
#include "zsp/ast/impl/VisitorBase.h"
#include "vsc/IDataTypeStruct.h"

namespace zsp {
namespace fe {
namespace parser {



class TaskBuildDataType : public ast::VisitorBase {
public:
    TaskBuildDataType(
        arl::IContext                                           *ctxt,
        std::map<ast::IScopeChild *, vsc::IDataTypeStruct *>    *type_m);

    virtual ~TaskBuildDataType();

    vsc::IDataTypeStruct *build(
        const std::vector<ast::ISymbolScope *>  &scope_s,
        ast::ISymbolTypeScope                   *type);

    virtual void visitSymbolTypeScope(ast::ISymbolTypeScope *i) override;

    virtual void visitAction(ast::IAction *i) override;

    virtual void visitComponent(ast::IComponent *i) override;

    virtual void visitStruct(ast::IStruct *i) override;

private:
    void buildType(
        vsc::IDataTypeStruct    *arl_type,
        ast::ISymbolTypeScope   *ast_type);

    void buildTypeFields(
        std::vector<int32_t>        &off_l,
        vsc::IDataTypeStruct        *arl_type,
        ast::ISymbolTypeScope       *ast_type);

    std::string getNamespacePrefix();

    vsc::IDataTypeStruct *findType(ast::IScopeChild *ast_t);

    ast::IScopeChild *resolvePath(ast::ISymbolRefPath *ref);

private:
    arl::IContext                                               *m_ctxt;
    uint32_t                                                    m_depth;
    std::vector<vsc::IDataTypeStruct *>                         m_type_s;
    std::vector<ast::ISymbolScope *>                            m_scope_s;
    std::map<ast::IScopeChild *, vsc::IDataTypeStruct *>        *m_datatype_m;
    uint32_t                                                    m_field_off;

};

}
}
}


