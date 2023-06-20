/*
 * Ast2ArlBuilder.cpp
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
#include "dmgr/impl/DebugMacros.h"
#include "Ast2ArlBuilder.h"
#include "TaskBuildDataType.h"
#include "TaskBuildEnumType.h"
#include "TaskBuildExecBody.h"
#include "TaskBuildExpr.h"

namespace zsp {
namespace fe {
namespace parser {


Ast2ArlBuilder::Ast2ArlBuilder(
    dmgr::IDebugMgr         *dmgr,
    zsp::parser::IFactory   *factory) : m_dmgr(dmgr) {
    DEBUG_INIT("Ast2ArlBuilder", dmgr);
    m_ctxt = 0;

}

Ast2ArlBuilder::~Ast2ArlBuilder() {

}

void Ast2ArlBuilder::build(
        ast::ISymbolScope               *root,
        IAst2ArlContext                 *ctxt) {
    m_ctxt = ctxt;

    m_ctxt->pushSymScopeStack(root);
    root->accept(this);
    m_ctxt->popSymScopeStack();

    m_ctxt = 0;
}

void Ast2ArlBuilder::visitSymbolScope(ast::ISymbolScope *i) {
    m_ctxt->pushSymScope(i);
    visitSymbolScopeChildren(i);
    m_ctxt->popSymScope();
}

void Ast2ArlBuilder::visitSymbolTypeScope(ast::ISymbolTypeScope *i) {
    DEBUG_ENTER("visitSymbolTypeScope %s", i->getName().c_str());
    if (!m_ctxt->findType(i->getTarget())) {
        DEBUG("Need to build type");
        // We haven't defined this type yet, so go build it
        vsc::dm::IDataType *type = TaskBuildDataType(m_ctxt).build(i);
    }
    DEBUG_LEAVE("visitSymbolTypeScope %s", i->getName().c_str());
}

void Ast2ArlBuilder::visitSymbolEnumScope(ast::ISymbolEnumScope *i) {
    DEBUG_ENTER("visitSymbolEnumScope");
    std::string ns_prefix = getNamespacePrefix();

    TaskBuildEnumType(m_ctxt).build(ns_prefix, i);

    DEBUG_LEAVE("visitSymbolEnumScope");
}

static std::map<ast::ParamDir, arl::dm::ParamDir> param_dir_m = {
    {ast::ParamDir::ParamDir_Default, arl::dm::ParamDir::In},
    {ast::ParamDir::ParamDir_In, arl::dm::ParamDir::In},
    {ast::ParamDir::ParamDir_Out, arl::dm::ParamDir::Out},
    {ast::ParamDir::ParamDir_InOut, arl::dm::ParamDir::InOut} 
};

void Ast2ArlBuilder::visitSymbolFunctionScope(ast::ISymbolFunctionScope *i) {
    DEBUG_ENTER("visitSymbolFunctionScope \"%s\"", i->getName().c_str());

    ast::IFunctionPrototype *proto = i->getPrototypes().at(0);
//    ast::IScopeChild *rtype = i->getDefinition()->getProto()->getRtype();
    ast::IScopeChild *rtype = proto->getRtype();
    arl::dm::IDataTypeFunction *func = m_ctxt->ctxt()->mkDataTypeFunction(
        i->getName(),
        rtype?TaskBuildDataType(m_ctxt).build(rtype):0,
        false);

    // Bring across the function parameters
    for (std::vector<ast::IFunctionParamDeclUP>::const_iterator
        it=proto->getParameters().begin();
        it!=proto->getParameters().end(); it++) {
        std::string name = (*it)->getName()->getId();
        arl::dm::ParamDir dir = param_dir_m.find((*it)->getDir())->second;
        vsc::dm::IDataType *type = TaskBuildDataType(m_ctxt).build((*it)->getType());
        vsc::dm::ITypeExpr *dflt = ((*it)->getDflt())?TaskBuildExpr(m_ctxt).build((*it)->getDflt()):0;
        arl::dm::IDataTypeFunctionParamDecl *param = 
        m_ctxt->ctxt()->mkDataTypeFunctionParamDecl(
            name,
            dir,
            type,
            false,
            dflt
        );
        func->addParameter(param);
    }
    m_ctxt->ctxt()->addDataTypeFunction(func);

    if (i->getDefinition()) {
        // Local implementation
        DEBUG("PSS-native function");

        m_ctxt->pushSymScope(i);


        m_ctxt->pushSymScope(i->getBody());
        // Build the function body
        TaskBuildExecBody(m_ctxt).build(
            func->getBody(),
            i->getDefinition()->getBody()
        );

        m_ctxt->popSymScope();
        m_ctxt->popSymScope();
    } else {
        // 
        DEBUG("Import function");
        func->addImportSpec(m_ctxt->ctxt()->mkDataTypeFunctionImport(""));
    }

    DEBUG_LEAVE("visitSymbolFunctionScope %s", i->getName().c_str());
}

std::string Ast2ArlBuilder::getNamespacePrefix() {
    std::string ret;
    for (std::vector<ast::ISymbolScope *>::const_iterator
        it=m_ctxt->symScopes().begin();
        it+1!=m_ctxt->symScopes().end(); it++) {
        if ((*it)->getName() != "") {
            ret += (*it)->getName();
            ret += "::";
        }
    }
    return ret;
}

void Ast2ArlBuilder::visitSymbolScopeChildren(ast::ISymbolScope *i) {
    for (std::vector<ast::IScopeChild *>::const_iterator
        it=i->getChildren().begin();
        it!=i->getChildren().end(); it++) {
        (*it)->accept(this);
    }
}

ast::IScopeChild *Ast2ArlBuilder::resolvePath(ast::ISymbolRefPath *ref) {
    ast::IScopeChild *ret = 0;
    ast::ISymbolScope *scope = m_ctxt->symScope();

    for (uint32_t i=0; i<ref->getPath().size(); i++) {
        ret = scope->getChildren().at(i);

        if (i+1 < ref->getPath().size()) {
            scope = dynamic_cast<ast::ISymbolScope *>(ret);
        }
    }

    return ret;
}

dmgr::IDebug *Ast2ArlBuilder::m_dbg = 0;

}
}
}
