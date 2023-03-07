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
#include "dmgr/IDebug.h"
#include "zsp/arl/dm/IContext.h"
#include "zsp/ast/IScopeChild.h"
#include "zsp/ast/ISymbolScope.h"
#include "zsp/ast/ISymbolTypeScope.h"
#include "zsp/ast/impl/VisitorBase.h"
#include "zsp/fe/parser/IAst2ArlContext.h"
#include "vsc/dm/IDataTypeStruct.h"

namespace zsp {
namespace fe {
namespace parser {



class TaskBuildDataType : public ast::VisitorBase {
public:
    TaskBuildDataType(IAst2ArlContext *ctxt);

    virtual ~TaskBuildDataType();

    vsc::dm::IDataType *build(ast::IScopeChild *type);

    virtual void visitSymbolTypeScope(ast::ISymbolTypeScope *i) override;

    virtual void visitAction(ast::IAction *i) override;

    virtual void visitComponent(ast::IComponent *i) override;

    virtual void visitDataTypeBool(ast::IDataTypeBool *i) override;

    virtual void visitDataTypeChandle(ast::IDataTypeChandle *i) override;

    virtual void visitDataTypeEnum(ast::IDataTypeEnum *i) override;

    virtual void visitDataTypeInt(ast::IDataTypeInt *i) override;

    virtual void visitDataTypeString(ast::IDataTypeString *i) override;

    virtual void visitDataTypeUserDefined(ast::IDataTypeUserDefined *i) override;

    virtual void visitStruct(ast::IStruct *i) override;

private:
    void buildType(
        vsc::dm::IDataTypeStruct    *arl_type,
        ast::ISymbolTypeScope   *ast_type);

    void buildTypeFields(
        std::vector<int32_t>        &off_l,
        vsc::dm::IDataTypeStruct    *arl_type,
        ast::ISymbolTypeScope       *ast_type);

    std::string getNamespacePrefix();

    vsc::dm::IDataType *findType(ast::IScopeChild *ast_t);

    template <class T> T *findTypeT(ast::IScopeChild *ast_t) {
        return dynamic_cast<T *>(findType(ast_t));
    }

    ast::IScopeChild *resolvePath(ast::ISymbolRefPath *ref);

private:
    static dmgr::IDebug                                         *m_dbg;
    IAst2ArlContext                                             *m_ctxt;
    uint32_t                                                    m_depth;
    vsc::dm::IDataType                                          *m_type;
    std::vector<vsc::dm::IDataTypeStruct *>                     m_type_s;
    uint32_t                                                    m_field_off;

};

}
}
}


