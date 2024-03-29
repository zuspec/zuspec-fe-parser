/**
 * TaskGetDataTypeAssocData.h
 *
 * Copyright 2023 Matthew Ballance and Contributors
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
#include "dmgr/IDebug.h"
#include "zsp/fe/parser/IAst2ArlContext.h"
#include "zsp/ast/impl/VisitorBase.h"

namespace zsp {
namespace fe {
namespace parser {



class TaskGetDataTypeAssocData : public virtual ast::VisitorBase {
public:
    TaskGetDataTypeAssocData(IAst2ArlContext *ctxt);

    virtual ~TaskGetDataTypeAssocData();

    ast::IAssocData *get(ast::IDataType *dt);

    ast::IAssocData *get(ast::IScopeChild *dt);

    virtual void visitSymbolTypeScope(ast::ISymbolTypeScope *i) override;

    virtual void visitTypeScope(ast::ITypeScope *i) override;

    virtual void visitDataTypeUserDefined(ast::IDataTypeUserDefined *i) override;

private:
    static dmgr::IDebug                 *m_dbg;
    IAst2ArlContext                     *m_ctxt;
    ast::IAssocData                     *m_ret;

};

}
}
}


