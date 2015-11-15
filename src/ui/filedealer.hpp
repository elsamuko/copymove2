#pragma once

#include <QObject>
#include <QAction>

#include <utility>

//! \brief IO Helper class, manages the filename and recent file actions
class FileDealer : public QObject {
        Q_OBJECT
    public:
        enum FileType { ImagesType, DataType };

        explicit FileDealer( QObject* parent = 0 );

        QString getFilename() const;
        void setFilename( const QString& filename );
        std::pair<bool, QString> getSaveFilename();
        std::pair<bool, QString> getOpenFilename();
        void setupRecentMenu( QMenu* recent );
        void updateRecentFileActions();
        void setType( FileType type );
        bool knows( const QString& suffix ) const;

    signals:
        void signalOpen( QString );

    public slots:
        void openRecent();

    private:
        QString mFilename;

        QString mLastDirectory;
        QString mSaveTitle;
        QString mOpenTitle;
        QString mIOSelection;
        QString mSettingsRecent;
        QString mDefaultSuffix;
        QStringList mPossibleSuffixes;

        enum { MaxRecentFiles = 5 };

        FileType mType;
        QList<QAction*> mRecentFileActions;
        void setCurrentFile();
};
