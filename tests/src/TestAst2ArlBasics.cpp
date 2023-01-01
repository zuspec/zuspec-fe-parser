/*
 * TestAst2ArlBasics.cpp
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
#include "TestAst2ArlBasics.h"

using namespace zsp::parser;

namespace zsp {
namespace fe {
namespace parser {


TestAst2ArlBasics::TestAst2ArlBasics() {

}

TestAst2ArlBasics::~TestAst2ArlBasics() {

}

TEST_F(TestAst2ArlBasics, smoke) {
    const char *content = R"(
        component pss_top {
            action A {

            }
        }
    )";

    IMarkerCollectorUP marker_c(m_zsp_factory->mkMarkerCollector());
    std::vector<ast::IGlobalScopeUP> files;
    files.push_back(ast::IGlobalScopeUP(parse(
        marker_c.get(),
        content,
        "smoke.pss"
    )));

    ASSERT_FALSE(marker_c->hasSeverity(MarkerSeverityE::Error));

    ast::ISymbolScopeUP root(link(
        marker_c.get(),
        files
    ));

    ASSERT_FALSE(marker_c->hasSeverity(MarkerSeverityE::Error));

    ast2Arl(
        marker_c.get(),
        root.get(),
        m_ctxt.get()
    );

    ASSERT_FALSE(marker_c->hasSeverity(MarkerSeverityE::Error));

    ASSERT_TRUE(m_ctxt->findDataTypeComponent("pss_top"));
    ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::A"));

}

TEST_F(TestAst2ArlBasics, smoke_ext_comp) {
    const char *content = R"(
        component pss_top {
            action A {

            }
        }

        extend component pss_top {
            action B {

            }
        }
    )";

    IMarkerCollectorUP marker_c(m_zsp_factory->mkMarkerCollector());
    std::vector<ast::IGlobalScopeUP> files;
    files.push_back(ast::IGlobalScopeUP(parse(
        marker_c.get(),
        content,
        "smoke.pss"
    )));

    ASSERT_FALSE(marker_c->hasSeverity(MarkerSeverityE::Error));

    ast::ISymbolScopeUP root(link(
        marker_c.get(),
        files
    ));

    ASSERT_FALSE(marker_c->hasSeverity(MarkerSeverityE::Error));

    ast2Arl(
        marker_c.get(),
        root.get(),
        m_ctxt.get()
    );

    ASSERT_FALSE(marker_c->hasSeverity(MarkerSeverityE::Error));

    ASSERT_TRUE(m_ctxt->findDataTypeComponent("pss_top"));
    ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::A"));
    ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::B"));

}

}
}
}
