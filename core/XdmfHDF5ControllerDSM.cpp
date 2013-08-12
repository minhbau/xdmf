/*****************************************************************************/
/*                                    XDMF                                   */
/*                       eXtensible Data Model and Format                    */
/*                                                                           */
/*  Id : XdmfHDF5ControllerDSM.cpp                                           */
/*                                                                           */
/*  Author:                                                                  */
/*     Kenneth Leiter                                                        */
/*     kenneth.leiter@arl.army.mil                                           */
/*     US Army Research Laboratory                                           */
/*     Aberdeen Proving Ground, MD                                           */
/*                                                                           */
/*     Copyright @ 2011 US Army Research Laboratory                          */
/*     All Rights Reserved                                                   */
/*     See Copyright.txt for details                                         */
/*                                                                           */
/*     This software is distributed WITHOUT ANY WARRANTY; without            */
/*     even the implied warranty of MERCHANTABILITY or FITNESS               */
/*     FOR A PARTICULAR PURPOSE.  See the above copyright notice             */
/*     for more information.                                                 */
/*                                                                           */
/*****************************************************************************/

#include <H5FDdsm.h>
#include <H5FDdsmManager.h>
#include <hdf5.h>
#include "XdmfHDF5ControllerDSM.hpp"
#include "XdmfDSMManager.hpp"
#include "XdmfDSMBuffer.hpp"
#include "XdmfDSMCommMPI.hpp"
#include "XdmfDSMDriver.hpp"
#include "XdmfError.hpp"

shared_ptr<XdmfHDF5ControllerDSM>
XdmfHDF5ControllerDSM::New(const std::string & hdf5FilePath,
                           const std::string & dataSetPath,
                           const shared_ptr<const XdmfArrayType> type,
                           const std::vector<unsigned int> & start,
                           const std::vector<unsigned int> & stride,
                           const std::vector<unsigned int> & dimensions,
                           const std::vector<unsigned int> & datspaceDimensions,
                           H5FDdsmBuffer * const dsmBuffer)
{
  shared_ptr<XdmfHDF5ControllerDSM> 
    p(new XdmfHDF5ControllerDSM(hdf5FilePath,
                                dataSetPath,
                                type,
                                start,
                                stride,
                                dimensions,
                                datspaceDimensions,
                                dsmBuffer));
  return p;
}

shared_ptr<XdmfHDF5ControllerDSM>
XdmfHDF5ControllerDSM::New(const std::string & hdf5FilePath,
                           const std::string & dataSetPath,
                           const shared_ptr<const XdmfArrayType> type,
                           const std::vector<unsigned int> & start,
                           const std::vector<unsigned int> & stride,
                           const std::vector<unsigned int> & dimensions,
                           const std::vector<unsigned int> & datspaceDimensions,
                           MPI_Comm comm,
                           unsigned int bufferSize)
{
  shared_ptr<XdmfHDF5ControllerDSM>
    p(new XdmfHDF5ControllerDSM(hdf5FilePath,
                                dataSetPath,
                                type,
                                start,
                                stride,
                                dimensions,
                                datspaceDimensions,
                                comm,
                                bufferSize));
  return p;
}

// Server/ nonthreaded versions
shared_ptr<XdmfHDF5ControllerDSM>
XdmfHDF5ControllerDSM::New(const std::string & hdf5FilePath,
                           const std::string & dataSetPath,
                           const shared_ptr<const XdmfArrayType> type,
                           const std::vector<unsigned int> & start,
                           const std::vector<unsigned int> & stride,
                           const std::vector<unsigned int> & dimensions,
                           const std::vector<unsigned int> & datspaceDimensions,
                           XdmfDSMBuffer * const dsmBuffer)
{
  shared_ptr<XdmfHDF5ControllerDSM>
    p(new XdmfHDF5ControllerDSM(hdf5FilePath,
                                dataSetPath,
                                type,
                                start,
                                stride,
                                dimensions,
                                datspaceDimensions,
                                dsmBuffer));
  return p;
}

shared_ptr<XdmfHDF5ControllerDSM>
XdmfHDF5ControllerDSM::New(const std::string & hdf5FilePath,
                           const std::string & dataSetPath,
                           const shared_ptr<const XdmfArrayType> type,
                           const std::vector<unsigned int> & start,
                           const std::vector<unsigned int> & stride,
                           const std::vector<unsigned int> & dimensions,
                           const std::vector<unsigned int> & datspaceDimensions,
                           MPI_Comm comm,
                           unsigned int bufferSize,
                           int startCoreIndex,
                           int endCoreIndex)
{
  shared_ptr<XdmfHDF5ControllerDSM>
    p(new XdmfHDF5ControllerDSM(hdf5FilePath,
                                dataSetPath,
                                type,
                                start,
                                stride,
                                dimensions,
                                datspaceDimensions,
                                comm,
                                bufferSize,
                                startCoreIndex,
                                endCoreIndex));
  return p;
}

XdmfHDF5ControllerDSM::XdmfHDF5ControllerDSM(const std::string & hdf5FilePath,
                                             const std::string & dataSetPath,
                                             const shared_ptr<const XdmfArrayType> type,
                                             const std::vector<unsigned int> & start,
                                             const std::vector<unsigned int> & stride,
                                             const std::vector<unsigned int> & dimensions,
                                             const std::vector<unsigned int> & dataspaceDimensions,
                                             H5FDdsmBuffer * const dsmBuffer) :
  XdmfHDF5Controller(hdf5FilePath, 
                     dataSetPath, 
                     type, 
                     start,
                     stride,
                     dimensions, 
                     dataspaceDimensions),
  mDSMManager(NULL),
  mDSMBuffer(dsmBuffer),
  mDSMServerBuffer(NULL),
  mDSMServerManager(NULL),
  mWorkerComm(MPI_COMM_NULL),
  mServerMode(false)
{
}

XdmfHDF5ControllerDSM::XdmfHDF5ControllerDSM(const std::string & hdf5FilePath,
                                             const std::string & dataSetPath,
                                             const shared_ptr<const XdmfArrayType> type,
                                             const std::vector<unsigned int> & start,
                                             const std::vector<unsigned int> & stride,
                                             const std::vector<unsigned int> & dimensions,
                                             const std::vector<unsigned int> & dataspaceDimensions,
                                             MPI_Comm comm,
                                             unsigned int bufferSize) :
  XdmfHDF5Controller(hdf5FilePath,
                     dataSetPath,
                     type,
                     start,
                     stride,
                     dimensions,
                     dataspaceDimensions),
  mDSMServerBuffer(NULL),
  mDSMServerManager(NULL),
  mWorkerComm(MPI_COMM_NULL),
  mServerMode(false)

{
  H5FDdsmManager * newManager = new H5FDdsmManager();
  newManager->SetMpiComm(comm);
  newManager->SetLocalBufferSizeMBytes(bufferSize);
  newManager->SetIsStandAlone(H5FD_DSM_TRUE);
  newManager->Create();

  H5FD_dsm_set_manager(newManager);

  H5FD_dsm_set_options(H5FD_DSM_LOCK_ASYNCHRONOUS);

  H5FDdsmBuffer * newBuffer = newManager->GetDsmBuffer();

  mDSMManager = newManager;
  mDSMBuffer = newBuffer;
}

XdmfHDF5ControllerDSM::XdmfHDF5ControllerDSM(const std::string & hdf5FilePath,
                                             const std::string & dataSetPath,
                                             const shared_ptr<const XdmfArrayType> type,
                                             const std::vector<unsigned int> & start,
                                             const std::vector<unsigned int> & stride,
                                             const std::vector<unsigned int> & dimensions,
                                             const std::vector<unsigned int> & dataspaceDimensions,
                                             XdmfDSMBuffer * const dsmBuffer) :
  XdmfHDF5Controller(hdf5FilePath,
                     dataSetPath,
                     type,
                     start,
                     stride,
                     dimensions,
                     dataspaceDimensions),
  mDSMManager(NULL),
  mDSMBuffer(NULL),
  mDSMServerBuffer(dsmBuffer),
  mDSMServerManager(NULL),
  mServerMode(true)
{
  mWorkerComm = mDSMServerBuffer->GetComm()->GetIntraComm();
  if (xdmf_dsm_get_manager() == NULL) {
    mDSMServerManager = new XdmfDSMManager();
    mDSMServerManager->SetLocalBufferSizeMBytes(mDSMServerBuffer->GetLength());
    mDSMServerManager->SetInterCommType(H5FD_DSM_COMM_MPI);
    mDSMServerManager->SetIsServer(false);
    mDSMServerManager->SetMpiComm(mDSMServerBuffer->GetComm()->GetIntraComm());
    mDSMServerManager->SetDsmBuffer(mDSMServerBuffer);
    XDMF_dsm_set_manager(mDSMServerManager);
  }
  else {
    static_cast<XdmfDSMManager *>(xdmf_dsm_get_manager())->SetDsmBuffer(mDSMServerBuffer);
  }
}

XdmfHDF5ControllerDSM::XdmfHDF5ControllerDSM(const std::string & hdf5FilePath,
                                             const std::string & dataSetPath,
                                             const shared_ptr<const XdmfArrayType> type,
                                             const std::vector<unsigned int> & start,
                                             const std::vector<unsigned int> & stride,
                                             const std::vector<unsigned int> & dimensions,
                                             const std::vector<unsigned int> & dataspaceDimensions,
                                             MPI_Comm comm,
                                             unsigned int bufferSize,
                                             int startCoreIndex,
                                             int endCoreIndex) :
  XdmfHDF5Controller(hdf5FilePath,
                     dataSetPath,
                     type,
                     start,
                     stride,
                     dimensions,
                     dataspaceDimensions),
  mDSMBuffer(NULL),
  mDSMManager(NULL),
  mServerMode(true)

{

  int rank, size;

  MPI_Comm_size(comm, &size);
  MPI_Comm_rank(comm, &rank);

  // Negative values will be changed to maximum range
  if (startCoreIndex < 0) {
    startCoreIndex = 0;
  }
  if (endCoreIndex < 0) {
    endCoreIndex = size - 1;
  }

  // Ensure start index is less than end index
  if (startCoreIndex > endCoreIndex) {
    int tempholder = startCoreIndex;
    startCoreIndex = endCoreIndex;
    endCoreIndex = tempholder;
  }

  MPI_Comm serverComm;

  MPI_Group workers, dsmgroup, serversplit, servergroup;

  int * ServerIds = (int *)calloc((3), sizeof(int));
  unsigned int index = 0;
  for(int i=startCoreIndex ; i <= endCoreIndex ; ++i) {
    ServerIds[index++] = i;
  }

  MPI_Comm_group(comm, &serversplit);
  MPI_Group_incl(serversplit, index, ServerIds, &servergroup);
  MPI_Comm_create(comm, servergroup, &serverComm);
  MPI_Comm_group(comm, &dsmgroup);
  MPI_Group_excl(dsmgroup, index, ServerIds, &workers);
  MPI_Comm_create(comm, workers, &mWorkerComm);
  cfree(ServerIds);

  // Create the manager

  mDSMServerManager = new XdmfDSMManager();

  mDSMServerManager->SetLocalBufferSizeMBytes(bufferSize);
  mDSMServerManager->SetInterCommType(H5FD_DSM_COMM_MPI);

  if (rank >= startCoreIndex && rank <= endCoreIndex) {
    mDSMServerManager->SetMpiComm(serverComm);
    mDSMServerManager->Create();
  }
  else {
    mDSMServerManager->SetMpiComm(mWorkerComm);
    mDSMServerManager->SetIsServer(false);
    mDSMServerManager->Create(startCoreIndex, endCoreIndex);
  }

  XDMF_dsm_set_manager(mDSMServerManager);

  mDSMServerBuffer = mDSMServerManager->GetDsmBuffer();

  mDSMServerBuffer->GetComm()->DupInterComm(comm);
  mDSMServerBuffer->SetIsConnected(true);

  if (startCoreIndex < size) {
    if (rank >= startCoreIndex && rank <= endCoreIndex) {
      mDSMServerManager->GetDsmBuffer()->ReceiveInfo();
    }
    else {
      mDSMServerManager->GetDsmBuffer()->SendInfo();
    }
  }

  MPI_Barrier(comm);

  // Loop needs to be started before anything can be done to the file
  // since the service is what sets up the file

  if (rank < startCoreIndex || rank > endCoreIndex) {
    // Turn off the server designation
    mDSMServerBuffer->SetIsServer(H5FD_DSM_FALSE);
    // If this is set to false then the buffer will attempt to
    // connect to the intercomm for DSM stuff
    mDSMServerManager->SetIsServer(H5FD_DSM_FALSE);
  }
  else {
    // On cores where memory is set up, start the service loop
    // This should iterate infinitely until a value to end the loop is passed
    H5FDdsmInt32 returnOpCode;
    try {
      mDSMServerBuffer->BufferServiceLoop(&returnOpCode);
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

XdmfHDF5ControllerDSM::~XdmfHDF5ControllerDSM()
{
}

void XdmfHDF5ControllerDSM::deleteManager()
{
  if (mDSMManager != NULL) {
    delete mDSMManager;
  }
  if (mDSMServerManager != NULL) {
    delete mDSMServerManager;
  }
}

std::string XdmfHDF5ControllerDSM::getName() const
{
  return "HDFDSM";
}

H5FDdsmBuffer * XdmfHDF5ControllerDSM::getBuffer()
{
  return mDSMBuffer;
}

H5FDdsmManager * XdmfHDF5ControllerDSM::getManager()
{
  return mDSMManager;
}

XdmfDSMBuffer * XdmfHDF5ControllerDSM::getServerBuffer()
{
  return mDSMServerBuffer;
}

XdmfDSMManager * XdmfHDF5ControllerDSM::getServerManager()
{
  return mDSMServerManager;
}

bool XdmfHDF5ControllerDSM::getServerMode()
{
  return mServerMode;
}

MPI_Comm XdmfHDF5ControllerDSM::getWorkerComm()
{
  MPI_Comm returnComm = MPI_COMM_NULL;
  int status = MPI_Comm_dup(mWorkerComm, &returnComm);
  return returnComm;
}

void XdmfHDF5ControllerDSM::setManager(XdmfDSMManager * newManager)
{
  XdmfDSMBuffer * newBuffer = newManager->GetDsmBuffer();
  mDSMServerManager = newManager;
  mDSMServerBuffer = newBuffer;
}

void XdmfHDF5ControllerDSM::setManager(H5FDdsmManager * newManager)
{
  H5FDdsmBuffer * newBuffer = newManager->GetDsmBuffer();
  mDSMManager = newManager;
  mDSMBuffer = newBuffer;
}

void XdmfHDF5ControllerDSM::setBuffer(XdmfDSMBuffer * newBuffer)
{
  mDSMServerBuffer = newBuffer;
}

void XdmfHDF5ControllerDSM::setBuffer(H5FDdsmBuffer * newBuffer)
{
  mDSMBuffer = newBuffer;
}

void XdmfHDF5ControllerDSM::setServerMode(bool newMode)
{
  mServerMode = newMode;
}

void XdmfHDF5ControllerDSM::setWorkerComm(MPI_Comm comm)
{
  int status;
  if (mWorkerComm != MPI_COMM_NULL) {
    status = MPI_Comm_free(&mWorkerComm);
    if (status != MPI_SUCCESS) {
      try {
        XdmfError::message(XdmfError::FATAL, "Failed to disconnect Comm");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  if (comm != MPI_COMM_NULL) {
    status = MPI_Comm_dup(comm, &mWorkerComm);
    if (status != MPI_SUCCESS) {
      try {
        XdmfError::message(XdmfError::FATAL, "Failed to duplicate Comm");
      }
      catch (XdmfError e) {
        throw e;
      }
    }
  }
  mDSMServerBuffer->GetComm()->DupComm(comm);
}

void XdmfHDF5ControllerDSM::stopDSM()
{
  // Send manually
  for (int i = mDSMServerBuffer->GetStartServerId();
       i <= mDSMServerBuffer->GetEndServerId();
       ++i) {
    try {
      mDSMServerBuffer->SendCommandHeader(H5FD_DSM_OPCODE_DONE, i, 0, 0, H5FD_DSM_INTER_COMM);
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void XdmfHDF5ControllerDSM::restartDSM()
{
  if (mDSMServerBuffer->GetComm()->GetInterId() >=
        mDSMServerBuffer->GetStartServerId() &&
      mDSMServerBuffer->GetComm()->GetInterId() <=
        mDSMServerBuffer->GetEndServerId()) {
    H5FDdsmInt32 returnOpCode;
    try {
      mDSMServerBuffer->BufferServiceLoop(&returnOpCode);
    }
    catch (XdmfError e) {
      throw e;
    }
  }
}

void XdmfHDF5ControllerDSM::read(XdmfArray * const array)
{
  // Set file access property list for DSM
  hid_t fapl = H5Pcreate(H5P_FILE_ACCESS);

  // Use DSM driver
  if (mServerMode) {
    if (mWorkerComm != MPI_COMM_NULL) {
      XDMFH5Pset_fapl_dsm(fapl, mWorkerComm, mDSMServerBuffer, 0);
    }
  }
  else {
    H5Pset_fapl_dsm(fapl, MPI_COMM_WORLD, mDSMBuffer, 0);
  }

  // Read from DSM Buffer
  XdmfHDF5Controller::read(array, fapl);

  // Close file access property list
  herr_t status = H5Pclose(fapl);
}
