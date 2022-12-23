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
#include "Ast2ArlBuilder.h"
#include "TaskBuildDataType.h"
#include "TaskBuildEnumType.h"

#define DEBUG_ENTER(fmt, ...) \
	fprintf(stdout, "--> Ast2ArlBuilder::"); \
	fprintf(stdout, fmt, ##__VA_ARGS__); \
	fprintf(stdout, "\n")

#define DEBUG(fmt, ...) \
	fprintf(stdout, "Ast2ArlBuilder: "); \
	fprintf(stdout, fmt, ##__VA_ARGS__); \
	fprintf(stdout, "\n")

#define DEBUG_LEAVE(fmt, ...) \
	fprintf(stdout, "<-- Ast2ArlBuilder::"); \
	fprintf(stdout, fmt, ##__VA_ARGS__); \
	fprintf(stdout, "\n")

namespace zsp {
namespace fe {
namespace parser {


Ast2ArlBuilder::Ast2ArlBuilder(zsp::IFactory *factory) :
    m_factory(factory), m_marker(
        factory->mkMarker("", MarkerSeverityE::Error, ast::Location())) {
    m_marker_l = 0;
    m_ctxt = 0;

}

Ast2ArlBuilder::~Ast2ArlBuilder() {

}

void Ast2ArlBuilder::build(
        IMarkerListener         *marker_l,
        ast::ISymbolScope       *root,
        arl::IContext           *ctxt) {
    m_marker_l = marker_l;
    m_ctxt = ctxt;

    root->accept(this);

    m_marker_l = 0;
    m_ctxt = 0;

}

void Ast2ArlBuilder::visitSymbolScope(ast::ISymbolScope *i) {
    m_scope_s.push_back(i);
    visitSymbolScopeChildren(i);
    m_scope_s.pop_back();
}

void Ast2ArlBuilder::visitSymbolTypeScope(ast::ISymbolTypeScope *i) {
    DEBUG_ENTER("visitSymbolTypeScope %s", i->getName().c_str());
    if (!findType(i->getTarget())) {
        DEBUG("Need to build type");
        // We haven't defined this type yet, so go build it
        vsc::IDataTypeStruct *type = TaskBuildDataType(
            m_ctxt, &m_datatype_m).build(
                m_scope_s,
                i
            );
    }
    DEBUG_LEAVE("visitSymbolTypeScope %s", i->getName().c_str());
}

void Ast2ArlBuilder::visitSymbolEnumScope(ast::ISymbolEnumScope *i) {
    DEBUG_ENTER("visitSymbolEnumScope");
    std::string ns_prefix = getNamespacePrefix();

    TaskBuildEnumType(m_ctxt).build(ns_prefix, i);

    DEBUG_LEAVE("visitSymbolEnumScope");
}

vsc::IDataTypeStruct *Ast2ArlBuilder::findType(ast::IScopeChild *ast_t) {
    std::map<ast::IScopeChild *,vsc::IDataTypeStruct *>::const_iterator it;

    if ((it=m_datatype_m.find(ast_t)) != m_datatype_m.end()) {
        return it->second;
    } else {
        return 0;
    }
}

std::string Ast2ArlBuilder::getNamespacePrefix() {
    std::string ret;
    for (std::vector<ast::ISymbolScope *>::const_iterator
        it=m_scope_s.begin();
        it+1!=m_scope_s.end(); it++) {
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
    ast::ISymbolScope *scope = m_scope_s.back();

    for (uint32_t i=0; i<ref->getPath().size(); i++) {
        ret = scope->getChildren().at(i);

        if (i+1 < ref->getPath().size()) {
            scope = dynamic_cast<ast::ISymbolScope *>(ret);
        }
    }

    return ret;
}

}
}
}
