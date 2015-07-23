/* poppler-structtreeroot.cc: qt interface to poppler
 *
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

#include "poppler-structtree.h"

#include "poppler-structtree-private.h"

#include "poppler-private.h"

#include <QtCore/QDebug>
#include <QtCore/QtAlgorithms>
#include <QtCore/QModelIndex>

#include "poppler/StructTreeRoot.h"

class QModelIndex;

namespace Poppler
{

  StructTreeItem::StructTreeItem(StructElement *structElement, StructTreeItem * parent)
    : m_parent( parent )
  {
    int numOfChildren = structElement->getNumChildren();
    StructElement *child;
    if( structElement->getTypeName() )
	m_name = QString(structElement->getTypeName());
    for(int i = 0; i < numOfChildren; i ++)
    {
	child = structElement->getChild( i );
	m_children.insert( i, new StructTreeItem( child, this ) );
    }

  }

  StructTreeItem::StructTreeItem(StructTreeRoot *structTreeContent)
  {
    unsigned numOfChildren = structTreeContent->getNumChildren();
    StructElement *child;
    for( int i = 0; i < numOfChildren; i ++ )
    {
      child = structTreeContent->getChild( i );
      m_children.insert( i, new StructTreeItem( child, this ) );
    }

    m_parent = 0;
  }

  StructTreeModelPrivate::StructTreeModelPrivate(StructTreeModel *qq, StructTreeRoot *structTreeContent) : q( qq )
  {
    m_rootNode = new StructTreeItem( structTreeContent );
  }

  StructTreeItem* StructTreeModelPrivate::nodeFromIndex( const QModelIndex &index, bool canBeNull) const
  {
    if (index.isValid()) {
      return static_cast<StructTreeItem *>(index.internalPointer());
    } else {
      return canBeNull ? 0 : m_rootNode;
    }
  }

  QModelIndex StructTreeModelPrivate::indexFromItem(StructTreeItem * node, int column) const
  {
    if (!node) {
      return QModelIndex();
    }
    StructTreeItem *parentNode = node->parent();
    if (!parentNode) {
      return QModelIndex();
    }
    const int row = parentNode->childList().indexOf(node);
    return q->createIndex(row, column, node);
  }

  StructTreeModel::StructTreeModel( StructTreeRoot *structTreeContent, QObject *parent )
    : QAbstractItemModel(parent)
  {
    d = new StructTreeModelPrivate( this, structTreeContent );
  }

  QModelIndex StructTreeModel::index(int row, int column, const QModelIndex &parent) const
  {
    if (row < 0 || column != 0) {
      return QModelIndex();
    }

    StructTreeItem *parentNode = d->nodeFromIndex( parent, false );
    if (row < parentNode->childList().count()) {
      return createIndex(row, column, parentNode->childList().at(row));
    }
    return QModelIndex();
  }

  QModelIndex StructTreeModel::parent(const QModelIndex &child) const
  {
    StructTreeItem *childNode = d->nodeFromIndex( child, false );
    if (!childNode) {
      return QModelIndex();
    }
    return d->indexFromItem(childNode->parent(), child.column());
  }

  int StructTreeModel::rowCount(const QModelIndex &parent) const
  {
    StructTreeItem *parentNode = d->nodeFromIndex( parent, false );
    if (!parentNode) {
      return 0;
    } else {
      return parentNode->childList().count();
    }
  }

  int StructTreeModel::columnCount(const QModelIndex &parent) const
  {
    return 1;
  }


  QVariant StructTreeModel::data(const QModelIndex &index, int role) const
  {
    StructTreeItem *node = d->nodeFromIndex(index, true);
    if (!node) {
      return QVariant();
    }

    switch (role) {
      case Qt::DisplayRole:
        return node->name();
        break;
    }

    return QVariant();
  }

  Qt::ItemFlags StructTreeModel::flags ( const QModelIndex & index ) const
  {
    StructTreeItem *node = d->nodeFromIndex(index, false);
    Qt::ItemFlags itemFlags = Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    return itemFlags;
  }

  QVariant StructTreeModel::headerData( int section, Qt::Orientation orientation, int role ) const
  {
    return QAbstractItemModel::headerData( section, orientation, role );
  }
}

#include "poppler-structtree.moc"
