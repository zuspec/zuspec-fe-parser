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
#include "TaskBuildDataTypeFunction.h"
#include "TaskBuildEnumType.h"
#include "TaskBuildExpr.h"
#include "TaskBuildTypeExecStmt.h"
#include "TaskDeclareTypes.h"
#include "TaskDeclareTypeBodies.h"
#include "TaskDeclareTypeFields.h"
#include "TaskLinkBuiltinTypeElemFactories.h"

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
    DEBUG_ENTER("build %s", root->getName().c_str());
    m_ctxt = ctxt;

    TaskLinkBuiltinTypeElemFactories(ctxt).link(root);

    m_ctxt->pushSymScopeStack(root);
    TaskDeclareTypes(ctxt).build(root);
    TaskDeclareTypeFields(ctxt).build(root);
    TaskDeclareTypeBodies(ctxt).build(root);
    root->accept(this);
    m_ctxt->popSymScopeStack();

    m_ctxt = 0;
    DEBUG_LEAVE("build %s", root->getName().c_str());
}

void Ast2ArlBuilder::visitSymbolScope(ast::ISymbolScope *i) {
    m_ctxt->pushSymScope(i);
    visitSymbolScopeChildren(i);
    m_ctxt->popSymScope();
}

void Ast2ArlBuilder::visitSymbolTypeScope(ast::ISymbolTypeScope *i) {
    DEBUG_ENTER("visitSymbolTypeScope %s", i->getName().c_str());
    ast::ITypeScope *i_t = dynamic_cast<ast::ITypeScope *>(i->getTarget());
    if (i->getSpec_types().size()) {
        DEBUG("Type has specializations. Processing those");
        for (std::vector<ast::ISymbolTypeScopeUP>::const_iterator
            it=i->getSpec_types().begin();
            it!=i->getSpec_types().end(); it++) {
            (*it)->accept(m_this);
        }
    } else if (i_t->getParams()) {
        DEBUG("Skipping unspecialized parameterized type %s", i->getName().c_str());
    } else {
        if (!m_ctxt->findType(i->getTarget())) {
            DEBUG("Need to build type");
            // We haven't defined this type yet, so go build it
            vsc::dm::IDataType *type = TaskBuildDataType(m_ctxt).buildT<vsc::dm::IDataType>(i);
        }
    }
    DEBUG_LEAVE("visitSymbolTypeScope %s", i->getName().c_str());
}

void Ast2ArlBuilder::visitSymbolEnumScope(ast::ISymbolEnumScope *i) {
    DEBUG_ENTER("visitSymbolEnumScope");
    std::string ns_prefix = getNamespacePrefix();

    TaskBuildEnumType(m_ctxt).build(ns_prefix, i);

    DEBUG_LEAVE("visitSymbolEnumScope");
}

void Ast2ArlBuilder::visitSymbolFunctionScope(ast::ISymbolFunctionScope *i) {
    DEBUG_ENTER("visitSymbolFunctionScope \"%s\"", i->getName().c_str());
    TaskBuildDataTypeFunction(m_ctxt).build(i);
    DEBUG_LEAVE("visitSymbolFunctionScope %s", i->getName().c_str());
}

std::string Ast2ArlBuilder::getNamespacePrefix() {
    std::string ret;
    for (std::vector<zsp::parser::ScopeUtil>::const_iterator
        it=m_ctxt->symScopes().begin();
        it+1!=m_ctxt->symScopes().end(); it++) {
        zsp::parser::ScopeUtil &util = const_cast<zsp::parser::ScopeUtil &>(*it);
        if (util.getName() != "") {
            ret += util.getName();
            ret += "::";
        }
    }
    return ret;
}

void Ast2ArlBuilder::visitSymbolScopeChildren(ast::ISymbolScope *i) {
    for (std::vector<ast::IScopeChildUP>::const_iterator
        it=i->getChildren().begin();
        it!=i->getChildren().end(); it++) {
        (*it)->accept(this);
    }
}

ast::IScopeChild *Ast2ArlBuilder::resolvePath(ast::ISymbolRefPath *ref) {
    ast::IScopeChild *ret = 0;
    ast::ISymbolChildrenScope *scope = m_ctxt->symScope();

    for (uint32_t i=0; i<ref->getPath().size(); i++) {
        ret = scope->getChildren().at(i).get();

        if (i+1 < ref->getPath().size()) {
            scope = dynamic_cast<ast::ISymbolChildrenScope *>(ret);
        }
    }

    return ret;
}

dmgr::IDebug *Ast2ArlBuilder::m_dbg = 0;

}
}
}
