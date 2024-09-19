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
#include "zsp/parser/impl/TaskResolveSymbolPathRefResult.h"

namespace zsp {
namespace fe {
namespace parser {



class TaskBuildDataType : public ast::VisitorBase {
public:
    TaskBuildDataType(IAst2ArlContext *ctxt);

    virtual ~TaskBuildDataType();

    vsc::dm::IDataType *build(ast::IScopeChild *type);

    vsc::dm::IDataType *build(ast::IExpr *type);

    vsc::dm::IDataType *build(ast::ITypeIdentifier *type);

    virtual void visitSymbolFunctionScope(ast::ISymbolFunctionScope *i) override;

    virtual void visitSymbolTypeScope(ast::ISymbolTypeScope *i) override;

    virtual void visitAction(ast::IAction *i) override;

    virtual void visitActivityDecl(ast::IActivityDecl *i) override;

    virtual void visitComponent(ast::IComponent *i) override;

    virtual void visitDataTypeBool(ast::IDataTypeBool *i) override;

    virtual void visitDataTypeChandle(ast::IDataTypeChandle *i) override;

    virtual void visitDataTypeEnum(ast::IDataTypeEnum *i) override;

    virtual void visitDataTypeInt(ast::IDataTypeInt *i) override;

    virtual void visitDataTypePyObj(ast::IDataTypePyObj *i) override;

    virtual void visitDataTypeString(ast::IDataTypeString *i) override;

    virtual void visitDataTypeUserDefined(ast::IDataTypeUserDefined *i) override;

    virtual void visitExecBlock(ast::IExecBlock *i) override;

    virtual void visitExecScope(ast::IExecScope *i) override;

    virtual void visitStruct(ast::IStruct *i) override;

    virtual void visitTypeIdentifier(ast::ITypeIdentifier *i) override;

    virtual void visitTypeScope(ast::ITypeScope *i) override;

    /**
     * Build body elements
     */
    virtual void visitField(ast::IField *i) override;

    virtual void visitFieldCompRef(ast::IFieldCompRef *i) override;

    virtual void visitFieldRef(ast::IFieldRef *i) override;

    virtual void visitFieldClaim(ast::IFieldClaim *i) override;
    
protected:
    void buildType(
        vsc::dm::IDataTypeStruct    *arl_type,
        ast::ISymbolTypeScope   *ast_type);

    void buildTypeFields(
        std::vector<int32_t>        &off_l,
        vsc::dm::IDataTypeStruct    *arl_type,
        ast::ISymbolTypeScope       *ast_type);

    void buildTypeConstraints(
        std::map<std::string, ast::IConstraintScope *>      &c_inherit_m,
        vsc::dm::IDataTypeStruct                            *arl_type,
        ast::ISymbolTypeScope                               *ast_type);

    void buildTypeExecs(
        std::vector<int32_t>        &off_l,
        vsc::dm::IDataTypeStruct    *arl_type,
        ast::ISymbolTypeScope       *ast_type);

    std::string getNamespacePrefix();

    vsc::dm::IDataType *findType(ast::IScopeChild *ast_t);

    template <class T> T *findTypeT(ast::IScopeChild *ast_t) {
        return dynamic_cast<T *>(findType(ast_t));
    }

    ast::IScopeChild *resolvePath(ast::ISymbolRefPath *ref);

    zsp::parser::TaskResolveSymbolPathRefResult resolveTypePath(ast::ISymbolRefPath *ref);

protected:
    static dmgr::IDebug                                         *m_dbg;
    IAst2ArlContext                                             *m_ctxt;
    uint32_t                                                    m_depth;
    vsc::dm::IDataType                                          *m_type;
    std::vector<vsc::dm::IDataTypeStruct *>                     m_type_s;
    uint32_t                                                    m_field_off;
    std::vector<std::map<std::string,ast::IConstraintScope *>>  m_constraint_m;

};

}
}
}


