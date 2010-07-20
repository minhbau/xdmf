#include <sstream>
#include "XdmfArrayType.hpp"
#include "XdmfHDF5Controller.hpp"

int main(int argc, char* argv[])
{
	boost::shared_ptr<XdmfHDF5Controller> controller = XdmfHDF5Controller::New("output.h5:/foo/data1", 10, XdmfArrayType::Int8());
	assert(controller->getDataSetName().compare("/foo/data1") == 0);

	std::string realPath = XdmfObject::getRealPath("output.h5");

	std::stringstream realDataSetPath;
	realDataSetPath << realPath << ":" << "/foo/data1";

	assert(controller->getDataSetPath().compare(realDataSetPath.str()) == 0);
	assert(controller->getFilePath().compare(realPath) == 0);
	assert(controller->getSize() == 10);
	assert(controller->getType() == XdmfArrayType::Int8());
	return 0;
}
