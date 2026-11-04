#include "searchthread.h"

#include <QDir>
#include <QDirIterator>
#include <qDebug>

#include "fileoptimizer.h"
#include "../../common/tool/utils.h"


//template<typename T>
//std::vector<T> create_copy(std::vector<T> const& vec)
//{
//    std::vector<T> v(vec);
//    return v;
//}

SearchThread::SearchThread(const QMap<FileOptimizeType, QStringList>& search_paths, bool ie_search, QObject *parent)
	: QThread(parent)
{
	
    _search_paths = search_paths;
    _search_paths.detach();


    _iesearch = ie_search;
	//_search_paths = create_copy(search_paths);
}

SearchThread::~SearchThread()
{
    if (isRunning()) {
        quit();
        wait(); // 스레드가 종료될 때까지 대기
    }

    _search_paths.clear();
}


void SearchThread::run()
{
    size_t allcount = 0;
    size_t allsize = 0;

    qDebug() << "SearchThread::run() Start ";

    for (auto key : _search_paths.keys()) {

        size_t typecount = 0;
        size_t typesize = 0;

        for (auto& path : _search_paths.value(key))
        {
            QDirIterator it(path, QDir::Files | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
            while (it.hasNext()) {

                //it.next();
                if (it.next().isEmpty())
                    continue;

                QFileInfo fileInfo = it.fileInfo();
                QString aa = fileInfo.fileName();

                qDebug() << "scan : "<< aa ;

                if (fileInfo.fileName().indexOf(".etl") > 0) 
                    continue;

                if (fileInfo.fileName().indexOf(".etlx") > 0)
                    continue;

                if (fileInfo.fileName().indexOf(".evtx") > 0)
                    continue;

                if (fileInfo.fileName().indexOf("TNI-") >= 0)
                    continue;

                if (fileInfo.fileName().indexOf("SCLog.log") >= 0)
                    continue;
                

                if (fileInfo.isReadable() && !fileInfo.isWritable()) {
                    continue;
                }

                QFile file(fileInfo.absoluteFilePath());
                if (!file.open(QFile::ReadWrite)) {
                    qDebug() << "!file.open : " << fileInfo.absoluteFilePath();
                    continue;
                }
                else 
                    file.close();

                QString filepath = fileInfo.absoluteFilePath();

                size_t size = 0;
                size = fileInfo.size();

                typecount++;
                typesize += size;

                if (_iesearch)
                    emit sendIefileinfo(key, filepath, size);
                else
                    emit sendfileinfo(key, filepath, size);
            }
        }

        if (_iesearch)
            emit searchFinishedbyIEType(key, typecount, typesize);
        else
            emit searchFinishedbyType(key, typecount, typesize);

        allcount += typecount;
        allsize += typesize;
    }

    qDebug() << "SearchThread::run() END";

    if (_iesearch)
        emit searchFinishedIE(allcount, allsize);
    else
        emit searchFinished(allcount, allsize);

}



//void SearchThread::run()
//{
//    size_t allcount = 0;
//    size_t allsize = 0;
//
//    for (auto key : _search_paths.keys()) {
//
//        size_t typecount = 0;
//        size_t typesize = 0;
//
//        for (auto& path : _search_paths.value(key))
//        {
//            QDirIterator it(path, QDir::AllEntries | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
//            while (it.hasNext()) {
//
//                it.next();
//                
//                QFileInfo fileInfo = it.fileInfo();
//                //if (!fileInfo.isWritable())
//                //    continue; // 파일이 삭제 가능하지 않으면 skip
//
//                QString filepath = fileInfo.absoluteFilePath();
//
//                qDebug() << "filepath : " << filepath;
//
//                size_t size = 0;
//                if (fileInfo.isDir())
//                    size = utils::getFolderAllSize(filepath);
//                else
//                    size = fileInfo.size();
//                //qDebug() << "size : " << size ;
//
//
//                typecount++;
//                typesize += size;
//
//                if (_iesearch)
//                    emit sendIefileinfo(key, filepath, size);
//                else
//                    emit sendfileinfo(key, filepath, size);
//            }
//        }
//
//        if(_iesearch)
//            emit searchFinishedbyIEType(key, typecount, typesize);
//        else
//            emit searchFinishedbyType(key, typecount, typesize);
//
//        allcount += typecount;
//        allsize += typesize;
//	}
//
//    if (_iesearch)
//        emit searchFinishedIE(allcount, allsize);
//    else
//        emit searchFinished(allcount, allsize);
//}
