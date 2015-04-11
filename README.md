# TheFreshEngineTranslator
A Autodesk Maya Plug-in to export to my game engines specific file type. Included is the binary file reader and writer as its own DLL so you can consume it within your own Engine.

##Install the Plug-in
1. Clone the repository
2. Open the Maya.sln
3. Open the FileExporter\FileExporter.sln
4. Build the FileExporter
5. Build the Maya project *TheFreshEngineTranslator*
6. Copy the Output .mll file `Maya\x64\Debug` to your Maya Plug-in dirctory
7. In Maya go to *Window\Settings\Plug-in Manager*
*Note: If the plug-in is not there, feel free to browse for it. But you can include it in your Environment Path on a system level with MAYA_PLUG_IN_PATH

##File Exporter
*Can be added for use in reading files
*No wrapper has been added for the Reader
*Headers for the file are included in FileExporter\FileExporter\Headers.h
