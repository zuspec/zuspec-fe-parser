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
}

Ast2ArlContext::~Ast2ArlContext() {

}

void Ast2ArlContext::pushSymScope(ast::IScopeChild *s) {
    DEBUG_ENTER("pushSymScope %s -> %d (%p)", 
        zsp::parser::ScopeUtil(s).getName().c_str(),
        (m_scope_s.size())?m_scope_s.back().size()+1:1,
        s);

    if (!m_scope_s.size()) {
        m_scope_s.push_back({zsp::parser::ScopeUtil(s)});
        m_type_s_idx_s.push_back(-1);
    } else {
        m_scope_s.back().push_back(s);
    }

    if (DEBUG_EN) {
        for (int32_t i=m_scope_s.back().size()-1; i>0; i--) {
            DEBUG("  Scope[%d] %p (%s)", i, 
                m_scope_s.back().at(i).get(),
                m_scope_s.back().at(i).getName().c_str());
        }
    }

    if (TaskIsTopDownScope().check(s)) {
        m_type_s_idx_s.back() = m_scope_s.back().size()-1;
        DEBUG("PUSH: m_type_s_idx=%d", m_type_s_idx_s.back());
    }
    DEBUG("m_scope_s.size: %d m_type_s_idx: %d", 
        m_scope_s.back().size(), m_type_s_idx_s.back());
    DEBUG_LEAVE("pushSymScope");
}

void Ast2ArlContext::popSymScope() {
    DEBUG_ENTER("popSymScope %s -> %d", 
        (m_scope_s.back().size())?m_scope_s.back().back().getName().c_str():"<empty>",
        (m_scope_s.size())?m_scope_s.back().size()-1:0);

    if (m_scope_s.back().size() == 0) {
        DEBUG("WARNING: size is now zero");
    }

    if (m_scope_s.size() && m_scope_s.back().size()) {
        if (m_type_s_idx_s.back() == m_scope_s.back().size()-1) {
            m_type_s_idx_s.back()--;
        }
        m_scope_s.back().pop_back();
//    } && TaskIsTopDownScope().check(m_scope_s.back().back())) {
//        m_type_s_idx = m_scope_s.back().size()-1;
        DEBUG("POP: m_type_s_idx=%d", m_type_s_idx_s.back());
    } else {
        m_type_s_idx_s.back() = -1;
        DEBUG("POP: m_type_s_idx=%d", m_type_s_idx_s.back());
    }

    DEBUG("m_scope_s.size: %d m_type_s_idx: %d", 
        m_scope_s.back().size(), m_type_s_idx_s.back());

    DEBUG_LEAVE("popSymScope");
}

void Ast2ArlContext::pushSymScopeStack(ast::ISymbolChildrenScope *s) {
    DEBUG_ENTER("pushSymScopeStack %s", (s)?s->getName().c_str():"<null>");
    if (s) {
        std::vector<zsp::parser::ScopeUtil> elems;
        ast::ISymbolChildrenScope *ss = s;
        while (ss) {
            elems.push_back(zsp::parser::ScopeUtil(ss));
            ss = ss->getUpper();
        }
        m_scope_s.push_back(std::vector<zsp::parser::ScopeUtil>());
        for (std::vector<zsp::parser::ScopeUtil>::const_reverse_iterator
            it=elems.rbegin();
            it!=elems.rend(); it++) {
            zsp::parser::ScopeUtil &util = const_cast<zsp::parser::ScopeUtil &>(*it);
            DEBUG("Push scope %s", util.getName().c_str());
            m_scope_s.back().push_back(*it);
        }
        m_type_s_idx_s.push_back(m_scope_s.back().size()-1);
        DEBUG("Full-push with root scope %s", elems.front().getName().c_str());
    } else {
        m_scope_s.push_back({m_scope_s.back().front()});
        m_type_s_idx_s.push_back(-1);
        DEBUG("Full-push with cloned stack");
    }
    DEBUG_LEAVE("pushSymScopeStack");
}

void Ast2ArlContext::popSymScopeStack() {
    DEBUG_ENTER("popSymStack");
    m_scope_s.pop_back();
    m_type_s_idx_s.pop_back();
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
        m_scope_s.back().front().getT<ast::ISymbolScope>()).resolve(ref);
    DEBUG_ENTER("resolveRefPath");
    return ret;
}

int32_t Ast2ArlContext::findBottomUpScope(ast::IScopeChild *scope) {
    DEBUG_ENTER("findBottomUpScope %p", scope);
    int32_t ret = -1;
    if (m_type_s_idx_s.back() != -1) {
        DEBUG("search: %d %d", m_scope_s.back().size(), m_type_s_idx_s.back());
        for (int32_t i=m_scope_s.back().size()-1; i>m_type_s_idx_s.back(); i--) {
            DEBUG("  Scope[%d] %p (%s)", i, 
            m_scope_s.back().at(i).get(),
            m_scope_s.back().at(i).getName().c_str());
            if (m_scope_s.back().at(i).get() == scope) {
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
    DEBUG_ENTER("typeScope m_type_s_idx=%d size=%d", 
        m_type_s_idx_s.back(), m_scope_s.back().size());
    ast::ISymbolScope *ret = 0;

    if (m_type_s_idx_s.back() >= 0 && m_type_s_idx_s.back() < m_scope_s.back().size()) {
        ret = m_scope_s.back().at(m_type_s_idx_s.back()).getT<ast::ISymbolScope>();
    }

    DEBUG_LEAVE("typeScope %p", ret);
    return ret;
}

vsc::dm::IAccept *Ast2ArlContext::findType(ast::IScopeChild *t) {
    std::map<ast::IScopeChild *, vsc::dm::IAccept *>::const_iterator it;
    
    if ((it=m_type_m.find(t)) != m_type_m.end()) {
        return dynamic_cast<vsc::dm::IDataType *>(it->second);
    } else {
        return 0;
    }
}

void Ast2ArlContext::addType(ast::IScopeChild *t, vsc::dm::IAccept *dmt) {
    m_type_m.insert({t, dmt});
}

vsc::dm::IAccept *Ast2ArlContext::getType(ast::IScopeChild *t) {
    std::map<ast::IScopeChild *, vsc::dm::IAccept *>::const_iterator it;
    vsc::dm::IAccept *ret = 0;
    it = m_type_m.find(t);

    if (it == m_type_m.end()) {
        // Failed to find
        DEBUG("TODO: failed to find type %p (%s)", 
            t, zsp::parser::TaskGetName().get(t).c_str());
        for (it=m_type_m.begin(); it!=m_type_m.end(); it++) {
            DEBUG("  Type: %p %s", it->first, 
                dynamic_cast<vsc::dm::IDataTypeStruct *>(it->second)?
                    dynamic_cast<vsc::dm::IDataTypeStruct *>(it->second)->name().c_str():"<primitive>");
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
        qname += m_scope_s.back().at(i).getName();
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
        DEBUG_ERROR("Attempting to setIsPyRef==true with empty stack");
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
