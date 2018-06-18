#include <Python.h>
#include <mach/mach_init.h>
#include <mach/mach_error.h>
#include <mach/mach_host.h>
#include <mach/vm_map.h>

/*<summary>
This is an extension function to check CPU load of the system on Mac OS and Linux.
Since calling a function to check CPU load varies from OS to OS, hence, different
functions needs to be written for different OSs.
</summary>*/

/* Program to get CPU usage*/
static unsigned long long _previousTotalTicks = 0;
static unsigned long long _previousIdleTicks = 0;

// Returns 1.0f for "CPU fully pinned", 0.0f for "CPU idle", or somewhere in between
// You'll need to call this at regular intervals, since it measures the load between
// the previous call and the current one.
float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks);
float GetCPULoad()
{
   host_cpu_load_info_data_t cpuinfo;
   mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
   if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuinfo, &count) == KERN_SUCCESS)
   {
      unsigned long long totalTicks = 0;
      for(int i=0; i<CPU_STATE_MAX; i++) totalTicks += cpuinfo.cpu_ticks[i];
      return CalculateCPULoad(cpuinfo.cpu_ticks[CPU_STATE_IDLE], totalTicks);
   }
   else return -1.0f;
}

float CalculateCPULoad(unsigned long long idleTicks, unsigned long long totalTicks)
{
  unsigned long long totalTicksSinceLastTime = totalTicks-_previousTotalTicks;
  unsigned long long idleTicksSinceLastTime  = idleTicks-_previousIdleTicks;
  float ret = 1.0f-((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime)/totalTicksSinceLastTime : 0);
  _previousTotalTicks = totalTicks;
  _previousIdleTicks  = idleTicks;
  return ret;
}
/* < -- program ends here*/


//Define a new exception object for our module
static PyObject *extError;

static PyObject* ext_cpu_mac(PyObject* self, PyObject *args)
{
  //int pid;//commented pid because it's not required now
  float sts=0;

  /*Commented the following argument fetching -->*/
  //We expect at least 1 argument to this function
  //if(!PyArg_ParseTuple(args, "i", &pid))
  //{
  //  return NULL;
  //}
  /*<--commenting ends here*/

  float cpu_load = GetCPULoad();

  printf("CPU load is %f \n", cpu_load);

  sts=cpu_load;

  return Py_BuildValue("f", sts);
}

static PyMethodDef ext_methods[] = {
  //PythonName, C-FunctionName, argument_presentation, description
  {"cpu_mac", ext_cpu_mac, METH_VARARGS, "Print cpu load on MAC OS"},
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
