/*
 * Factory.cpp
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
#include "Factory.h"
#include "zsp/fe/parser/FactoryExt.h"
#include "Ast2ArlBuilder.h"
#include "Ast2ArlContext.h"


namespace zsp {
namespace fe {
namespace parser {


Factory::Factory() : m_dmgr(0), m_factory(0) {

}

Factory::~Factory() {

}

void Factory::init(
    dmgr::IDebugMgr         *dmgr,
    zsp::parser::IFactory   *factory) {
    m_dmgr = dmgr;
    m_factory = factory;
}

dmgr::IDebugMgr *Factory::getDebugMgr() {
    return m_dmgr;
}

IAst2ArlBuilder *Factory::mkAst2ArlBuilder() {
    return new Ast2ArlBuilder(m_dmgr, m_factory);
}

IAst2ArlContext *Factory::mkAst2ArlContext(
    arl::dm::IContext               *ctxt,
    ast::ISymbolScope               *root,
    zsp::parser::IMarkerListener    *marker_l) {
    return new Ast2ArlContext(
        m_dmgr,
        m_factory,
        marker_l,
        ctxt,
        root);
}

Factory *Factory::inst() {
    if (!m_inst) {
        m_inst = FactoryUP(new Factory());
    }
    return m_inst.get();
}

FactoryUP Factory::m_inst;


}
}
}

extern "C" zsp::fe::parser::IFactory *zsp_fe_parser_getFactory() {
    return zsp::fe::parser::Factory::inst();
}