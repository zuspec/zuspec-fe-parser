/**
 * TaskBuildDataTypeFunction.h
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
#include "zsp/fe/parser/IAst2ArlContext.h"
#include "zsp/arl/dm/IDataTypeArlStruct.h"

namespace zsp {
namespace fe {
namespace parser {



class TaskBuildDataTypeFunction {
public:
    TaskBuildDataTypeFunction(
        IAst2ArlContext             *ctxt, 
        arl::dm::IDataTypeArlStruct *type=0);

    virtual ~TaskBuildDataTypeFunction();

    virtual zsp::arl::dm::IDataTypeFunction *build(
        ast::ISymbolFunctionScope   *func,
        bool                        ctxt_f=false);

private:
    static dmgr::IDebug                 *m_dbg;
    IAst2ArlContext                     *m_ctxt;
    arl::dm::IDataTypeArlStruct         *m_type;


};

}
}
}


