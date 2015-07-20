#include <QMenu>
#include <QSettings>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QDebug>

#include "filedealer.hpp"
#include "log/log.hpp"

FileDealer::FileDealer( QObject* parent ) :
    QObject( parent ) {

    mRecentFileActions.reserve( MaxRecentFiles );

    for( int i = 0; i < MaxRecentFiles; i++ ) {
        mRecentFileActions.push_back( 0 );
    }

    setType( ImagesType );
}

void FileDealer::setType( FileType type ) {
    mType = type;

    switch( mType ) {
        case ImagesType: {
            mLastDirectory = "lastDirectoryImages";
            mSaveTitle = tr( "Save image" );
            mOpenTitle = tr( "Open image" );
            mIOSelection = tr( "Images (*.jpg *.jpeg *.JPG *.JPEG *.png *.PNG)" );
            mSettingsRecent = "recentImages";
            mDefaultSuffix = "jpg";
            mPossibleSuffixes << "jpg" << "jpeg" << "png";
        }
        break;

        default:
            qWarning() << "No type set";
    }
}

bool FileDealer::knows( const QString& suffix ) const {
    bool ok = mPossibleSuffixes.contains( suffix, Qt::CaseInsensitive );
    return ok;
}

QString FileDealer::getFilename() const {
    return mFilename;
}

void FileDealer::setFilename( const QString& filename ) {
    mFilename = filename;
    setCurrentFile();
}

std::pair<bool, QString> FileDealer::getSaveFilename() {
    QSettings settings;
    QString dir = settings.value( mLastDirectory ).toString();

    if( dir.isEmpty() ) {
        dir = QDir::homePath();
    }

    QString filename = QFileDialog::getSaveFileName( 0, mSaveTitle, dir, mIOSelection );

    if( filename.isEmpty() ) {
        return std::make_pair( false, QString() );
    }

    mFilename = filename;

    QFileInfo info( mFilename );

    if( ! mPossibleSuffixes.contains( info.suffix(), Qt::CaseInsensitive ) ) {
        mFilename += "." + mDefaultSuffix;
    }

    return std::make_pair( true, mFilename );
}

std::pair<bool, QString> FileDealer::getOpenFilename() {

    bool ok = true;

    QSettings settings;
    QString dir = settings.value( mLastDirectory ).toString();

    if( dir.isEmpty() ) {
        dir = QDir::homePath();
    }

    QString filename = QFileDialog::getOpenFileName( 0, mOpenTitle, dir, mIOSelection );

    if( filename.isEmpty() ) {
        ok = false;
    } else {
        mFilename = filename;
        setCurrentFile();
    }

    return std::make_pair( ok, mFilename );
}

//! \brief Setup the menu under File -> Recent
//! \see void MainWindow::setupRecentMenu( )
void FileDealer::setupRecentMenu( QMenu* recent ) {

    for( int i = 0; i < MaxRecentFiles; ++i ) {
        mRecentFileActions[i] = new QAction( this );
        mRecentFileActions[i]->setVisible( false );
        CHECK_QT_CONNECT( connect( mRecentFileActions[i], &QAction::triggered, this, &FileDealer::openRecent ) );
    }

    this->updateRecentFileActions();

    recent->addActions( mRecentFileActions );
}

//! \brief Update GUI state according to mFilename
//! \see void MainWindow::openImage( const QString& filename )
//! \arg open, true, if recent menu is updated from opening a file
void FileDealer::setCurrentFile() {
    QSettings settings;

    /* set last opened directory */
    QFileInfo info( mFilename );
    QStringList files = settings.value( mSettingsRecent ).toStringList();

    if( info.exists() ) {
        settings.setValue( mLastDirectory, info.absolutePath() );
        files.removeAll( mFilename );
        files.prepend( mFilename );
    }

    while( files.size() > MaxRecentFiles ) {
        files.removeLast();
    }

    settings.setValue( mSettingsRecent, files );

    this->updateRecentFileActions();
}

//! \brief Refreshs the Recent File Actions in menu and settings
//! \arg open, true, if recent menu is updated from opening a file
void FileDealer::updateRecentFileActions() {
    QList<Qt::Key> numKeys;
    numKeys << Qt::Key_1
            << Qt::Key_2
            << Qt::Key_3
            << Qt::Key_4
            << Qt::Key_5;

    QSettings settings;
    QStringList files = settings.value( mSettingsRecent ).toStringList();

    int numRecentFiles = qMin( files.size(), ( int )MaxRecentFiles );

    int actionNo = 0;

    foreach( QAction* action, mRecentFileActions ) {
        action->setVisible( false );
    }

    for( int i = 0; i < numRecentFiles; ++i ) {
        QFileInfo info( files[i] );

        if( info.exists() ) {
            mRecentFileActions[i]->setText( info.fileName() );
            mRecentFileActions[i]->setData( files[i] );
            mRecentFileActions[i]->setVisible( true );

            if( mType == ImagesType ) {
#if __APPLE__
                mRecentFileActions[i]->setShortcut( Qt::CTRL + numKeys[actionNo] );
#else
                mRecentFileActions[i]->setShortcut( Qt::ALT + numKeys[actionNo] );
#endif // __APPLE__
            }

            actionNo++;
            mRecentFileActions[i]->setToolTip( info.absoluteFilePath() );
            mRecentFileActions[i]->setStatusTip( info.absoluteFilePath() );
        }
    }
}

//! \brief Slot called by mRecentFileActions
void FileDealer::openRecent() {
    QAction* action = qobject_cast<QAction*>( sender() );

    if( action ) {
        mFilename = action->data().toString();
        setCurrentFile();

        /* received by  */
        emit signalOpen( mFilename );
    }
}
