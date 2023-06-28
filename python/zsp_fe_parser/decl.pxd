#
# decl.pxd
#
# Copyright 2022 Matthew Ballance and Contributors
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may 
# not use this file except in compliance with the License.  
# You may obtain a copy of the License at:
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software 
# distributed under the License is distributed on an "AS IS" BASIS, 
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  
# See the License for the specific language governing permissions and 
# limitations under the License.
#
# Created on:
#     Author: 
#

from zsp_arl_dm cimport decl as arl_dm
from debug_mgr cimport decl as dm
cimport zsp_parser.decl as zspp
cimport zsp_parser.ast_decl as ast_decl

ctypedef IFactory *IFactoryP

cdef extern from "zsp/fe/parser/IFactory.h" namespace "zsp::fe::parser":
    cdef cppclass IFactory:

        void init(dm.IDebugMgr *dmgr, zspp.IFactory *factory)

        dm.IDebugMgr *getDebugMgr()

        IAst2ArlBuilder *mkAst2ArlBuilder()

        IAst2ArlContext *mkAst2ArlContext(
            arl_dm.IContext         *ctxt,
            ast_decl.ISymbolScope   *root,
            zspp.IMarkerListener    *marker_l)

cdef extern from "zsp/fe/parser/IAst2ArlBuilder.h" namespace "zsp::fe::parser":
    cdef cppclass IAst2ArlBuilder:

        void build(
            ast_decl.ISymbolScope       *root,
            IAst2ArlContext             *ctxt)

cdef extern from "zsp/fe/parser/IAst2ArlContext.h" namespace "zsp::fe::parser":
    cdef cppclass IAst2ArlContext:
        pass

