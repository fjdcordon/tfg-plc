#include <Python.h>

PyObject *import_name(const char *modname, const char *symbol){
    PyObject *u_name, *module;
    u_name = PyUnicode_FromString(modname);
    module = PyImport_Import(u_name);
    Py_DECREF(u_name);

    return PyObject_GetAttrString(module, symbol);
}

float calcularEnergia(){
    PyObject *func = import_name("/root/funciones", "calcularEnergia");
    PyObject *args = PyTuple_New(0);
    PyObject *kwargs = NULL;
    PyObject *result = PyObject_Call(func, args, kwargs);
    float s = PyFloat_AsDouble(result);
    Py_XDECREF(kwargs);
    Py_DECREF(result);
    return s;
}

int getStatus(){
    PyObject *func = import_name("/root/funciones", "getStatus");
    PyObject *args = PyTuple_New(0);
    PyObject *kwargs = NULL;
    PyObject *result = PyObject_Call(func, args, kwargs);
    int s = PyLong_AsLong(result);
    Py_XDECREF(kwargs);
    Py_DECREF(result);
    return s;
}

int getAnguloAct(){
    PyObject *func = import_name("/root/funciones", "getAnguloAct");
    PyObject *args = PyTuple_New(0);
    PyObject *kwargs = NULL;
    PyObject *result = PyObject_Call(func, args, kwargs);
    int s = PyLong_AsLong(result);
    Py_XDECREF(kwargs);
    Py_DECREF(result);
    return s;
}

int setStatus(int status){
    PyObject *func = import_name("/root/funciones", "setStatus");
    PyObject *args = Py_BuildValue("(i)", status);
    PyObject *kwargs = NULL;
    PyObject *result = PyObject_Call(func, args, kwargs);
    int s = PyLong_AsLong(result);
    Py_XDECREF(kwargs);
    Py_DECREF(result);
    return s;
}

int moverPlaca(int angulo){
    PyObject *func = import_name("/root/funciones", "moverPlaca");
    PyObject *args = Py_BuildValue("(i)", angulo);
    PyObject *kwargs = NULL;
    PyObject *result = PyObject_Call(func, args, kwargs);
    int s = PyLong_AsLong(result);
    Py_XDECREF(kwargs);
    Py_DECREF(result);
    return s;
}

int reset(int mem){
    PyObject *func = import_name("/root/funciones", "reset");
    PyObject *args = Py_BuildValue("(i)", mem);
    PyObject *kwargs = NULL;
    PyObject *result = PyObject_Call(func, args, kwargs);
    int s = PyLong_AsLong(result);
    Py_XDECREF(kwargs);
    Py_DECREF(result);
    return s;
}