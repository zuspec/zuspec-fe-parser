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
#include "Ast2ArlBuilder.h"


namespace zsp {
namespace fe {
namespace parser {


Factory::Factory() : m_factory(0) {

}

Factory::~Factory() {

}

void Factory::init(zsp::IFactory *factory) {
    m_factory = factory;
}

IAst2ArlBuilder *Factory::mkAst2ArlBuilder() {
    return new Ast2ArlBuilder(m_factory);
}

Factory *Factory::inst() {
    if (!m_inst) {
        m_inst = FactoryUP(new Factory());
    }
    return m_inst.get();
}

FactoryUP Factory::m_inst;

extern "C" IFactory *zsp_fe_parser_getFactory() {
    return Factory::inst();
}

}
}
}
