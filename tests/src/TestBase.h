/**
 * TestBase.h
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
#include "gtest/gtest.h"
#include "arl/IContext.h"
#include "zsp/IFactory.h"
#include "zsp/ast/IFactory.h"
#include "zsp/fe/parser/IFactory.h"

namespace zsp {
namespace fe {
namespace parser {



class TestBase : public ::testing::Test {
public:
    TestBase();

    virtual ~TestBase();

    virtual void SetUp() override;

    virtual void TearDown() override;

    ast::IGlobalScope *parse(
        IMarkerListener         *marker_l,
        const std::string       &content,
        const std::string       &name);

    ast::ISymbolScope *link(
        IMarkerListener                         *marker_l,
        const std::vector<ast::IGlobalScopeUP>  &files);

    void ast2Arl(
        IMarkerListener                         *marker_l,
        ast::ISymbolScope                       *root,
        arl::IContext                           *ctxt);

protected:
    IFactory                    *m_factory;
    zsp::IFactory               *m_zsp_factory;
    arl::IContextUP             m_ctxt;

};

}
}
}


