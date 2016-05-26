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

#include <qmetatcl-qvariant.h>
#include <stdio.h>

static unsigned int objectNameCounter;

static const char * generateCommandName(Tcl_Interp *interp)
{
    static char nameBuffer[64];
    Tcl_CmdInfo commandInfo;
    do {
        snprintf(nameBuffer, sizeof(nameBuffer), "::qmetatcl::qvariant%u",
                objectNameCounter);
        objectNameCounter++;
    } while (Tcl_GetCommandInfo(interp, nameBuffer, &commandInfo));
    return nameBuffer;
}

const Tcl_ObjType QMetaTclQVariant::TclType = {
    "QMetaTcl-QVariant",
    freeInternalRep,
    dupInternalRep,
    NULL, // updateStringProc not needed - string rep is never invalidated
    setFromAny
};

Tcl_Obj * QMetaTclQVariant::createTclObj(const QVariant &qvariant,
        Tcl_Interp *interp)
{
    Tcl_Obj *tclObj = Tcl_NewStringObj(generateCommandName(interp), -1);

    // a new string object shouldn't have an internal representation - however,
    // proper cleanup is done as a safeguard mechanism
    if (tclObj->typePtr && tclObj->typePtr->freeIntRepProc)
        tclObj->typePtr->freeIntRepProc(tclObj);

    tclObj->internalRep.twoPtrValue.ptr1 =
            new QMetaTclQVariant(qvariant, interp, Tcl_GetString(tclObj));
    tclObj->internalRep.twoPtrValue.ptr2 = NULL;
    tclObj->typePtr = &TclType;
    return tclObj;
}

QMetaTclQVariant::QMetaTclQVariant(const QVariant &qvariant, Tcl_Interp *interp,
        const char *command)
    : m_qvariant(qvariant)
    , m_interp(interp)
    , m_tclCommandToken(NULL)
{
    m_tclCommandToken =
            Tcl_CreateObjCommand(interp, command, commandProc, this, NULL);
}

QMetaTclQVariant::~QMetaTclQVariant()
{
    Tcl_DeleteCommandFromToken(m_interp, m_tclCommandToken);
}

void QMetaTclQVariant::freeInternalRep(Tcl_Obj *obj)
{
    QMetaTclQVariant *thiz =
            static_cast<QMetaTclQVariant *>(obj->internalRep.twoPtrValue.ptr1);
    delete thiz;
    obj->typePtr = NULL;
}

void QMetaTclQVariant::dupInternalRep(Tcl_Obj *src, Tcl_Obj *copy)
{
    // create a new QMetaTclQVariant object
    QMetaTclQVariant *thiz =
            static_cast<QMetaTclQVariant *>(src->internalRep.twoPtrValue.ptr1);
    Tcl_SetStringObj(copy, generateCommandName(thiz->m_interp), -1);
    if (copy->typePtr && copy->typePtr->freeIntRepProc)
        copy->typePtr->freeIntRepProc(copy);
    copy->internalRep.twoPtrValue.ptr1 = new QMetaTclQVariant(
            thiz->m_qvariant, thiz->m_interp, Tcl_GetString(copy));
    copy->internalRep.twoPtrValue.ptr2 = NULL;
    copy->typePtr = &TclType;
}

int QMetaTclQVariant::setFromAny(Tcl_Interp *interp, Tcl_Obj *obj)
{
    if (obj->typePtr == &TclType)
        return TCL_OK;

    // cleanup old internal representation
    if (obj->typePtr != NULL && obj->typePtr->freeIntRepProc != NULL)
        obj->typePtr->freeIntRepProc(obj);
    obj->typePtr = NULL;

    // pointer to QMetaTclQVariant is extracted from the Tcl_CmdInfo struct
    // of the object's command - this only works when that command has not
    // been renamed or deleted
    const char *commandName = Tcl_GetStringFromObj(obj, NULL);
    Tcl_CmdInfo commandInfo;
    if (!Tcl_GetCommandInfo(interp, commandName, &commandInfo)) {
        if (interp)
            Tcl_AppendResult(interp, commandName, " not found", (char *)NULL);
        return TCL_ERROR;
    }
    if (commandInfo.objProc != commandProc) {
        if (interp)
            Tcl_AppendResult(interp, commandName, " is not a QVariant",
                    (char *)NULL);
        return TCL_ERROR;
    }
    obj->internalRep.twoPtrValue.ptr1 = commandInfo.clientData;
    obj->typePtr = &TclType;
    return TCL_OK;
}

int QMetaTclQVariant::commandProc(ClientData clientData, Tcl_Interp *interp,
        int objc, Tcl_Obj * const objv[])
{
    return TCL_OK;
}
