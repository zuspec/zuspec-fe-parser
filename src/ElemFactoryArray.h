/**
 * ElemFactoryArray.h
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
#include "dmgr/IDebugMgr.h"
#include "zsp/fe/parser/impl/ElemFactoryAssocDataBase.h"

namespace zsp {
namespace fe {
namespace parser {


class ElemFactoryArray : public virtual ElemFactoryAssocDataBase {
public:
    ElemFactoryArray(dmgr::IDebugMgr *dmgr);

    virtual ~ElemFactoryArray();

    virtual vsc::dm::IDataType *mkDataType(
        IAst2ArlContext         *ctx,
        const std::string       &name,
        ast::IScopeChild        *type) override;

    virtual vsc::dm::ITypeField *mkTypeFieldPhy(
        IAst2ArlContext         *ctx,
        const std::string       &name,
        ast::IScopeChild        *type,
        vsc::dm::TypeFieldAttr  attr,
        const vsc::dm::ValRef   &init) override;

    static ElemFactoryArray *create(dmgr::IDebugMgr *dmgr) {
        return new ElemFactoryArray(dmgr);
    }

private:
    static dmgr::IDebug             *m_dbg;

};


}
}
}

