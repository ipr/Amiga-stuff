#include <QtCore/QCoreApplication>
#include <QStringList>

//#include <QApplication>

#include "AnsiFile.h"

#include <iostream>

class HunkNode; // fwd.
class HunkNode
{
public:
	HunkNode() {}

	//HunkNode *next; // no need (currently..)

	uint32_t number;
	uint32_t offset;
	uint32_t size;

	// see types in OS headers <exec/types.h>
	uint32_t type; // note: check, guessing type field size

	// actual data in hunk..
	//void *data;
};

class ADosHeader
{
public:
	ADosHeader()
		: first(nullptr)
	{}
	~ADosHeader()
	{
		delete [] first;
	}

	uint32_t magic_four; // 0x000003F3

	uint32_t hunk_count; // note: guessing size
	//?? prog_numberofhunks; // ??
	//?? table_of_length; // sizes of each hunk..

	HunkNode *first;

	bool setHunkCount(uint32_t count)
	{
		hunk_count = count;
		if (count > 0)
		{
			first = new HunkNode[count];
		}
		if (first != nullptr)
		{
			return true;
		}
		return false;
	}

	//bool read()
};

bool dumpNode(HunkNode *node);

bool dumpFile(CAnsiFile &file);

void toHex(unsigned char *pDigest, const size_t nLen, QString &szOutput);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

	//QStringList vCmdLine = QApplication::arguments();
	QStringList vCmdLine = a.arguments();
	if (vCmdLine.size() < 2)
	{
		// print help & exit

		return 1;
	}

	CAnsiFile file(vCmdLine[1].toStdString());
	if (file.IsOk() == false)
	{
		// print error & exit
		return 2;
	}

	// dump it (temp stuff mostly)
	dumpFile(file);

    return a.exec();
}

bool dumpNode(HunkNode *node)
{
}

bool dumpFile(CAnsiFile &file)
{
	CIOBuffer buffer(1024); // at least 1kB

	size_t nSize = file.GetSize();
	size_t nRead = 0;
	while (nRead < nSize)
	{
		size_t next = ((nSize - nRead) > 1024) ? 1024 : (nSize - nRead);
		if (file.Read(buffer.GetBegin(), next) == false)
		{
			// could throw, check compiler first..
			return false;
		}
		nRead += next;

		// this is temp stuff..
		size_t nPos = 0;
		while (next > 0)
		{
			QString dump;
			toHex(buffer.GetAt(nPos),
				  (next > 32) ? 32 : next,
				  dump);
			nPos += (next > 32) ? 32 : next;
			next -= 32;

			std::cout << dump.toStdString() << std::endl;
		}
	}

	return true;
}

void toHex(unsigned char *pDigest, const size_t nLen, QString &szOutput)
{
	char hextable[] = "0123456789ABCDEF";

	// reserve 2*iLen space in output
	// (should improve efficiency slightly..)
	//
	size_t nSize = (szOutput.size() + (nLen*2));
	size_t nCapacity = szOutput.capacity();
	if (nCapacity < nSize)
	{
		szOutput.reserve(nSize);
	}

	// determine half-bytes of each byte
	// and appropriate character representing value of it
	// for hex-encoded string
	for ( size_t y = 0; y < nLen; y++ )
	{
		unsigned char upper;
		unsigned char lower;

		upper = lower = pDigest[y];

		lower = lower & 0xF;

		upper = upper >> 4;
		upper = upper & 0xF;

		// STL string grows automatically so we just push new
		// characters at the end, same way with reserve().
		//
		szOutput += hextable[upper]; szOutput += hextable[lower];
	}
}
