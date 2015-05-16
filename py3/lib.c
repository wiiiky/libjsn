/*
 * Copyright (C) 2015  Wiky L
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with main.c; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */

#include <Python.h>
#include <libjsn.h>

/* TODO */
static PyObject *libjsn_loads(PyObject *self, PyObject *args)
{
    const char *data;
    if(!PyArg_ParseTuple(args, "s", &data)){
        return NULL;
    }
    JSONNode *root=json_loads_from_data(data);
    if(root==NULL){
        return Py_BuildValue("i",0);
    }
    json_node_free(root);
    return Py_BuildValue("i",1);
}
