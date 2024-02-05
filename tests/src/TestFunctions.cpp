/*
 * TestFunctions.cpp
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
#include "TestFunctions.h"


namespace zsp {
namespace fe {
namespace parser {

using namespace zsp::parser;


TestFunctions::TestFunctions() {

}

TestFunctions::~TestFunctions() {

}

TEST_F(TestFunctions, smoke) {
    const char *content = R"(
        function void doit(int a, int b) {
            int c;
            c = 20;
        }
    )";

    enableDebug(true);

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

    ASSERT_TRUE(m_ctxt->findDataTypeFunction("doit"));
    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::A"));
    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::B"));
    // ASSERT_EQ(m_ctxt->findDataTypeComponent("pss_top")->getActionTypes().size(), 2);

    dumpJSON({m_ctxt->findDataTypeFunction("doit")});
}

TEST_F(TestFunctions, param_ref) {
    const char *content = R"(
        function void doit(int a, int b) {
            int c;
            c = b;
        }
    )";

    enableDebug(true);

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

    ASSERT_TRUE(m_ctxt->findDataTypeFunction("doit"));
    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::A"));
    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::B"));
    // ASSERT_EQ(m_ctxt->findDataTypeComponent("pss_top")->getActionTypes().size(), 2);

    dumpJSON({m_ctxt->findDataTypeFunction("doit")});
}

TEST_F(TestFunctions, context_func_param_ref) {
    const char *content = R"(
        component my_reg_group {
            function bit[64] get_offset(string name, int index) {
                return (index*32);
            }
        }
    )";

    enableDebug(true);

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

    ASSERT_TRUE(m_ctxt->findDataTypeFunction("my_reg_group::get_offset"));
    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::A"));
    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::B"));
    // ASSERT_EQ(m_ctxt->findDataTypeComponent("pss_top")->getActionTypes().size(), 2);

//    dumpJSON({m_ctxt->findDataTypeFunction("doit")});
}

TEST_F(TestFunctions, global_func_call) {
    const char *content = R"(
        function void doit_s(int a) {
            
        }

        function void doit(int a, int b) {
            int c;
            c = b;
            doit_s(c+1);
        }
    )";

    enableDebug(true);

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

    ASSERT_TRUE(m_ctxt->findDataTypeFunction("doit"));
    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::A"));
    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::B"));
    // ASSERT_EQ(m_ctxt->findDataTypeComponent("pss_top")->getActionTypes().size(), 2);

    dumpJSON({m_ctxt->findDataTypeFunction("doit")});
}

TEST_F(TestFunctions, call_global_pass_typescope_context) {
    const char *content = R"(
        function void doit(int a) {
            
        }
        
        component pss_top {
            action A {
                int val;

                exec body {
                    doit(val);
                }
            }
        }
    )";

    enableDebug(true);

    IMarkerCollectorUP marker_c(m_zsp_factory->mkMarkerCollector());
    std::vector<ast::IGlobalScopeUP> files;
    files.push_back(ast::IGlobalScopeUP(parse(
        marker_c.get(),
        content,
        "smoke.pss"
    )));

    checkNoErrors("Parse", marker_c.get());

    ast::ISymbolScopeUP root(link(
        marker_c.get(),
        files
    ));

    checkNoErrors("Link", marker_c.get());

    ast2Arl(
        marker_c.get(),
        root.get(),
        m_ctxt.get()
    );

    checkNoErrors("Ast2Arl", marker_c.get());

    ASSERT_TRUE(m_ctxt->findDataTypeFunction("doit"));
    ASSERT_TRUE(m_ctxt->findDataTypeComponent("pss_top"));
    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::A"));
    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::B"));
    // ASSERT_EQ(m_ctxt->findDataTypeComponent("pss_top")->getActionTypes().size(), 2);

    dumpJSON({
        m_ctxt->findDataTypeFunction("doit"),
        m_ctxt->findDataTypeComponent("pss_top")});
}

}
}
}
