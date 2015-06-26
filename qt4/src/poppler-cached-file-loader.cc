/* poppler-cached-file-loader.cc: Qt4 interface to poppler
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

// qt/kde includes
#include <QtCore/QIODevice>

// local includes
#include "poppler-cached-file-loader.h"


namespace Poppler {

    PopplerCachedFileLoader::PopplerCachedFileLoader( QIODevice * device )
    {
	m_device = device;
    }

    PopplerCachedFileLoader::~PopplerCachedFileLoader()
    {
	m_device->close();
	delete m_device;
    }

    size_t PopplerCachedFileLoader::init( GooString * /*url*/, CachedFile * /*cachedFile*/ )
    {
	if( m_device->open( QIODevice::ReadOnly ) )
	{
	    return m_device->bytesAvailable();
	}
	else
	{
	    return -1;
	}
    }

    int PopplerCachedFileLoader::load( const std::vector<ByteRange> &ranges, CachedFileWriter * writer )
    {
	qint64 bytesRead,rangeBytesRead, bytesToRead;
	char buffer[ CachedFileChunkSize ];

	for (size_t i = 0; i < ranges.size(); i++) {
	    rangeBytesRead = 0;
	    bytesToRead = CachedFileChunkSize < ranges[ i ].length ? CachedFileChunkSize : ranges[ i ].length;
	    m_device->seek( ranges[ i ].offset );
	    do
	    {
		bytesRead = m_device->read( buffer, bytesToRead );
		if( bytesRead == -1 )
		    return -1;
		writer->write( buffer, bytesRead );
		rangeBytesRead += bytesRead;
		bytesToRead = ranges[ i ].length - rangeBytesRead;
	    } while( bytesRead > 0 && bytesToRead > 0 );
	}

	return 0;
    }

}