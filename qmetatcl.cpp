// This file is part of QMetaTcl.
//
// QMetaTcl is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// QMetaTcl is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with QMetaTcl.  If not, see <http://www.gnu.org/licenses/>.

#include <tcl.h>

extern "C" int Qmetatcl_Init(Tcl_Interp *interp)
{
#ifdef USE_TCL_STUBS
    if (Tcl_InitStubs(interp, TCL_VERSION, 0) == NULL) {
        Tcl_SetResult(interp, (char *)"Tcl_InitStubs failed", TCL_STATIC);
        return TCL_ERROR;
    }
#endif
    return TCL_OK;
}
