#include "systrayxstatusnotifier.h"

#if defined( Q_OS_UNIX ) && defined( KDE_INTEGRATION )

/*
 *	Local includes
 */
#include "preferences.h"

/*
 *  System includes
 */
#include "systrayxlink.h"

/*
 *	Qt includes
 */
#include <QIcon>
#include <QTimer>
#include <QPixmap>
#include <QPainter>


/*
 *	Constructor
 */
SysTrayXStatusNotifier::SysTrayXStatusNotifier( SysTrayXLink* link, Preferences* pref, QObject* parent )
    : KStatusNotifierItem( parent )
{
    /*
     *  Initialize
     */
    m_link = link;
    m_pref = pref;

    m_unread_mail = 0;

    m_show_number = m_pref->getShowNumber();
    m_number_color = m_pref->getNumberColor();
    m_number_size = m_pref->getNumberSize();
    m_number_alignment = Qt::AlignHCenter | Qt::AlignVCenter;
    setNumberAlignment( m_pref->getNumberAlignment() );
    m_number_margins = m_pref->getNumberMargins();

    /*
     * Setup notifier
     */
    setCategory( KStatusNotifierItem::ApplicationStatus );

    setIconByPixmap( QIcon( QPixmap( ":/files/icons/Thunderbird.png") ) );
    setTitle("SysTray-X");

    setStatus( KStatusNotifierItem::ItemStatus::Passive );
    m_hide_default_icon = true;

//  setStatus(KStatusNotifierItem::ItemStatus::Active);
//  setStatus(KStatusNotifierItem::ItemStatus::NeedsAttention);

    connect( this, &KStatusNotifierItem::activateRequested, this, &SysTrayXStatusNotifier::slotActivateRequested );
    connect( this, &KStatusNotifierItem::secondaryActivateRequested, this, &SysTrayXStatusNotifier::slotSecondaryActivateRequested );
}


/*
 *  Set the default icon type
 */
void    SysTrayXStatusNotifier::setDefaultIconType( Preferences::DefaultIconType icon_type )
{
    if( m_default_icon_type != icon_type )
    {
        /*
         *  Store the new value
         */
        m_default_icon_type = icon_type;

        /*
         *  Render and set a new icon in the tray
         */
        renderIcon();
    }
}


/*
 *  Set the default icon mime
 */
void    SysTrayXStatusNotifier::setDefaultIconMime( const QString& icon_mime )
{
    if( m_default_icon_mime != icon_mime )
    {
        /*
         *  Store the new value
         */
        m_default_icon_mime = icon_mime;
    }
}


/*
 *  Set the default icon data
 */
void    SysTrayXStatusNotifier::setDefaultIconData( const QByteArray& icon_data )
{
    if( m_default_icon_data != icon_data )
    {
        /*
         *  Store the new value
         */
        m_default_icon_data = icon_data;

        /*
         *  Render and set a new icon in the tray
         */
        renderIcon();
    }
}


/*
 *  Set the hide default icon
 */
void    SysTrayXStatusNotifier::setHideDefaultIcon( bool hide )
{
    if( m_hide_default_icon != hide )
    {
        /*
         *  Store the new value
         */
        m_hide_default_icon = hide;
    }
}


/*
 *  Set the icon type
 */
void    SysTrayXStatusNotifier::setIconType( Preferences::IconType icon_type )
{
    if( icon_type != m_icon_type )
    {
        /*
         *  Store the new value
         */
        m_icon_type = icon_type;

        /*
         *  Render and set a new icon in the tray
         */
        renderIcon();
    }
}


/*
 *  Set the icon mime
 */
void    SysTrayXStatusNotifier::setIconMime( const QString& icon_mime )
{
    if( m_icon_mime != icon_mime )
    {
        /*
         *  Store the new value
         */
        m_icon_mime = icon_mime;
    }
}


/*
 *  Set the icon data
 */
void    SysTrayXStatusNotifier::setIconData( const QByteArray& icon_data )
{
    if( m_icon_data != icon_data )
    {
        /*
         *  Store the new value
         */
        m_icon_data = icon_data;

        /*
         *  Render and set a new icon in the tray
         */
        renderIcon();
    }
}


/*
 *  Enable/disable number
 */
void    SysTrayXStatusNotifier::showNumber( bool state )
{
    if( m_show_number != state )
    {
        /*
         *  Store the new value
         */
        m_show_number = state;

        /*
         *  Render and set a new icon in the tray
         */
        renderIcon();
    }
}


/*
 *  Set number color
 */
void    SysTrayXStatusNotifier::setNumberColor( const QString& color )
{
    if( m_number_color != color )
    {
        /*
         *  Store the new value
         */
        m_number_color = color;

        /*
         *  Render and set a new icon in the tray
         */
        renderIcon();
    }
}


/*
 *  Set number size
 */
void    SysTrayXStatusNotifier::setNumberSize( int size )
{
    if( m_number_size != size )
    {
        /*
         *  Store the new value
         */
        m_number_size = size;

        /*
         *  Render and set a new icon in the tray
         */
        renderIcon();
    }
}


/*
 *  Set number alignment
 */
void    SysTrayXStatusNotifier::setNumberAlignment( int alignment )
{
    int alignment_qt;
    switch( alignment )
    {
        case 0: alignment_qt = Qt::AlignTop | Qt::AlignLeft; break;
        case 1: alignment_qt = Qt::AlignTop | Qt::AlignHCenter; break;
        case 2: alignment_qt = Qt::AlignTop | Qt::AlignRight; break;

        case 3: alignment_qt = Qt::AlignVCenter | Qt::AlignLeft; break;
        case 4: alignment_qt = Qt::AlignVCenter | Qt::AlignHCenter; break;
        case 5: alignment_qt = Qt::AlignVCenter | Qt::AlignRight; break;

        case 6: alignment_qt = Qt::AlignBottom | Qt::AlignLeft; break;
        case 7: alignment_qt = Qt::AlignBottom | Qt::AlignHCenter; break;
        case 8: alignment_qt = Qt::AlignBottom | Qt::AlignRight; break;

        default:
        {
            alignment_qt = Qt::AlignHCenter | Qt::AlignVCenter;
            break;
        }
    }

    if( m_number_alignment != alignment_qt )
    {
        /*
         *  Store the new value
         */
        m_number_alignment = alignment_qt;

        /*
         *  Render and set a new icon in the tray
         */
        renderIcon();
    }
}


/*
 *  Set number alignment
 */
void    SysTrayXStatusNotifier::setNumberMargins( QMargins margins )
{
    if( m_number_margins != margins )
    {
        /*
         *  Store the new value
         */
        m_number_margins = margins;

        /*
         *  Render and set a new icon in the tray
         */
        renderIcon();
    }
}


/*
 *  Set the number of unread mails
 */
void    SysTrayXStatusNotifier::setUnreadMail( int unread_mail )
{
    if( m_unread_mail != unread_mail )
    {
        /*
         *  Store the new value
         */
        m_unread_mail = unread_mail;

        /*
         *  Render and set a new icon in the tray
         */
        renderIcon();
    }
}


/*
 *  Set and render the icon in the system tray
 */
void    SysTrayXStatusNotifier::renderIcon()
{
    QPixmap pixmap;

    if( m_unread_mail > 0 )
    {
        switch( m_icon_type )
        {
            case Preferences::PREF_BLANK_ICON:
            {
                Preferences::Theme theme = m_pref->getTheme();

                if( theme == Preferences::PREF_THEME_LIGHT )
                {
                    pixmap = QPixmap( ":/files/icons/blank-icon.png" );
                }
                else
                {
                    pixmap = QPixmap( ":/files/icons/blank-icon-dark.png" );
                }
                break;
            }

            case Preferences::PREF_NEWMAIL_ICON:
            {
                QIcon new_mail = QIcon::fromTheme("mail-unread", QIcon(":/files/icons/mail-unread.png"));
                pixmap = new_mail.pixmap( 256, 256 );
                break;
            }

            case Preferences::PREF_CUSTOM_ICON:
            {
                pixmap.loadFromData( m_icon_data );
                break;
            }

            case Preferences::PREF_NO_ICON:
            {
                QPixmap lookthrough( 256, 256 );
                lookthrough.fill( Qt::transparent );
                pixmap = lookthrough;
                break;
            }

            case Preferences::PREF_TB_ICON:
            {
                pixmap = QPixmap( ":/files/icons/Thunderbird.png" );
                break;
            }
        }
    }
    else
    {
        switch( m_default_icon_type )
        {
            case Preferences::PREF_DEFAULT_ICON_DEFAULT:
            {
                pixmap = QPixmap( ":/files/icons/Thunderbird.png" );
                break;
            }

            case Preferences::PREF_DEFAULT_ICON_HIDE:
            {
                pixmap = QPixmap();
                break;
            }

            case Preferences::PREF_DEFAULT_ICON_CUSTOM:
            {
                pixmap.loadFromData( m_default_icon_data );
                break;
            }
        }
    }

    if( m_show_number && ( m_unread_mail > 0 ) )
    {
        /*
         *  Paint the number
         */
        QPainter painter( &pixmap );

        painter.setFont( QFont("Sans") );

#if QT_VERSION < QT_VERSION_CHECK(5, 13, 0)
        double factor = pixmap.width() / ( 3 * painter.fontMetrics().width( "0" ) );
#else
        double factor = pixmap.width() / ( 3 * painter.fontMetrics().horizontalAdvance( "0" ) );
#endif
        QFont font = painter.font();
        font.setPointSizeF( font.pointSizeF() * ( factor * m_number_size / 10 ) );
        font.setBold( true );
        painter.setFont( font );
        painter.setPen( QColor( m_number_color ) );

        QRect bounding = pixmap.rect().adjusted( m_number_margins.left(), m_number_margins.top(),
                                                 -m_number_margins.right(), -m_number_margins.bottom());

        painter.drawText( bounding, m_number_alignment, QString::number( m_unread_mail ) );
    }

    /*
     *  Set the tray icon
     */
    setIconByPixmap( QIcon( pixmap ) );

    /*
     *  Hide the icon?
     */
    if( m_hide_default_icon && m_unread_mail == 0 )
    {
        setStatus( KStatusNotifierItem::ItemStatus::Passive );
    }
    else
    {
        QTimer::singleShot(500, this, &SysTrayXStatusNotifier::showIcon);
    }
}


void    SysTrayXStatusNotifier::showIcon()
{
    if( !m_hide_default_icon || m_unread_mail > 0 )
    {
        setStatus( KStatusNotifierItem::ItemStatus::Active );
    }
}


/*
 *  Handle unread mail signal
 */
void    SysTrayXStatusNotifier::slotSetUnreadMail( int unread_mail )
{
    setUnreadMail( unread_mail );
}


/*
 *  Handle the default icon type change signal
 */
void    SysTrayXStatusNotifier::slotDefaultIconTypeChange()
{
    setDefaultIconType( m_pref->getDefaultIconType() );
}


/*
 *  Handle the default icon data change signal
 */
void    SysTrayXStatusNotifier::slotDefaultIconDataChange()
{
    setDefaultIconMime( m_pref->getDefaultIconMime() );
    setDefaultIconData( m_pref->getDefaultIconData() );
}


/*
 *  Handle the hide default icon change signal
 */
void    SysTrayXStatusNotifier::slotHideDefaultIconChange()
{
    setHideDefaultIcon( m_pref->getHideDefaultIcon() );
}


/*
 *  Handle the icon type change signal
 */
void    SysTrayXStatusNotifier::slotIconTypeChange()
{
    setIconType( m_pref->getIconType() );
}


/*
 *  Handle the icon data change signal
 */
void    SysTrayXStatusNotifier::slotIconDataChange()
{
    setIconMime( m_pref->getIconMime() );
    setIconData( m_pref->getIconData() );
}


/*
 *  Handle the enable number state change signal
 */
void    SysTrayXStatusNotifier::slotShowNumberChange()
{
    showNumber( m_pref->getShowNumber() );
}


/*
 *  Handle the number color change signal
 */
void    SysTrayXStatusNotifier::slotNumberColorChange()
{
    setNumberColor( m_pref->getNumberColor() );
}


/*
 *  Handle the number size change signal
 */
void    SysTrayXStatusNotifier::slotNumberSizeChange()
{
    setNumberSize( m_pref->getNumberSize() );
}


/*
 *  Handle the number alignment change signal
 */
void    SysTrayXStatusNotifier::slotNumberAlignmentChange()
{
    setNumberAlignment( m_pref->getNumberAlignment() );
}


/*
 *  Handle the number margins change signal
 */
void    SysTrayXStatusNotifier::slotNumberMarginsChange()
{
    setNumberMargins( m_pref->getNumberMargins() );
}


/*
 *  Handle the theme change signal
 */
void    SysTrayXStatusNotifier::slotThemeChange()
{
    renderIcon();
}


/*
 *  Handle activate request of the notification icon
 */
void    SysTrayXStatusNotifier::slotActivateRequested( bool active, const QPoint &pos )
{
    Q_UNUSED( active )
    Q_UNUSED( pos )

    emit signalShowHide();
}


/*
 *  Handle secondary activate request of the notification icon
 */
void    SysTrayXStatusNotifier::slotSecondaryActivateRequested( const QPoint &pos )
{
    Q_UNUSED( pos )

    emit signalShowHide();
}

#endif
