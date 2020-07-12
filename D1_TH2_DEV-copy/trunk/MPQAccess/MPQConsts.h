//////////////////////////////////////////////////////////////////////////
// THE HELL 2 MOD
// MPQAccess project
//
// File: MPQConsts.h
// Description:
//    Constants for MPQAccess
// Author: spaun2002@pisem.net
//////////////////////////////////////////////////////////////////////////

#pragma once

namespace MPQAccess {

enum OpenFileMode {
	OpenFromMPQ = 0,	//The file is open from the MPQ. This is the default value. hMpq must be valid if SFILE_OPEN_FROM_MPQ is specified.
	OpenPatched = 1,	//The file will be open as patched. Reading data from the file results in getting patched file data.
	OpenIndexed = 2,	//The file is opened by index. See Remarks section for more information.
	OpenLocal = 0xffffffff	//Opens a local file instead. The file is open using CreateFileEx with GENERIC_READ access and FILE_SHARE_READ mode.
};

} //MPQAccess