/**

#include <maya/MSimple.h>
#include <maya/MIOStream.h>

DeclareSimpleCommand( helloWorld, "Doug Fresh", "2015");
MStatus helloWorld::doIt( const MArgList& args )
{
    cout << "Hello world" << endl;
    return MS::kSuccess;
}
//**/