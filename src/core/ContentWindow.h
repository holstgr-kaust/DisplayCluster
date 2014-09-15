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

#ifndef CONTENT_WINDOW_H
#define CONTENT_WINDOW_H

#include "Event.h"
#include "types.h"

#include "serializationHelpers.h"
#include "Content.h" // needed for serialization

#include <QObject>
#include <QUuid>
#include <QRectF>

#ifndef Q_MOC_RUN
// https://bugreports.qt.nokia.com/browse/QTBUG-22829: When Qt moc runs on CGAL
// files, do not process <boost/type_traits/has_operator.hpp>
#  include <boost/shared_ptr.hpp>
#  include <boost/weak_ptr.hpp>
#  include <boost/date_time/posix_time/posix_time.hpp>
#endif
#include <boost/enable_shared_from_this.hpp>
#include <boost/serialization/weak_ptr.hpp>
#include <boost/date_time/posix_time/time_serialize.hpp>

class EventReceiver;

enum ControlState
{
    STATE_PAUSED = 1 << 0,
    STATE_LOOP   = 1 << 1
};

enum SizeState
{
    SIZE_1TO1,
    SIZE_FULLSCREEN,
    SIZE_NORMALIZED
};

class ContentInteractionDelegate;
using dc::Event;

/**
 * A window for placing a Content on the Wall.
 *
 * Can be serialized and distributed to the Wall applications.
 */
class ContentWindow : public QObject,
        public boost::enable_shared_from_this< ContentWindow >
{
    Q_OBJECT

public:
    /** The possible states of the window. */
    enum WindowState
    {
        UNSELECTED, // not selected, interaction modifies position/size
        SELECTED    // selected, interaction goes to ContentInteractionDelegate
    };

    /** No-argument constructor required for serialization. */
    ContentWindow();

    /**
     * Create a new window.
     * @param content The Content for this window.
     * @note Rank0 only.
     */
    ContentWindow( ContentPtr content );

    /** Destructor. */
    virtual ~ContentWindow();

    /** Get the content. */
    ContentPtr getContent() const;

    /** Set the content, replacing the existing one. @note Rank0 only. */
    void setContent( ContentPtr content );

    /** Get the parent DisplayGroup of this window. */
    DisplayGroupPtr getDisplayGroup() const;

    /** Set a reference on the parent DisplayGroup of this window. */
    void setDisplayGroup( DisplayGroupPtr displayGroup );

    /** @return the unique identifier for this window. */
    const QUuid& getID() const;

    /** Get the ContentWindowManger associated to this object if it has one, otherwise returns 0. */
    ContentWindowPtr getContentWindow();

    /** Get content dimensions in pixels. */
    void getContentDimensions( int &contentWidth, int &contentHeight );

    /** Get the normalized window coordiates. */
    void getCoordinates( double &x, double &y, double &w, double &h );

    /** Get the normalized window coordiates. */
    QRectF getCoordinates() const;

    /** Get the normalized position. */
    void getPosition( double &x, double &y );

    /** Get the normalized size. */
    void getSize( double &w, double &h );

    /** Get the normalized center position. */
    void getCenter( double &centerX, double &centerY );

    /** Get the zoom factor [1;inf]. */
    double getZoom();

    /** Get the current size state. */
    SizeState getSizeState() const;

    /** Set the control state. */
    void setControlState( const ControlState state ) { controlState_ = state; }

    /** Get the control state. */
    ControlState getControlState() const { return controlState_; }

    /** Get the last event for this window. */
    Event getEvent() const;

    /** Toggle the window state. */
    void toggleWindowState();

    /** Toggle between fullscreen and 'normalized' by keeping the position
     *  and size after leaving fullscreen */
    void toggleFullscreen();

    /** Get the window state. */
    ContentWindow::WindowState getWindowState();

    /** Is the window selected. */
    bool selected() const { return windowState_ == SELECTED; }

    /** Register an object to receive this window's Events. */
    bool registerEventReceiver( EventReceiver* receiver );

    /** Does this window already have registered Event receiver(s) */
    bool hasEventReceivers() const { return eventReceiversCount_ > 0; }

    /**
     * Get the interaction delegate.
     * @see createInteractionDelegate()
     * @note Rank0 only.
     */
    ContentInteractionDelegate& getInteractionDelegate() const;

    /**
     * Create a delegate to handle user interaction through dc::Events.
     * The type of delegate created depends on the ContentType.
     * @note Rank0 only.
     */
    void createInteractionDelegate();

    /** Get the position of the window center. */
    QPointF getWindowCenterPosition() const;

    /**
     * Move the window to a new position.
     * @param position The position for the window center
     * @param constrainToWindowBorders If true, the new position will be
     *        adjusted so that the window does not exceed the screen boundaries.
     */
    void centerPositionAround( const QPointF& position,
                               const bool constrainToWindowBorders );

    void setPosition( const double x, const double y );
    void setSize( const double w, const double h );
    void setCoordinates( const QRectF coordinates );
    void scaleSize( const double factor );
    void adjustSize( const SizeState state );

    void setCenter( double centerX, double centerY );
    void setZoom( const double zoom );

    void setWindowState( const ContentWindow::WindowState state );
    void setEvent( const Event event );

    void moveToFront();
    void close();

public slots:
    void setContentDimensions( int contentWidth, int contentHeight );

signals:
    /** Emitted when the Content signals that it has been modified. */
    void contentModified();

    /** Emitted just before the dimensions are going to change. */
    void dimensionsAboutToChange();

    // Used by the DisplayGroup to monitor changes
    void contentDimensionsChanged( int contentWidth, int contentHeight );
    void coordinatesChanged( QRectF coordinates );
    void positionChanged( double x, double y );
    void sizeChanged( double w, double h );
    void centerChanged( double centerX, double centerY );
    void zoomChanged( double zoom );
    void windowStateChanged( ContentWindow::WindowState windowState );
    void eventChanged( Event event );
    void closed();

private:
    void fixAspectRatio();
    void setEventToNewDimensions();

    friend class boost::serialization::access;

    /** Serialize for sending to Wall applications. */
    template< class Archive >
    void serialize( Archive & ar, const unsigned int )
    {
        ar & content_;
        ar & displayGroup_;
        ar & contentWidth_;
        ar & contentHeight_;
        ar & coordinates_;
        ar & centerX_;
        ar & centerY_;
        ar & zoom_;
        ar & controlState_;
        ar & windowState_;
    }

    /** Serialize for saving to an xml file */
    template< class Archive >
    void serialize_for_xml( Archive & ar, const unsigned int )
    {
        ar & boost::serialization::make_nvp( "content", content_ );
        ar & boost::serialization::make_nvp( "contentWidth", contentWidth_ );
        ar & boost::serialization::make_nvp( "contentHeight", contentHeight_ );
        ar & boost::serialization::make_nvp( "coordinates", coordinates_ );
        ar & boost::serialization::make_nvp( "coordinatesBackup", coordinatesBackup_ );
        ar & boost::serialization::make_nvp( "centerX", centerX_ );
        ar & boost::serialization::make_nvp( "centerY", centerY_ );
        ar & boost::serialization::make_nvp( "zoom", zoom_ );
        ar & boost::serialization::make_nvp( "controlState", controlState_ );
        ar & boost::serialization::make_nvp( "windowState", windowState_ );
    }

    const QUuid uuid_;

    // content dimensions in pixels
    // TODO remove those (DISCL-231)
    int contentWidth_;
    int contentHeight_;

    // normalized window coordinates
    QRectF coordinates_;
    QRectF coordinatesBackup_;

    // panning and zooming
    double centerX_;
    double centerY_;

    double zoom_;

    ContentWindow::WindowState windowState_;
    Event latestEvent_;
    SizeState sizeState_;
    ControlState controlState_;
    unsigned int eventReceiversCount_;

    ContentPtr content_;

    boost::weak_ptr< DisplayGroup > displayGroup_;

    boost::scoped_ptr< ContentInteractionDelegate > interactionDelegate_;
};

DECLARE_SERIALIZE_FOR_XML( ContentWindow )

#endif
