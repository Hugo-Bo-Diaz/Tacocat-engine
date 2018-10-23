#pragma once

#define IMPORT_PATH "Library"

class Importer
{
public:
	Importer() {};
	virtual void CreateOwnFile() { return; };
	virtual void Import() { return; };

};