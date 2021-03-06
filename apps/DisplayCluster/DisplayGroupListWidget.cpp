/*********************************************************************/
/* Copyright (c) 2011 - 2012, The University of Texas at Austin.     */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/*   1. Redistributions of source code must retain the above         */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer.                                                  */
/*                                                                   */
/*   2. Redistributions in binary form must reproduce the above      */
/*      copyright notice, this list of conditions and the following  */
/*      disclaimer in the documentation and/or other materials       */
/*      provided with the distribution.                              */
/*                                                                   */
/*    THIS  SOFTWARE IS PROVIDED  BY THE  UNIVERSITY OF  TEXAS AT    */
/*    AUSTIN  ``AS IS''  AND ANY  EXPRESS OR  IMPLIED WARRANTIES,    */
/*    INCLUDING, BUT  NOT LIMITED  TO, THE IMPLIED  WARRANTIES OF    */
/*    MERCHANTABILITY  AND FITNESS FOR  A PARTICULAR  PURPOSE ARE    */
/*    DISCLAIMED.  IN  NO EVENT SHALL THE UNIVERSITY  OF TEXAS AT    */
/*    AUSTIN OR CONTRIBUTORS BE  LIABLE FOR ANY DIRECT, INDIRECT,    */
/*    INCIDENTAL,  SPECIAL, EXEMPLARY,  OR  CONSEQUENTIAL DAMAGES    */
/*    (INCLUDING, BUT  NOT LIMITED TO,  PROCUREMENT OF SUBSTITUTE    */
/*    GOODS  OR  SERVICES; LOSS  OF  USE,  DATA,  OR PROFITS;  OR    */
/*    BUSINESS INTERRUPTION) HOWEVER CAUSED  AND ON ANY THEORY OF    */
/*    LIABILITY, WHETHER  IN CONTRACT, STRICT  LIABILITY, OR TORT    */
/*    (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY WAY OUT    */
/*    OF  THE  USE OF  THIS  SOFTWARE,  EVEN  IF ADVISED  OF  THE    */
/*    POSSIBILITY OF SUCH DAMAGE.                                    */
/*                                                                   */
/* The views and conclusions contained in the software and           */
/* documentation are those of the authors and should not be          */
/* interpreted as representing official policies, either expressed   */
/* or implied, of The University of Texas at Austin.                 */
/*********************************************************************/

#include "DisplayGroupListWidget.h"

#include "DisplayGroup.h"
#include "ContentWindow.h"
#include "Content.h"
#include "ContentWindowListWidgetItem.h"

#include <QtGui/QListWidget>

DisplayGroupListWidget::DisplayGroupListWidget( QWidget* parent_ )
    : QListWidget( parent_ )
{
    connect( this, SIGNAL( itemClicked( QListWidgetItem* )),
             this, SLOT( moveListWidgetItemToFront( QListWidgetItem* )));
}

void DisplayGroupListWidget::setModel( DisplayGroupPtr displayGroup )
{
    if( displayGroup_ )
        displayGroup_->disconnect( this );
    clear();

    displayGroup_ = displayGroup;
    if( !displayGroup_ )
        return;

    connect( displayGroup_.get(), SIGNAL( contentWindowAdded( ContentWindowPtr )),
             this, SLOT( addContentWindow( ContentWindowPtr )));
    connect( displayGroup_.get(), SIGNAL( contentWindowRemoved( ContentWindowPtr )),
            this, SLOT(removeContentWindow(ContentWindowPtr)));
    connect( displayGroup_.get(), SIGNAL( contentWindowMovedToFront( ContentWindowPtr )),
             this, SLOT( moveContentWindowToFront( ContentWindowPtr )));
}

void DisplayGroupListWidget::addContentWindow( ContentWindowPtr contentWindow )
{
    ContentWindowListWidgetItem* newItem = new ContentWindowListWidgetItem( contentWindow );
    newItem->setText( contentWindow->getContent()->getURI( ));
    insertItem( 0, newItem );
}

void DisplayGroupListWidget::removeContentWindow( ContentWindowPtr contentWindow )
{
    for( int i = 0; i < count(); ++i )
    {
        QListWidgetItem* listWidgetItem = item( i );
        ContentWindowListWidgetItem* windowItem = dynamic_cast< ContentWindowListWidgetItem* >( listWidgetItem );
        if( windowItem && windowItem->getContentWindow() == contentWindow )
        {
            takeItem( i );
            delete listWidgetItem;
            return;
        }
    }
}

void DisplayGroupListWidget::moveContentWindowToFront( ContentWindowPtr contentWindow )
{
    for( int i = 0; i < count(); ++i )
    {
        QListWidgetItem* listWidgetItem = item( i );
        ContentWindowListWidgetItem* windowItem = dynamic_cast< ContentWindowListWidgetItem* >( listWidgetItem );
        if( windowItem && windowItem->getContentWindow() == contentWindow )
        {
            takeItem( i );
            insertItem( 0, listWidgetItem );
            return;
        }
    }
}

void DisplayGroupListWidget::moveListWidgetItemToFront( QListWidgetItem* listWidgetItem )
{
    ContentWindowListWidgetItem* windowItem = dynamic_cast< ContentWindowListWidgetItem* >( listWidgetItem );
    if( windowItem )
        displayGroup_->moveContentWindowToFront( windowItem->getContentWindow( ));
}
