/**
 * IAst2ArlContext.h
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
#include <memory>
#include "dmgr/IDebugMgr.h"
#include "zsp/parser/IFactory.h"
#include "zsp/arl/dm/IContext.h"
#include "zsp/ast/IScopeChild.h"

namespace zsp {
namespace fe {
namespace parser {


class IAst2ArlContext;
using IAst2ArlContextUP=std::unique_ptr<IAst2ArlContext>;
class IAst2ArlContext {
public:

    virtual ~IAst2ArlContext() { }

    virtual dmgr::IDebugMgr *getDebugMgr() const = 0;

    virtual zsp::parser::IFactory *factory() const = 0;

    virtual arl::dm::IContext *ctxt() const = 0;

    virtual ast::ISymbolScope *symScope() const = 0;

    virtual const std::vector<ast::ISymbolScope *> &symScopes() const = 0;

    virtual void pushSymScope(ast::ISymbolScope *s) = 0;

    virtual void popSymScope() = 0;

    virtual vsc::dm::IDataTypeStruct *findType(ast::IScopeChild *t) = 0;

    virtual void addType(ast::IScopeChild *t, vsc::dm::IDataTypeStruct *dmt) = 0;

};

} /* namespace parser */
} /* namespace fe */
} /* namespace zsp */

