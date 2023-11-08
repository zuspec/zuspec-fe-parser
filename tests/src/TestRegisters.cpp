/*
 * TestRegisters.cpp
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
#include "TestRegisters.h"

using namespace zsp::parser;

namespace zsp {
namespace fe {
namespace parser {


TestRegisters::TestRegisters() {

}

TestRegisters::~TestRegisters() {

}

TEST_F(TestRegisters, field_ctor) {
    const char *content = R"(
        import addr_reg_pkg::*;
        /*
        struct MyReg : packed_s<> {
            bit[16]  v1;
            bit[16]  v2;
        }
         */

        component MyRegs : reg_group_c {
//            reg_c<MyReg>        my_reg1;
//            reg_c<MyReg>        my_reg2;
            reg_c<bit[32]>      my_reg3;
            reg_c<bit[64]>      my_reg4;
        }
        component pss_top {
            MyRegs          regs;

            action Entry {
                exec body {
//                    write32(0, 0);
//                    comp.regs.my_reg1.write_val(2);
                    comp.regs.my_reg3.write_val(2);
                    comp.regs.my_reg4.write_val(2+3);
                }
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
    for (std::vector<IMarkerUP>::const_iterator
        it=marker_c->markers().begin();
        it!=marker_c->markers().end(); it++) {
        fprintf(stdout, "Marker: %s\n", (*it)->msg().c_str());
    }

    ASSERT_FALSE(marker_c->hasSeverity(MarkerSeverityE::Error));
    
    enableDebug(true);

    ast::ISymbolScopeUP root(link(
        marker_c.get(),
        files
    ));
    for (std::vector<IMarkerUP>::const_iterator
        it=marker_c->markers().begin();
        it!=marker_c->markers().end(); it++) {
        fprintf(stdout, "Marker: %s\n", (*it)->msg().c_str());
    }

    ASSERT_FALSE(marker_c->hasSeverity(MarkerSeverityE::Error));


    ast2Arl(
        marker_c.get(),
        root.get(),
        m_ctxt.get()
    );

    ASSERT_FALSE(marker_c->hasSeverity(MarkerSeverityE::Error));

    ASSERT_TRUE(m_ctxt->findDataTypeComponent("pss_top"));
    ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::Entry"));
    ASSERT_EQ(m_ctxt->findDataTypeComponent("pss_top")->getActionTypes().size(), 1);

    dumpJSON({m_ctxt->findDataTypeComponent("pss_top")});
}

}
}
}
