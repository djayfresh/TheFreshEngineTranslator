#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxCommand.h>
#include <maya/MObject.h>
#include <maya/MDagPath.h>
#include <maya/MFnDagNode.h>
#include <maya/MItDag.h>
#include <maya/MPxTransformationMatrix.h>

class ShapeList : public MPxCommand
{
public:
	~ShapeList() {};
	virtual MStatus	doIt ( const MArgList& )
	{
		cout.clear();
		return doScan();;
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

	MItDag dagIterator(MItDag::kDepthFirst, MFn::kShape, &status);
	if(!status)
	{
		status.perror("MItDag::dagIterator");
		return status;
	}
	for(;!dagIterator.isDone(); dagIterator.next())
	{
		MDagPath dagPath;
		status = dagIterator.getPath(dagPath);
		if(!status)
		{
			status.perror("MItDag::getPath");
			continue;
		}

		MFnDagNode dagNode(dagPath, &status);
		if(!status)
		{
			status.perror("MFnDagNode constructor");
			continue;
		}

		cout << dagNode.name().asChar() << ": " << dagNode.typeName().asChar() << endl;
		cout << " dagPath: " << dagPath.fullPathName().asChar() << endl;
		//**
		if(dagPath.hasFn(MFn::kTransform))
		{
			printTransformData(dagPath);
		}
		//**/
		cout << endl;
		objectCount += 1;
	}
	cout << "ObjectCount: " << objectCount << endl;
	cout.flush();
	return status;
}

void ShapeList::printTransformData(const MDagPath& path)
{
	//**
	//This method simply determines the transformation information on the DAG node and prints it out.
    MStatus status;
    MObject transformNode = path.transform(&status);
    // This node has no transform - i.e., it’s the world node
    if (!status && status.statusCode () == MStatus::kInvalidParameter)
        return;
    MFnDagNode transform (transformNode, &status);
    if (!status) {
        status.perror("MFnDagNode constructor");
        return;
    }
    MTransformationMatrix matrix (transform.transformationMatrix());
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