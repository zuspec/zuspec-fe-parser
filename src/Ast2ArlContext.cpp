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


namespace zsp {
namespace fe {
namespace parser {


Ast2ArlContext::Ast2ArlContext(
    dmgr::IDebugMgr                         *dmgr,
    zsp::parser::IFactory                   *factory,
    zsp::parser::IMarkerListener            *marker_l,
    arl::dm::IContext                       *ctxt) :
        m_dmgr(dmgr), m_factory(factory), m_marker_l(marker_l),
        m_ctxt(ctxt), m_marker(
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
    if (dynamic_cast<ast::ISymbolTypeScope *>(s) || dynamic_cast<ast::ISymbolFunctionScope *>(s)) {
        m_type_s_idx = m_scope_s.size();
        DEBUG("PUSH: m_type_s_idx=%d", m_type_s_idx);
    }

    m_scope_s.push_back(s);
}

void Ast2ArlContext::popSymScope() {
    m_scope_s.pop_back();

    if (m_scope_s.size() && (
            dynamic_cast<ast::ISymbolTypeScope *>(m_scope_s.back()) ||
            dynamic_cast<ast::ISymbolFunctionScope *>(m_scope_s.back()))) {
        m_type_s_idx = m_scope_s.size()-1;
        DEBUG("POP: m_type_s_idx=%d", m_type_s_idx);
    } else {
        m_type_s_idx = -1;
        DEBUG("POP: m_type_s_idx=%d", m_type_s_idx);
    }
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
        m_scope_s.front()).resolve(ref);
    DEBUG_ENTER("resolveRefPath");
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
        DEBUG("TODO: failed to find type %p", t);
        for (it=m_type_m.begin(); it!=m_type_m.end(); it++) {
            DEBUG("  Type: %p %s", it->first, it->second->name().c_str());
        }
    } else {
        ret = it->second;
    }

    return ret;
}

dmgr::IDebug *Ast2ArlContext::m_dbg = 0;

}
}
}
