#ifndef THE_FRESH_ENGINE_TRANSLATOR_H
#define THE_FRESH_ENGINE_TRANSLATOR_H

#include <maya/MPxFileTranslator.h>

class TheFreshEngineTranslator : public MPxFileTranslator
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


#endif /*THE_FRESH_ENGINE_TRANSLATOR_H*/