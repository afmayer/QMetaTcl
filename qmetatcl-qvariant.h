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
#include <QVariant>

class QMetaTclQVariant
{
public:
    static Tcl_Obj * createTclObj(const QVariant &qvariant, Tcl_Interp *interp);
private:
    QMetaTclQVariant(const QVariant &qvariant, Tcl_Interp *interp,
            const char *command);
    ~QMetaTclQVariant();
    static const Tcl_ObjType TclType;

    static void freeInternalRep(Tcl_Obj *obj);
    static void dupInternalRep(Tcl_Obj *src, Tcl_Obj *copy);
    static int setFromAny(Tcl_Interp *interp, Tcl_Obj *obj);

    static int commandProc(ClientData clientData, Tcl_Interp *interp,
            int objc, Tcl_Obj *const objv[]);

    QVariant m_qvariant;
    Tcl_Interp *m_interp;
    Tcl_Command m_tclCommandToken;
};
