#include <maya/MString.h>
#include <maya/MGlobal.h>
#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MObject.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MItDag.h>
#include <maya/MPxTransformationMatrix.h>
#include <maya/MPxFileTranslator.h>
#include "MayaFileExporter.h"
#include <fstream>

/**
* When adding a kMesh iterator using inclusiveMatrix returns even Parented transforms to world space.
* dag.name() is the mesh name (name'shape') not the transform name. This way shapes of the same mesh 
* can be grouped for tranlations only, and not duplicate mesh data. 
* A new type of file will need to be created. Not only does each header section need a unquie value 
* to distingush header types, but an addition head slot pointing to end header groups would allow 
* for future expansion, or new headers. Some types to consider our Lights and Cameras.
/**/



class ShapeList : public MPxFileTranslator
{
public:

	MStatus	writer (const MFileObject& file,
									const MString& optionsString,
									MPxFileTranslator::FileAccessMode mode);
	bool haveWriteMethod () const;
	bool haveReadMethod () const;
	bool canBeOpened () const;

	MString defaultExtension () const;
	
	static void* creator();
protected:
	MStatus exportAll(ostream& os);
	MStatus exportSelection(ostream& os);
};

MStatus ShapeList::writer(const MFileObject& fileObject,const MString& optionsString, MPxFileTranslator::FileAccessMode mode)
{
	uint objectCount = 0;
	MStatus status;

	#if defined (OSMac_)
		char nameBuffer[MAXPATHLEN];
		strcpy (nameBuffer, file.fullName().asChar());
		const MString fileName(nameBuffer);
	#else
		const MString fileName = fileObject.fullName();
	#endif

	std::ofstream file;
	file.open(fileName.asChar(), std::ios::binary | std::ios::out);
	MayaFileExporter exporter(file);

	//**
	if (MPxFileTranslator::kExportAccessMode == mode) {
		if (MStatus::kFailure == exporter.parseScene()) {
			return MStatus::kFailure;
		}
	} else if (MPxFileTranslator::kExportActiveAccessMode == mode) {
		if (MStatus::kFailure == exporter.parseSelection()) {
			return MStatus::kFailure;
		}
	} else {
		return MStatus::kFailure;
	}
	/**/

	
	status = exporter.writeFile();
	file.close();

	MGlobal::displayInfo("Export to " + fileName + " successful!");
	return status;
}

bool ShapeList::haveWriteMethod () const
{
	return true;
}
bool ShapeList::haveReadMethod () const
{
	return false;
}
bool ShapeList::canBeOpened () const
{
	return false;
}

MString ShapeList::defaultExtension () const
{
	return "frh";
}

void* ShapeList::creator()
{
	return new ShapeList();
}

MStatus initializePlugin(MObject obj)
//Summary:	registers the commands, tools, devices, and so on, defined by the 
//			plug-in with Maya
//Returns:	MStatus::kSuccess if the registration was successful;
//			MStatus::kFailure otherwise
{
	MStatus status;
	MFnPlugin plugin(obj, "Doug Fresh", "0.5", "Any");

	// Register the translator with the system
	//
	status =  plugin.registerFileTranslator("The Fresh Engine",
											"",
											ShapeList::creator,
											"",
											"option1=1",
											true);
	if (!status) {
		status.perror("registerFileTranslator");
		return status;
	}

	return status;
}


MStatus uninitializePlugin(MObject obj) 
//Summary:	deregisters the commands, tools, devices, and so on, defined by the 
//			plug-in
//Returns:	MStatus::kSuccess if the deregistration was successful;
//			MStatus::kFailure otherwise
{
	MStatus   status;
	MFnPlugin plugin( obj );

	status =  plugin.deregisterFileTranslator("The Fresh Engine");
	if (!status) {
		status.perror("deregisterFileTranslator");
		return status;
	}

	return status;
}