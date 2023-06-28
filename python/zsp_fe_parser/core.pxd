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
cimport zsp_arl_dm.core as arl_dm
from zsp_fe_parser cimport decl
from debug_mgr cimport core as dm
cimport zsp_parser.ast as ast
cimport zsp_parser.core as zspp
from libcpp cimport bool

cdef class Factory:
    cdef decl.IFactory      *_hndl

#    void init(dm.IDebugMgr *dmgr, zspp.IFactory *factory)

    cpdef dm.DebugMgr getDebugMgr(self)

    cpdef Ast2ArlBuilder mkAst2ArlBuilder(self)

    cpdef Ast2ArlContext mkAst2ArlContext(
        self,
        arl_dm.Context         ctxt,
        ast.SymbolScope        root,
        zspp.MarkerListener    marker_l)

cdef class Ast2ArlBuilder:
    cdef decl.IAst2ArlBuilder       *_hndl
    cdef bool                       _owned

    cpdef void build(self,
        ast.SymbolScope root,
        Ast2ArlContext  ctxt)

    @staticmethod
    cdef Ast2ArlBuilder mk(decl.IAst2ArlBuilder *hndl, bool owned=*)

cdef class Ast2ArlContext:
    cdef decl.IAst2ArlContext       *_hndl
    cdef bool                       _owned

    @staticmethod
    cdef Ast2ArlContext mk(decl.IAst2ArlContext *hndl, bool owned=*)
