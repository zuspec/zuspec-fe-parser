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
#include "Ast2ArlContext.h"


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

}

Ast2ArlContext::~Ast2ArlContext() {

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

}
}
}
