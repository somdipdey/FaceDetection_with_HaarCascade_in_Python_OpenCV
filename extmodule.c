#include <Python.h>

//Define a new exception object for our module
static PyObject *extError;

static PyObject* ext_cpu(PyObject* self, PyObject *args)
{
  int pid;
  int sts=0;

  //We expect at least 1 argument to this function
  if(!PyArg_ParseTuple(args, "i", &pid))
  {
    return NULL;
  }


  printf("Hello, from C World! Pid: %i", pid);
  sts=pid+1;

  return Py_BuildValue("i", sts);
}

static PyMethodDef ext_methods[] = {
  //PythonName, C-FunctionName, argument_presentation, description
  {"cpu", ext_cpu, METH_VARARGS, "Print cpu consumption of a particular process with pid"},
  {NULL, NULL, 0, NULL}
};

static struct PyModuleDef ext_module = {
  PyModuleDef_HEAD_INIT,
  "ext",
  "Extension module that does stuff",
  -1,
  ext_methods
};

PyMODINIT_FUNC
PyInit_ext(void)
{
    PyObject *m;

    m = PyModule_Create(&ext_module);
    if (m == NULL)
        return NULL;

    extError = PyErr_NewException("spam.error", NULL, NULL);
    Py_INCREF(extError);
    PyModule_AddObject(m, "error", extError);
    return m;
}
