/*
 * TestBlogExamples.cpp
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
#include "TestBlogExamples.h"


namespace zsp {
namespace fe {
namespace parser {

using namespace zsp::parser;

TestBlogExamples::TestBlogExamples() {

}

TestBlogExamples::~TestBlogExamples() {

}

TEST_F(TestBlogExamples, memtest_1) {
    const char *content = R"(
import addr_reg_pkg::*;
import executor_pkg::*;

struct core_s : executor_trait_s {
    rand bit[8]     id;
}

component memtest_c {
    addr_handle_t       base_addr;

    action Write {
        rand executor_claim_s<core_s> core;
        rand bit[64] in [0..0xFFFFFF] offset;
        rand bit[32] in [1..256]      words;

        exec body {
            repeat (i : words) {
                write32(
                    make_handle_from_handle(comp.base_addr, 4*(offset+i)),
                    i+1);
            }
        }
    }

    action Copy {
        rand executor_claim_s<core_s> core;
        rand bit[64] in [0..0xFFFFFF] src;
        rand bit[64] in [0..0xFFFFFF] dst;
        rand bit[32] in [1..256]      words;

        exec body {
            bit[32] tmp;
            repeat (i : words) {
                tmp = read32(
                    make_handle_from_handle(comp.base_addr,
                        4*(src+i)));
                write32(
                    make_handle_from_handle(comp.base_addr,
                        4*(dst+i)),
                    tmp);
            }
        }
    }

    action Check {
        rand executor_claim_s<core_s> core;
        rand bit[64] in [0..0xFFFFFF] offset;
        rand bit[32] in [1..256]      words;

        exec body {
            bit[32] tmp;
            repeat (i : words) {
                tmp = read32(
                    make_handle_from_handle(comp.base_addr,
                        4*(offset+i)));
                if (tmp != i+1) {
                    error("0x%08x: expect %d ; receive %d",
                        4*(offset+i), i+1, tmp);
                }
            }
        }
    }

    action WriteCopyCheck {
        Write             write;
        Copy              copy;
        Check             check;

        activity {
            write;
            copy;
            check;
        }

        constraint {
            // Copy reads from same location that Write populated
            copy.src == write.offset;
            // Check reads from the same location that Copy populated
            copy.dst == check.offset;
            // All actions write the same number of words
            copy.words == write.words;
            copy.words == check.words;

            // Ensure that src/dst regions do not overlap
            (copy.src+(4*copy.words) < copy.dst) ||
            (copy.src > copy.dst+(4*copy.words));
        }
    }

}

component pss_top {
    executor_c<core_s>         core[4];
    executor_group_c<core_s>   cores;
    transparent_addr_space_c<> aspace;
    memtest_c                  memtest;

    exec init {
        foreach (core[i]) {
            core[i].trait.id = i;
            cores.add_executor(core[i]);
        }

        // Define a memory region
        transparent_addr_region_s<> region;
        region.addr = 0x8000_0000;
        region.size = 0x1000_0000;
        memtest.base_addr = aspace.add_region(region);
    }

    action Copy_0_1_0 {
        activity {
            do memtest_c::WriteCopyCheck with {
                write.core.trait.id == 0;
                copy.core.trait.id == 1;
                check.core.trait.id == 0;
            }
        }
    }

    action Copy_check_diff_core {
        activity {
            do memtest_c::WriteCopyCheck with {
                write.core.trait.id != check.core.trait.id;
            }
        }
    }

    action Copy_same_core {
        activity {
            do memtest_c::WriteCopyCheck with {
                write.core.trait.id == copy.core.trait.id;
                copy.core.trait.id == check.core.trait.id;
            }
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
    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::A"));
    // ASSERT_TRUE(m_ctxt->findDataTypeAction("pss_top::B"));
    // ASSERT_EQ(m_ctxt->findDataTypeComponent("pss_top")->getActionTypes().size(), 2);

    dumpJSON({m_ctxt->findDataTypeComponent("pss_top")});
}

}
}
}
