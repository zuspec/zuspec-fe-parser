/*
 * Ast2ArlContext.cpp
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
#include "Ast2ArlContext.h"
#include "zsp/parser/impl/TaskResolveSymbolPathRef.h"
#include "zsp/parser/impl/TaskGetName.h"
#include "zsp/fe/parser/impl/TaskIsTopDownScope.h"


namespace zsp {
namespace fe {
namespace parser {


Ast2ArlContext::Ast2ArlContext(
    dmgr::IDebugMgr                         *dmgr,
    zsp::parser::IFactory                   *factory,
    zsp::parser::IMarkerListener            *marker_l,
    arl::dm::IContext                       *ctxt,
    ast::ISymbolScope                       *root) :
        m_dmgr(dmgr), m_factory(factory), m_marker_l(marker_l),
        m_ctxt(ctxt), m_root(root), m_marker(
            factory->mkMarker(
                "", 
                zsp::parser::MarkerSeverityE::Error,
                {})) {
    DEBUG_INIT("Ast2ArlContext", dmgr); 
    m_type_s_idx = -1;
}

Ast2ArlContext::~Ast2ArlContext() {

}

void Ast2ArlContext::pushSymScope(ast::ISymbolScope *s) {
    DEBUG_ENTER("pushSymScope %s -> %d", 
        s->getName().c_str(),
        (m_scope_s.size())?m_scope_s.back().size()+1:1);

    if (!m_scope_s.size()) {
        m_scope_s.push_back({s});
    } else {
        m_scope_s.back().push_back(s);
    }

    if (TaskIsTopDownScope().check(s)) {
        m_type_s_idx = m_scope_s.back().size()-1;
        DEBUG("PUSH: m_type_s_idx=%d", m_type_s_idx);
    }
    DEBUG_LEAVE("pushSymScope");
}

void Ast2ArlContext::popSymScope() {
    DEBUG_ENTER("popSymScope %s -> %d", 
        m_scope_s.back().back()->getName().c_str(),
        (m_scope_s.size())?m_scope_s.back().size()-1:0);

    if (m_scope_s.back().size() == 0) {
        DEBUG("WARNING: size is now zero");
    }

    if (m_scope_s.size() && m_scope_s.back().size()) {
        if (m_type_s_idx == m_scope_s.back().size()-1) {
            m_type_s_idx--;
        }
        m_scope_s.back().pop_back();
//    } && TaskIsTopDownScope().check(m_scope_s.back().back())) {
//        m_type_s_idx = m_scope_s.back().size()-1;
        DEBUG("POP: m_type_s_idx=%d", m_type_s_idx);
    } else {
        m_type_s_idx = -1;
        DEBUG("POP: m_type_s_idx=%d", m_type_s_idx);
    }

    DEBUG_LEAVE("popSymScope");
}

void Ast2ArlContext::pushSymScopeStack(ast::ISymbolScope *s) {
    DEBUG_ENTER("pushSymScopeStack %s", (s)?s->getName().c_str():"<null>");
    if (!s) {
        s = m_scope_s.back().front();
        DEBUG("Full-push with root scope %s", s->getName().c_str());
    }
    DEBUG("s=%s", s->getName().c_str());
    m_scope_s.push_back({s});
    DEBUG_LEAVE("pushSymScopeStack");
}

void Ast2ArlContext::popSymScopeStack() {
    DEBUG_ENTER("popSymStack");
    m_scope_s.pop_back();
    DEBUG_LEAVE("popSymStack");
}

ast::IScopeChild *Ast2ArlContext::resolveRefPath(const ast::ISymbolRefPath *ref) {
    DEBUG_ENTER("resolveRefPath");
    for (std::vector<ast::SymbolRefPathElem>::const_iterator
        it=ref->getPath().begin();
        it!=ref->getPath().end(); it++) {
        DEBUG("IT: kind=%d idx=%d", it->kind, it->idx);
    }

    ast::IScopeChild *ret = zsp::parser::TaskResolveSymbolPathRef(
        m_factory->getDebugMgr(),
        m_scope_s.back().front()).resolve(ref);
    DEBUG_ENTER("resolveRefPath");
    return ret;
}

int32_t Ast2ArlContext::findBottomUpScope(ast::ISymbolScope *scope) {
    DEBUG_ENTER("findBottomUpScope %p", scope);
    int32_t ret = -1;
    if (m_type_s_idx != -1) {
        DEBUG("search: %d %d", m_scope_s.back().size(), m_type_s_idx);
        for (int32_t i=m_scope_s.back().size()-1; i>m_type_s_idx; i--) {
            if (m_scope_s.back().at(i) == scope) {
                DEBUG("Found @ %d", i);
                ret = m_scope_s.back().size()-i-1;
                break;
            }
        }
    }

    DEBUG_LEAVE("findBottomUpScope %d", ret);
    return ret;
}

ast::ISymbolScope *Ast2ArlContext::typeScope() const {
    DEBUG_ENTER("typeScope m_type_s_idx=%d size=%d", m_type_s_idx, m_scope_s.back().size());
    ast::ISymbolScope *ret = 
        (m_type_s_idx >= 0 && m_type_s_idx < m_scope_s.back().size())?
            m_scope_s.back().at(m_type_s_idx):0;
    DEBUG_LEAVE("typeScope %p", ret);
    return ret;
}

vsc::dm::IDataTypeStruct *Ast2ArlContext::findType(ast::IScopeChild *t) {
    std::map<ast::IScopeChild *, vsc::dm::IDataTypeStruct *>::const_iterator it;
    
    if ((it=m_type_m.find(t)) != m_type_m.end()) {
        return it->second;
    } else {
        return 0;
    }
}

void Ast2ArlContext::addType(ast::IScopeChild *t, vsc::dm::IDataTypeStruct *dmt) {
    m_type_m.insert({t, dmt});
}

vsc::dm::IDataTypeStruct *Ast2ArlContext::getType(ast::IScopeChild *t) {
    std::map<ast::IScopeChild *, vsc::dm::IDataTypeStruct *>::const_iterator it;
    vsc::dm::IDataTypeStruct *ret = 0;
    it = m_type_m.find(t);

    if (it == m_type_m.end()) {
        // Failed to find
        DEBUG("TODO: failed to find type %p (%s)", 
            t, zsp::parser::TaskGetName().get(t).c_str());
        for (it=m_type_m.begin(); it!=m_type_m.end(); it++) {
            DEBUG("  Type: %p %s", it->first, it->second->name().c_str());
        }
    } else {
        ret = it->second;
    }

    return ret;
}

std::string Ast2ArlContext::getQName(const std::string &name) {
    std::string qname;

    for (uint32_t i=2; i<m_scope_s.back().size(); i++) {
        if (i > 2) {
            qname += "::";
        }
        qname += m_scope_s.back().at(i)->getName();
    }
    if (m_scope_s.back().size() > 2) {
        qname += "::";
    }
    qname += name;

    return qname;
}

void Ast2ArlContext::pushIsPyRef(bool v) {
    m_pyref_s.push_back(v);
}

void Ast2ArlContext::setIsPyRef(bool v) {
    if (m_pyref_s.size()) {
        m_pyref_s.back() = v;
    } else if (v) {
        ERROR("Attempting to setIsPyRef==true with empty stack");
    }
}

bool Ast2ArlContext::isPyRef() {
    return (m_pyref_s.size() && m_pyref_s.back());
}

void Ast2ArlContext::popIsPyRef() {
    if (m_pyref_s.size()) {
        m_pyref_s.pop_back();
    }
}

void Ast2ArlContext::pushBaseExpr(vsc::dm::ITypeExpr *expr) {
    m_base_s.push_back(expr);
}
    
vsc::dm::ITypeExpr *Ast2ArlContext::baseExpr() {
    return (m_base_s.size())?m_base_s.back():0;
}

vsc::dm::ITypeExpr *Ast2ArlContext::popBaseExpr() {
    vsc::dm::ITypeExpr *ret = 0;

    if (m_base_s.size()) {
        ret = m_base_s.back();
        m_base_s.pop_back();
    }

    return ret;
}

dmgr::IDebug *Ast2ArlContext::m_dbg = 0;

}
}
}
