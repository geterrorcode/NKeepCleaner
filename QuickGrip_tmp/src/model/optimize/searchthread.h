#pragma once

#include <QObject>
#include <QThread>

#include <vector>
#include <Qmap>
#include <Qvector>

#include "optimizdata.h"

class SearchThread  : public QThread
{
	Q_OBJECT

public:
	//SearchThread(const std::vector<QString>& search_paths,  QObject *parent = Q_NULLPTR);
	SearchThread(const QMap<FileOptimizeType, QStringList>& search_paths, bool ie_search = false, QObject* parent = Q_NULLPTR);
	~SearchThread();
	void run() override;

private:
	//std::vector<QString> _search_paths;

	QMap<FileOptimizeType, QStringList> _search_paths;
	bool _iesearch;

signals:
	void sendfileinfo(FileOptimizeType type, QString path, size_t size);
	void sendIefileinfo(FileOptimizeType type, QString path, size_t size);

	void logUpdated(const std::string& message);

	void searchFinished(size_t allcount, size_t allsize);
	void searchFinishedIE(size_t allcount, size_t allsize);
	
	void searchFinishedbyType(FileOptimizeType type, size_t allcount, size_t allsize);
	void searchFinishedbyIEType(FileOptimizeType type, size_t allcount, size_t allsize);

	//void groupSearchFinished(const std::string& group, const std::vector<std::string>& logMessages, const size_t& size);
	//void totalProgressUpdated(int progress);


};
