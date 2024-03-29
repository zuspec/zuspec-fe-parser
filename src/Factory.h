/**
 * Factory.h
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
#include "zsp/fe/parser/IFactory.h"
#include "zsp/fe/parser/IAst2ArlBuilder.h"

namespace zsp {
namespace fe {
namespace parser {

class Factory;
using FactoryUP=std::unique_ptr<Factory>;
class Factory : public virtual IFactory {
public:
    Factory();

    virtual ~Factory();

    virtual void init(
        dmgr::IDebugMgr         *dmgr,
        zsp::parser::IFactory   *factory) override;

    virtual dmgr::IDebugMgr *getDebugMgr() override;

    virtual IAst2ArlBuilder *mkAst2ArlBuilder() override;

    virtual IAst2ArlContext *mkAst2ArlContext(
        arl::dm::IContext               *ctxt,
        ast::ISymbolScope               *root,
        zsp::parser::IMarkerListener    *marker_l) override;

    static Factory *inst();

private:
    static FactoryUP            m_inst;
    dmgr::IDebugMgr             *m_dmgr;
    zsp::parser::IFactory       *m_factory;

};

}
}
}


