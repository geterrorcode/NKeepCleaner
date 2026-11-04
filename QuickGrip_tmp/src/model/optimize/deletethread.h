#pragma once

#include <QObject>
#include <QThread>
#include <vector>
#include <QMap>

#include "optimizdata.h"

class DeleteThread  :  public QThread
{
	Q_OBJECT

public:	
	DeleteThread(QObject *parent = Q_NULLPTR);
	~DeleteThread();

	void run() override;

	void setDelFileInfo(const QVector<fileOptimizeData> fileinfo, const QVector<FileOptimizeType> optimizeTypeState, bool iefile = false);
	void setDelRegInfo(const QMap<int, regOptimizeData> reginfo);


private:
	OptimizeType _opttype;
	QVector<fileOptimizeData> _fileinfos;
	QMap<int, regOptimizeData> _reginfos;
	QVector<FileOptimizeType> _optimizeUseState;

	void deleteSubFolders(FileOptimizeType type);
	QStringList getTargetFolder(FileOptimizeType type);

	void deleteFiles();
	void deleteIEFiles();
	void deleteRegs();

	bool isOptimizeType(FileOptimizeType type) const;
signals:
	void snedDeleteFile(fileOptimizeData data, bool delsuccess);
	void snedDeleteFileComplete();

	void snedDeleteIEFile(fileOptimizeData data, bool delsuccess);
	void snedDeleteIEFileComplete();

	void snedDeleteReg(regOptimizeData data, bool delsuccess);
	void snedDeleteRegComplete();

	void snedReg(QString path, RegOptimizeType type);

	
};
