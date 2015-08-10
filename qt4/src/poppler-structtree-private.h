/* poppler-structtree-private.h: qt interface to poppler
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

#ifndef POPPLER_STRUCTTREE_PRIVATE_H
#define POPPLER_STRUCTTREE_PRIVATE_H

class StructElement;
class StructTreeRoot;

namespace Poppler
{
  class StructTreeItem
  {
    public:
    StructTreeItem( StructElement *structElement, StructTreeItem *parent = 0 );
    StructTreeItem( StructTreeRoot *structTreeContent);

    QList<StructTreeItem*> childList() { return m_children; }
    StructTreeItem* parent() { return m_parent; }
    QString name() const { return m_name; }
    void setName( QString newName ) { m_name = newName; }
    StructElement* structElement() { return m_structElement;}
    bool fetchMore() { return m_fetchMore; }
    void setFetchMore( bool fetchMore ) { m_fetchMore = fetchMore; }

    private:
    QList<StructTreeItem*> m_children;
    StructTreeItem *m_parent;
    QString m_name;
    StructElement * m_structElement;
    bool m_fetchMore;
  };

  class StructTreeModelPrivate
  {
    public:
    StructTreeModelPrivate(StructTreeModel *qq, StructTreeRoot *structTreeContent);

    StructTreeItem *nodeFromIndex(const QModelIndex &index, bool canBeNull = false) const;
    QModelIndex indexFromItem(StructTreeItem *node, int column) const;

    private:
    StructTreeItem *m_rootNode;
    StructTreeModel *q;
  };

}
#endif