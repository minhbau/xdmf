/*
  To create C++ files:
ICE_INCLUDES="-I${Xdmf_SOURCE_DIR} -I${Xdmf_SOURCE_DIR}/libsrc -I${Xdmf_BINARY_DIR}/libsrc -I${Xdmf_BINARY_DIR}/Ice/libsrc"

XdmfTcl.cxx:
swig -v -c++ -make_default -includeall -tcl -prefix Xdmf  -namespace ${ICE_INCLUDES} -o XdmfTcl.cxx Xdmf.i

XdmfPython.cxx:
swig -v -c++ -make_default -includeall -python -shadow $(ICE_INCLUDES) -o XdmfPython.cxx ${srcdir}/Xdmf.i

XdmfJava.cxx:
swig -v -c++ -make_default -includeall -shadow -java $(ICE_INCLUDES) -o XdmfJava.cxx ${srcdir}/Xdmf.i;  
*/

%module Xdmf
%{
    /*
#include <XdmfArray.h>
#include <XdmfAttribute.h>
#include <XdmfCharArray.h>
    */
#include <XdmfDOM.h>
#include <XdmfLightData.h>
#include <XdmfInformation.h>
#include <XdmfElement.h>
    /*
#include <XdmfDataDesc.h>
#include <XdmfExpression.h>
#include <XdmfFormat.h>
#include <XdmfFormatHDF.h>
#include <XdmfFormatMulti.h>
#include <XdmfFormatXML.h>
#include <XdmfGeometry.h>
#include <XdmfGrid.h>
#include <XdmfHDF.h>
#include <XdmfHeavyData.h>
    */
#include <XdmfObject.h>
    /*
#include <XdmfParameter.h>
#include <XdmfTopology.h>
#include <XdmfTransform.h>
#include <XdmfXNode.h>
#include <XdmfNDGM.h>
    */

#ifndef HAVE_STRTOLL
# define strtoll XDMF_strtoll
inline XDMF_LONG64 XDMF_strtoll(char *str, void*, int)
{
  XDMF_LONG64 result = 0;
  int negative=0;

  while (*str == ' ' || *str == '\t')
    {
    str++;
    }
  if (*str == '+')
    {
    str++;
    }
  else if (*str == '-')
    {
    negative = 1;
    str++;
    }

  while (*str >= '0' && *str <= '9')
    {
    result = (result*10)-(*str++ - '0');
    }
  return negative ? result : -result;
}
#else
# define XDMF_strtoll strtoll
#endif

%}

%include std_string.i
/*
%include XdmfArray.h
%include XdmfAttribute.h
%include XdmfCharArray.h
*/
%include XdmfDOM.h
%include XdmfLightData.h
%include XdmfInformation.h
%include XdmfElement.h
/*
%include XdmfDataDesc.h
%include XdmfExpression.h
%include XdmfFormat.h
%include XdmfFormatHDF.h
%include XdmfFormatMulti.h
%include XdmfFormatXML.h
%include XdmfGeometry.h
%include XdmfGrid.h
%include XdmfHDF.h
%include XdmfHDFSupport.h
%include XdmfHeavyData.h
*/
%include XdmfObject.h
/*
%include XdmfParameter.h
%include XdmfTopology.h
%include XdmfTransform.h
%include XdmfXNode.h
%include XdmfNDGM.h
*/

#ifdef SWIGPYTHON
%{
void XdmfSwigException(int code, const char* msg)
{
/*   SWIG_exception(code, msg); */
}
%}
#endif
