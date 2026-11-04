#include "downdata.h"

#include <QApplication>
#include <QScopedPointer>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QThread>

#include "../QUX/include/utility/variant.h"
//#include <QUX/include/utility/variant.h>
#include "../QUX/include/utility/qvalue.h"
//#include <Qux/include/utility/qvalue.h>
#include "../QUX/include/async/waitforsignal.h"
//#include <Qux/include/async/waitforsignal.h>

#include "../Network/loader.h"
//#include "env/env.h"
#include "Logger.h"

DownData::DownData(QThread* _thread)
	: QObject(nullptr)
{
	moveToThread(_thread);
	QObject::connect(_thread, &QThread::started, this, &DownData::slotFindDownList);
	QObject::connect(this, &DownData::signalError, _thread, &QThread::quit);
	QObject::connect(this, &DownData::signalComplate, _thread, &QThread::quit);

	QObject::connect(_thread, &QThread::finished, _thread, &QThread::deleteLater);
}

void DownData::setDownInfo(const QList<QVariantMap>& versions, const QString& workDir)
{
	_workDir = workDir;
	getDownLoadList(versions);


}

void DownData::getDownLoadList(const QList<QVariantMap>& versions)
{


	for (auto g : versions)
	{
		auto fileName = g["Name"].toString();
		auto hash = g["MD5"].toString();
		auto folder = g["sub"].toString();
		auto size = g["size"].toLongLong();

		qint64 receiveSize = 0;

		// 다운로드 경로
		QString downPath = folder.isEmpty() == true ?
			fileName : QString("%1/%2").arg(folder).arg(fileName);

		//PLOG_INFO << "DownloadPath : " << downPath;

		if (!folder.isEmpty()) {
			qDebug() << folder;
			//PLOG_INFO << "Folder Empty : " << folder;
		}

		// 원본 폴더
		auto orgFilePath = _workDir + QDir::separator() + downPath;
		orgFilePath = QDir::toNativeSeparators(orgFilePath);

		// 다운로드 폴더
		auto downFilePath = _workDir + QDir::separator() + "temp" + QDir::separator() + fileName;
		downFilePath = QDir::toNativeSeparators(downFilePath);

		//QString url = QString("%1/Launcher/build/%2").arg(env().cdnHost).arg(downPath);
		//https://download.tnilab.com/files/updater.json

		QString url = "";
		if (QFile::exists(QDir::currentPath() + QDir::separator() + "dev.txt")) { //dev
			url = QString("https://download.tnilab.com/dev/files/build/%1").arg(downPath);
		}
		else
			url = QString("https://download.tnilab.com/files/build/%1").arg(downPath);


		//PLOG_INFO << "Downloadurl : " << url;

		// 다운받을 항목인지 체크 
		bool isdown = true;
		QFile orgfile(orgFilePath);
		if (orgfile.open(QFile::ReadOnly))
		{
			// HASH 체크필요
			QCryptographicHash cryptohash(QCryptographicHash::Md5);
			if (cryptohash.addData(&orgfile))
			{
				QString fileHash = QString::fromStdString(cryptohash.result().toHex().toStdString()).toUpper();

				if (fileHash == hash)
					isdown = false;
			}

			orgfile.close();

		}

		if (isdown == false)
			continue;

		_downList.append({ size ,
						orgFilePath ,
						downFilePath,
						url });

		//qDebug() << "downFile" << orgFilePath;
		PLOG_INFO << "downFile : " << orgFilePath << " , URL : " << url;
	}

}

bool DownData::isDownFiles()
{
	return _downList.size() > 0 ? true : false;
}

int DownData::getDownFileSize()
{
	return _downList.size();
}

void DownData::slotFindDownList()
{
	auto loaderPtr = Qux::createQValue<QScopedPointer<Loader>>(this);
	auto saveFile = Qux::createQValue<QScopedPointer<QFile>>(this);

	static qint64 totalSize = 0;
	static qint64 recvSize = 0;

	int downCnt = 0;
	for (auto g : _downList)
	{
		totalSize += g.size;
	}

	//QString downFolder = QDir().absoluteFilePath("Temp");
	QString downFolder = QString("%1/Temp").arg(qApp->applicationDirPath()); //QDir().absoluteFilePath("Temp");

	if (QDir(downFolder).exists() == false)
		QDir().mkpath(downFolder);



	for (auto g : _downList)
	{
		QString filePath = g.downFilePath;
		QString orgFilePath = g.orgFilePath;
		QString url = g.url;
		qint64 size = g.size;


		// 파일 
		saveFile->reset(new QFile(filePath));

		if (saveFile->data()->exists() == true)
		{
			qDebug() << "already file removeFile : " << filePath;
			saveFile->data()->remove();
		}


		if (!saveFile->data()->open(QIODevice::ReadWrite | QIODevice::Append)) {
			qWarning() << QStringLiteral("failed to open %1 (error:%2)").arg(saveFile->data()->fileName()).arg(saveFile->data()->errorString());
			continue; // skip
		}



		qDebug() << url;
		loaderPtr->reset(new Loader(url));

		connect(loaderPtr->data(), &Loader::completed, [=](QByteArray bytes) {
			if (saveFile->data()->size() == size)
			{
				saveFile->data()->close();
				emit signalComplateOneFile(filePath, orgFilePath);
			}

			});

		connect(loaderPtr->data(), &Loader::progress, this, [=](qint64 bytesReceived, qint64 bytesTotal) {
			auto receiveFile = loaderPtr->data()->reply()->readAll();
			saveFile->data()->write(receiveFile);
			auto size = receiveFile.size();
			recvSize += size;


			emit signalProgress(recvSize, totalSize);
			});


		connect(loaderPtr->data(), &Loader::error, [=](QNetworkReply::NetworkError code) {
			qDebug() << code;
			auto path = saveFile->data()->fileName();

			QFileInfo file(path);
			auto fileName = file.fileName();
			saveFile->data()->close();

			QString errorString = loaderPtr->data()->reply()->errorString();

			PLOG_INFO << "down error : " << path << " , errorString : " << errorString;

			emit signalError(code, errorString, fileName);
		});

		Qux::Async::waitForSignal(loaderPtr->data(), &Loader::completed, &Loader::error);

	}


	emit signalComplate(downCnt);
}


