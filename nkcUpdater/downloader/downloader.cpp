#include "downloader.h"

#include <QApplication>
#include <QJsonDocument>
#include <QDir>
#include <QFileInfo>
#include <QThread>
#include <QMessageBox>

#include "../QUX/include/utility/variant.h"
//#include <QUX/include/utility/variant.h>
#include "../QUX/include/async/waitforsignal.h"
//#include <Qux/include/async/waitforsignal.h>

#include "../Network/loadsync.h"
#include "../Tool/downdata.h"
#include "../Tool/filecopyer.h"
#include "Logger.h"


#include "../define/errordefine.h"
//#include "env/env.h"

DownLoader::DownLoader(QObject *parent /*= nullptr*/)
	: QObject(parent)
{
}

DownLoader::~DownLoader()
{
}

bool DownLoader::getDownVersionInfo()
{
	// CDN 정보 다운로드
	//f"hash"
	//QString versionUrl = QString("%1/Launcher/buildversion.ini").arg(env().cdnHost);
	//https://download.tnilab.com/files/updater.json
	QString versionUrl;
	if (QFile::exists(QDir::currentPath() + QDir::separator() + "dev.txt")) { //dev
		versionUrl = QString("https://download.tnilab.com/dev/files/updater.json");
	}
	else
		versionUrl = QString("https://download.tnilab.com/files/updater.json");

	//QString versionUrl = QString("https://download.tnilab.com/files/updater.json");

	_versionData = loadSync(QUrl(versionUrl));

	if (_versionData.isEmpty())
		return false;


	return true;
}


bool DownLoader::deleteOldFile()
{
	//PLOG_INFO << "File Clear";

	auto map = QJsonDocument::fromJson(_versionData).toVariant().toMap();
	//auto files = Qux::fromQVariantList<QVariantMap>(map["patchDataList"].toList());
	auto files = Qux::fromQVariantList<QVariantMap>(map["items"].toList());

	//QString folder = qApp->applicationDirPath();
	//QString workFolder = QDir().currentPath();
	//QString downFolder = QDir().absoluteFilePath("Temp");
	QString workFolder = qApp->applicationDirPath();
	//QString downFolder = QDir().absoluteFilePath("Temp");
	QString downFolder = QString("%1/Temp").arg(qApp->applicationDirPath()); // QDir().absoluteFilePath("Temp");

	QDir dir(downFolder);

	if (dir.count() > 0)
	{
		PLOG_INFO << "Remove Temp Folder";
		dir.removeRecursively();
	}

	for (auto g : files)
	{
		auto fileName = g["Name"].toString();
		auto fileFolder = g["sub"].toString();



		
		QString reNameFileName = fileName.section(".", 0, -2);
		QString reNameExe = fileName.section(".", -1);

		QString deleteFileName = QString("%1_.%2").arg(reNameFileName).arg(reNameExe);

		QString filePath = fileFolder.isEmpty() == true ?
			deleteFileName : QString("%1\\%2").arg(fileFolder).arg(deleteFileName);


		QString deleteFilePath = QString("%1\\%2").arg(workFolder).arg(filePath);
		deleteFilePath = QDir::toNativeSeparators(deleteFilePath);

		QFile deleteFile(deleteFilePath);
		if (deleteFile.exists() == true)
		{
			QFile::remove(deleteFilePath);
		}

	}


	return true;
}

int DownLoader::getVersion()
{
	auto versionMap = QJsonDocument::fromJson(
		_versionData).toVariant().toMap();

	int ver = versionMap["version"].toInt();
	return ver;
}

void DownLoader::update()
{

	bool success = false;
	auto map = QJsonDocument::fromJson(_versionData).toVariant().toMap();
	auto downs = Qux::fromQVariantList<QVariantMap>(map["items"].toList());

	QString workFolder_old = QDir().currentPath();
	QString workFolder = qApp->applicationDirPath();
	if (downs.size() <= 0)
	{
		emit signalError(DOWNLOAD_ERROR::ERROR_METAFILE, -1, "Meta File Down Error", "buildversion.ini");
		return;
	}


	
	struct Files {
		QString downPath;
		QString orgPath;
		QString reNamePath;
	};

	QList<Files> downFile;

	{
		auto local = new QThread;
		auto worker = new DownData(local);
		int downCount = 0;
		bool downSuccess = false;

		QObject::connect(worker, &DownData::signalComplate, [&downCount, &downSuccess](int count) {
			downCount = count;

			PLOG_INFO << "File Download Complate" << LOG_TAB()
					<< "DownCount [" << downCount << "]" << LOG_TAB();
			//qDebug() << "Down Success - " << "DownCount : " << downCount;

			downSuccess = true;
		});

		QObject::connect(worker, &DownData::signalError, [this, &downSuccess](int errorCode, QString errorMsg, QString errorFileName) {
			downSuccess = false;
			//PLOG_ERROR << "File Down Error" << LOG_TAB()
			//			<< "ErrorCode [" << errorCode << "]" << LOG_TAB()
			//			<< "ErrorString : " << errorMsg;
			
			QString errorText = QString("File Down Error [%1] %2 (%3) ").arg(errorCode).arg(errorMsg).arg(errorFileName);
			qDebug() << errorText;
			PLOG_ERROR << errorText;

			
			emit signalError(DOWNLOAD_ERROR::ERROR_DOWNLOAD, errorCode, errorMsg, errorFileName);
		});	

		QObject::connect(worker, &DownData::signalComplateOneFile, [&downFile](QString endFilePath, QString orgFilePath) {
			QString reNameFile = orgFilePath.section(".", 0, -2);
			QString exe = orgFilePath.section(".", -1);
			QString reNamePath = reNameFile + "_." + exe;

			QString text = QString("Down complete [%1] %2 (%3) ").arg(endFilePath).arg(orgFilePath);
			PLOG_INFO << text;

			downFile.append({ endFilePath, orgFilePath, reNamePath });
		});

		QObject::connect(worker, &DownData::signalProgress, [=](qint64 bytesReceived, qint64 bytesTotal) {

			emit siganlProgress(bytesReceived, bytesTotal);
		});

		worker->setDownInfo(downs, workFolder);

		if (worker->isDownFiles() == true)
		{
			local->start();
			Qux::Async::waitForSignal(worker, &DownData::signalComplate);
		}

		 

		// 다운로드 받을 파일이 없다면 성공으로 처리
		int downSize = worker->getDownFileSize();
		PLOG_INFO << "DownLoad Files [" << downSize << "]";

		if (downSize == 0)
		{
			PLOG_INFO << "Update File Size = 0 : Complate";
			emit signalComplate();
			return;
		}

		PLOG_INFO << "DownLoad End"
			<< " Queue Down Count : " << downSize << LOG_TAB()
			<< " File Down Count : " << downFile.size() << LOG_TAB();

		if (downSize != downFile.size())
		{
			PLOG_ERROR << "Down File Count Error";

			downSuccess = false;
			QString downFolder = QDir().absoluteFilePath("Temp");

			QDir dir(downFolder);

			if (dir.count() > 0)
				dir.removeRecursively();


			emit signalError(DOWNLOAD_ERROR::ERROR_DOWNLOAD, -1, "DownLoad File Count Error", "");
		}

		if (downSuccess == false)
		{
			PLOG_INFO << "File Down Error";
			return;
		}
	}

	
		
	{
		for (auto g : downFile)
		{
			QString reNameFile = g.reNamePath;
			QString orgFile = g.orgPath;

			if (QFileInfo(orgFile).exists() == true)
			{
				if (QFile::rename(orgFile, reNameFile) == false)
				{
					PLOG_ERROR << "Down Complate File Rename [_]" << LOG_TAB()
						<< "Error File [" << orgFile << "]" << LOG_TAB()
						<< "Rename File : " << reNameFile;
				}
			}
		}

	}


	{
		auto local = new QThread;
		auto worker = new FileCopyer(local);
		QString errorCopyFile;

		QObject::connect(worker, &FileCopyer::finished, this, [&success](bool s) {
			success = s;
			s ? qDebug() << "FINISHED" : qDebug() << "FAILED";
			/*PLOG_INFO << "File Copy Finish" << LOG_TAB()
				<< "success [" << success << "]";*/

		});

		QObject::connect(worker, &FileCopyer::oneFinished, this, [=](const QString dstPath, bool success) {
			if (success == false)
			{
				worker->interrupt();
				qDebug() << "File Copy Error (One File) ";
				//PLOG_ERROR << "File Copy Error (One File) " << LOG_TAB()
				//	<< "File Name : " << dstPath;
			}
		});

	

		QVector<QString> src;
		QVector<QString> dst;

		for (auto g : downFile)
		{
			src.append(g.downPath);
			dst.append(g.orgPath);
		}
		
		worker->setSourcePaths(src);
		worker->setDestinationPaths(dst);
		local->start();

		Qux::Async::waitForSignal(worker, &FileCopyer::finished);

		//PLOG_INFO << "FInal";
		
		// 파일 복구
		for (auto g : downFile)
		{
			QString reNameFile = g.reNamePath;
			QString orgFile = g.orgPath;

			if (success == true)
			{

				if (QFileInfo(reNameFile).exists() == true)
				{
					if (QFile::remove(reNameFile) == false)
					{
						//PLOG_WARNING << "[" << success << "]" << LOG_TAB()
						//	<< "Remove File : " << reNameFile;
					}
				}
			}
			else
			{
				if (QFileInfo(orgFile).exists() == true)
				{
					if (QFile::remove(orgFile) == false)
					{
						qDebug() << "remove Success : " << reNameFile;


						/*PLOG_ERROR << "[" << success << "]" << LOG_TAB()
							<< "Remove File : " << reNameFile;*/
					}

				}

				if (QFile::rename(reNameFile, orgFile) == false)
				{
					qDebug() << "rename Success : " << reNameFile;

					//PLOG_ERROR << "[" << success << "]" << LOG_TAB()
					//	<< "Rename File : " << reNameFile << LOG_TAB()
					//	<< "Org File : " << orgFile;

				}

			}
			
		}


		if (success == true)
		{
			emit signalComplate();
		}
		else
		{
			emit signalError(DOWNLOAD_ERROR::ERROR_FILECOPY, -1, "File Copy Error", "");
		}


	}

}
