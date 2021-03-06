/*********************************************************************/
/* Copyright (c) 2014, EPFL/Blue Brain Project                       */
/*                     Raphael Dumusc <raphael.dumusc@epfl.ch>       */
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

#ifndef MASTERWINDOW_H
#define MASTERWINDOW_H

#include "types.h"

#include <QMainWindow>
#include <QMimeData>

class BackgroundWidget;
class Configuration;
class DisplayGroupGraphicsView;
class WebbrowserWidget;

/**
 * The main UI window for Master applications.
 *
 * It lets users control the contents displayed on the wall.
 */
class MasterWindow : public QMainWindow
{
    Q_OBJECT

public:
    /** Constructor. */
    MasterWindow(DisplayGroupPtr displayGroup, Configuration& config);

    /** Destructor. */
    ~MasterWindow();

    /** Get the GraphicsView used for touch interaction. */
    DisplayGroupGraphicsView* getGraphicsView();

    /** Get the display options that change during runtime. */
    OptionsPtr getOptions() const;

signals:
    /** Emitted when users want to open a dock. */
    void openDock(QPointF pos);

    /** Emitted when users want to hide the dock. */
    void hideDock();

    /** Emitted when users want to open a webbrowser. */
    void openWebBrowser(QPointF pos, QSize size, QString url);

protected:
    /** @name Drag events re-implemented from QMainWindow */
    //@{
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    //@}

private slots:
    void openContent();
    void openContentsDirectory();

    void saveState();
    void loadState();

    void computeImagePyramid();

    void openAboutWidget();

private:
    void setupMasterWindowUI();

    void addContentDirectory(const QString &directoryName, unsigned int gridX=0, unsigned int gridY=0);
    void loadState(const QString &filename);

    void estimateGridSize(unsigned int numElem, unsigned int& gridX, unsigned int& gridY);

    QStringList extractValidContentUrls(const QMimeData* mimeData);
    QStringList extractFolderUrls(const QMimeData *mimeData);
    QString extractStateFile(const QMimeData *mimeData);

    DisplayGroupPtr displayGroup_;
    OptionsPtr options_;
    BackgroundWidget* backgroundWidget_;
    WebbrowserWidget* webbrowserWidget_;
    DisplayGroupGraphicsView* dggv_;
};

#endif // MASTERWINDOW_H
