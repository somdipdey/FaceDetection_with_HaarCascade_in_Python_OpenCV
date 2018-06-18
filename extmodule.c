#include <Python.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* Program to get CPU usage*/

struct pstat {
    long unsigned int utime_ticks;
    long int cutime_ticks;
    long unsigned int stime_ticks;
    long int cstime_ticks;
    long unsigned int vsize; // virtual memory size in bytes
    long unsigned int rss; //Resident  Set  Size in bytes

    long unsigned int cpu_total_time;
};

/*
 * read /proc data into the passed struct pstat
 * returns 0 on success, -1 on error
*/
int get_usage(const pid_t pid, struct pstat* result) {
    char stat_filepath[80] ;
    int ii;
        // use snprintf() , not str[n]cat() to compose strings
    ii = snprintf(stat_filepath, sizeof stat_filepath, "/proc/%d/stat", pid );
    if (ii >= (int)sizeof(stat_filepath)) return -1;

    FILE *fpstat = fopen(stat_filepath, "r");
    if (fpstat == NULL) {
        perror("FOPEN ERROR "); // <<-- should be printed by caller
        return -1;
    }

    FILE *fstat = fopen("/proc/stat", "r");
    if (fstat == NULL) {
        perror("FOPEN ERROR "); // <<-- could be printed by caller
        fclose(fstat);
        return -1;
    }

    //read values from /proc/pid/stat
    memset(result, 0 , sizeof *result ); // bzero is a BSD-ism
    long int rss;
    if (fscanf(fpstat, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu"
                "%lu %ld %ld %*d %*d %*d %*d %*u %lu %ld",
                &result->utime_ticks, &result->stime_ticks,
                &result->cutime_ticks, &result->cstime_ticks, &result->vsize,
                &rss) == EOF) {
        fclose(fpstat);
        return -1;
    }
    fclose(fpstat);
    result->rss = rss * getpagesize();

    //read+calc cpu total time from /proc/stat
    long unsigned int cpu_time[10];
    memset(cpu_time,0, sizeof cpu_time);
    if (fscanf(fstat, "%*s %lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
                &cpu_time[0], &cpu_time[1], &cpu_time[2], &cpu_time[3],
                &cpu_time[4], &cpu_time[5], &cpu_time[6], &cpu_time[7],
                &cpu_time[8], &cpu_time[9]) == EOF) {
        fclose(fstat);
        return -1;
    }

    fclose(fstat);

    for(ii=0; ii < 10;ii++)
        result->cpu_total_time += cpu_time[ii];

    return 0;
}



/*
* calculates the elapsed CPU usage between 2 measuring points. in percent
*/
void calc_cpu_usage_pct(const struct pstat* cur_usage,
                        const struct pstat* last_usage,
                        double* ucpu_usage, double* scpu_usage)
{
    const long unsigned int total_time_diff = cur_usage->cpu_total_time -
                                              last_usage->cpu_total_time;

    *ucpu_usage = 100 * (((cur_usage->utime_ticks + cur_usage->cutime_ticks)
                    - (last_usage->utime_ticks + last_usage->cutime_ticks))
                    / (double) total_time_diff);

    *scpu_usage = 100 * ((((cur_usage->stime_ticks + cur_usage->cstime_ticks)
                    - (last_usage->stime_ticks + last_usage->cstime_ticks))) /
                    (double) total_time_diff);
}

/*
* calculates the elapsed CPU usage between 2 measuring points in ticks
*/
void calc_cpu_usage(const struct pstat* cur_usage,
                    const struct pstat* last_usage,
                    long unsigned int* ucpu_usage,
                    long unsigned int* scpu_usage)
{

    *ucpu_usage = (cur_usage->utime_ticks + cur_usage->cutime_ticks) -
                  (last_usage->utime_ticks + last_usage->cutime_ticks);

    *scpu_usage = (cur_usage->stime_ticks + cur_usage->cstime_ticks) -
                  (last_usage->stime_ticks + last_usage->cstime_ticks);
}
/* < -- program ends here*/


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

  struct pstat result;
  double cpu = 0.0;
  int success = get_usage(pid, &result);
  cpu = result.cpu_total_time;
  printf("CPU usage of %i %f \n", pid, cpu);

  sts=cpu;

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
