/*
 * TestTypeTemplate.cpp
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
#include "TestTypeTemplate.h"


namespace zsp {
namespace fe {
namespace parser {

using namespace zsp::parser;

TestTypeTemplate::TestTypeTemplate() {

}

TestTypeTemplate::~TestTypeTemplate() {

}

TEST_F(TestTypeTemplate, smoke) {
    const char *content = R"(
        package p1 {
        struct Base<type T> {
            rand T ba;
        }
        struct Ext {
            Base<int>   ea;
            Base<bit>   eb;
        }
        }
    )";

   enableDebug(true);
   setLoadStdLib(true);

    IMarkerCollectorUP marker_c(m_zsp_factory->mkMarkerCollector());
    std::vector<ast::IGlobalScopeUP> files;
    files.push_back(ast::IGlobalScopeUP(parse(
        marker_c.get(),
        content,
        "smoke.pss"
    )));

    checkNoErrors("syntax parsing", marker_c.get());
    ASSERT_FALSE(marker_c->hasSeverity(zsp::parser::MarkerSeverityE::Error));

    ast::ISymbolScopeUP root(link(
        marker_c.get(),
        files
    ));

    checkNoErrors("linking", marker_c.get());
    ASSERT_FALSE(marker_c->hasSeverity(zsp::parser::MarkerSeverityE::Error));

    ast2Arl(
        marker_c.get(),
        root.get(),
        m_ctxt.get()
    );

    checkNoErrors("ast2arl", marker_c.get());
    ASSERT_FALSE(marker_c->hasSeverity(zsp::parser::MarkerSeverityE::Error));

/*
    vsc::dm::IDataTypeStruct *Base;
    ASSERT_TRUE((Base=m_ctxt->findDataTypeStruct("Base")));
    ASSERT_EQ(Base->getFields().size(), 1);
 */

    vsc::dm::IDataTypeStruct *Ext;
    ASSERT_TRUE((Ext=m_ctxt->findDataTypeStruct("p1::Ext")));
    ASSERT_EQ(Ext->getFields().size(), 2);

    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::A"));
    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::B"));
    // ASSERT_EQ(m_ctxt->findDataTypeComponent("pss_top")->getActionTypes().size(), 2);

    dumpJSON({
        Ext
        });
}

TEST_F(TestTypeTemplate, struct_type_param) {
    const char *content = R"(
      package p1 {
        struct X { }

        struct S { }

        struct Y { }

        struct Base<type T=S> {
            rand T ba;
        }

        struct Ext {
            Base<>      ea;
            Base<S>     eb;
            Base<bit>   ec;
        }
      }
    )";

   enableDebug(true);
   setLoadStdLib(false);

    IMarkerCollectorUP marker_c(m_zsp_factory->mkMarkerCollector());
    std::vector<ast::IGlobalScopeUP> files;
    files.push_back(ast::IGlobalScopeUP(parse(
        marker_c.get(),
        content,
        "smoke.pss"
    )));

    checkNoErrors("syntax parsing", marker_c.get());
    ASSERT_FALSE(marker_c->hasSeverity(zsp::parser::MarkerSeverityE::Error));

    ast::ISymbolScopeUP root(link(
        marker_c.get(),
        files
    ));

    checkNoErrors("linking", marker_c.get());
    ASSERT_FALSE(marker_c->hasSeverity(zsp::parser::MarkerSeverityE::Error));

    ast2Arl(
        marker_c.get(),
        root.get(),
        m_ctxt.get()
    );

    checkNoErrors("ast2arl", marker_c.get());
    ASSERT_FALSE(marker_c->hasSeverity(zsp::parser::MarkerSeverityE::Error));

/*
    vsc::dm::IDataTypeStruct *Base;
    ASSERT_TRUE((Base=m_ctxt->findDataTypeStruct("Base")));
    ASSERT_EQ(Base->getFields().size(), 1);
 */

    vsc::dm::IDataTypeStruct *Ext;
    ASSERT_TRUE((Ext=m_ctxt->findDataTypeStruct("p1::Ext")));
    ASSERT_EQ(Ext->getFields().size(), 3);

    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::A"));
    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::B"));
    // ASSERT_EQ(m_ctxt->findDataTypeComponent("pss_top")->getActionTypes().size(), 2);

    dumpJSON({
        Ext
        });
}

TEST_F(TestTypeTemplate, contiguous_addr_space) {
    const char *content = R"(
        package addr_reg_pkg {
//            import executor_pkg::* ;

            component addr_space_base_c {};

            struct addr_trait_s {};

            struct empty_addr_trait_s : addr_trait_s {};

//    typedef chandle addr_handle_t;
            struct addr_handle_t { }

            component contiguous_addr_space_c<
                struct TRAIT : addr_trait_s = empty_addr_trait_s> /*: addr_space_base_c*/ {

                function addr_handle_t add_region(addr_region_s <TRAIT> r);
                function addr_handle_t add_nonallocatable_region(addr_region_s <> r);
                bool byte_addressable = true;
            };

            struct test_s {
                contiguous_addr_space_c<> aspace;
            }
        }
    )";

   enableDebug(true);
   setLoadStdLib(false);

    IMarkerCollectorUP marker_c(m_zsp_factory->mkMarkerCollector());
    std::vector<ast::IGlobalScopeUP> files;
    files.push_back(ast::IGlobalScopeUP(parse(
        marker_c.get(),
        content,
        "smoke.pss"
    )));

    checkNoErrors("syntax parsing", marker_c.get());
    ASSERT_FALSE(marker_c->hasSeverity(zsp::parser::MarkerSeverityE::Error));

    ast::ISymbolScopeUP root(link(
        marker_c.get(),
        files
    ));

    checkNoErrors("linking", marker_c.get());
    ASSERT_FALSE(marker_c->hasSeverity(zsp::parser::MarkerSeverityE::Error));

    ast2Arl(
        marker_c.get(),
        root.get(),
        m_ctxt.get()
    );

    checkNoErrors("ast2arl", marker_c.get());
    ASSERT_FALSE(marker_c->hasSeverity(zsp::parser::MarkerSeverityE::Error));

/*
    vsc::dm::IDataTypeStruct *Base;
    ASSERT_TRUE((Base=m_ctxt->findDataTypeStruct("Base")));
    ASSERT_EQ(Base->getFields().size(), 1);
 */

    vsc::dm::IDataTypeStruct *Ext;
    ASSERT_TRUE((Ext=m_ctxt->findDataTypeStruct("addr_reg_pkg::test_s")));
    ASSERT_EQ(Ext->getFields().size(), 1);

    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::A"));
    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::B"));
    // ASSERT_EQ(m_ctxt->findDataTypeComponent("pss_top")->getActionTypes().size(), 2);

    dumpJSON({
        Ext
        });
}

}
}
}
