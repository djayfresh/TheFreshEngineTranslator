#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxCommand.h>
#include <maya/MObject.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MItDag.h>
#include <maya/MPxTransformationMatrix.h>
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

class ShapeList : public MPxCommand
{
public:
	~ShapeList() {};
	virtual MStatus	doIt ( const MArgList& )
	{
		cout.clear();
		return doScan();
	}
	MStatus doScan();
	void printTransformData(const MDagPath& path);
	static void* creator()
	{
		return new ShapeList;
	}
private:
	MDagPath path;
};

MStatus ShapeList::doScan()
{
	uint objectCount = 0;
	MStatus status;

	std::ofstream file;
	file.open("MayaTestFile.frh", std::ios::binary | std::ios::out);
	MayaFileExporter exporter(file);

	status = exporter.parseScene();
	status = exporter.writeFile();
	return status;
}

void ShapeList::printTransformData(const MDagPath& path)
{
	//**
	//This method simply determines the transformation information on the DAG node and prints it out.
    MStatus status;
	MMatrix transformNode = path.inclusiveMatrix(&status);
    // This node has no transform - i.e., it’s the world node
    if (!status && status.statusCode () == MStatus::kInvalidParameter)
        return;
	/**
    MFnDagNode transform (transformNode, &status);
    if (!status) {
        status.perror("MFnDagNode constructor");
        return;
    }
	//**/
	MTransformationMatrix matrix (transformNode); //(transform.transformationMatrix())
	MVector translation = matrix.translation(MSpace::kWorld);
	cout << " translation: ["
			<< translation.x << ", "
            << translation.y << ", "
            << translation.z << "]\n";
    double threeDoubles[3];
    MTransformationMatrix::RotationOrder rOrder;
    matrix.getRotation (threeDoubles, rOrder, MSpace::kWorld);
    cout << " rotation: ["
            << threeDoubles[0] << ", "
            << threeDoubles[1] << ", "
            << threeDoubles[2] << "]\n";
    matrix.getScale (threeDoubles, MSpace::kWorld);
    cout << " scale: ["
            << threeDoubles[0] << ", "
            << threeDoubles[1] << ", "
            << threeDoubles[2] << "]\n";
	/**/
}

MStatus initializePlugin( MObject _obj )	
{
	MFnPlugin plugin(_obj, "ShapeList", "1.0");
	return plugin.registerCommand("ShapeList", ShapeList::creator);
}

MStatus uninitializePlugin(MObject _obj)
{
	MFnPlugin plugin(_obj);
	return plugin.deregisterCommand("ShapeList");
}