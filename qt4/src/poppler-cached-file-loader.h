/* poppler-cached-file-loader.h: Qt4 interface to poppler
 * Copyright (C) 2015, Saheb Preet Singh <saheb.preet@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef __POPPLER_CACHED_FILE_LOADER_H__
#define __POPPLER_CACHED_FILE_LOADER_H__

#include <CachedFile.h>

#include "poppler-export.h"

class QIODevice;

namespace Poppler {

    class POPPLER_QT4_EXPORT PopplerCachedFileLoader : public CachedFileLoader
    {
	public:
	    PopplerCachedFileLoader( QIODevice * device );
	    ~PopplerCachedFileLoader();
	    size_t init( GooString *url, CachedFile* cachedFile );
	    int load( const std::vector<ByteRange> &ranges, CachedFileWriter * writer );

	private:
	    QIODevice * m_device;
    };

}

#endif

