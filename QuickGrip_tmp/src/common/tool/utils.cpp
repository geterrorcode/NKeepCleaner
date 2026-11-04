#include "utils.h"

#include <windows.h>
#include <QtCore>
#include <iostream>
#include <netcon.h>
#include <sstream>
#include <stdlib.h>
#include <winternl.h>
#include <tlhelp32.h>
#include <wininet.h>
#include <psapi.h>
#include <tchar.h>
#include <ShlObj.h>
#include <shlobj.h>         // REFKNOWNFOLDERID, SHGetKnownFolderPath
#include <KnownFolders.h>   // FOLDERID_Documents
#include <string> 
#include <QFile>
#include <QSysInfo>
#include <QSettings>
#include <QStorageInfo>
#include <QDirIterator>
#include <QStandardPaths>
#include <QProcess>
#include <QProcessEnvironment>

#include <QByteArray>

#include <QCryptographicHash>
#include "qaes/qaesencryption.h"

namespace utils {


	QIcon getIconFromFile(const QString filePath)
	{
		auto info = QFileInfo(filePath);
		QFileIconProvider fileicon;
		QIcon icon = fileicon.icon(info);
		if (icon.isNull()) {
			QFileIconProvider defaulticon;
			icon = defaulticon.icon(QFileIconProvider::Folder);
		}
			
		return icon;
	}

	QString getFileSizeType(float size)
	{
		if (size == 0)
			return "0 MB";

		QStringList list;
		list << "KB" << "MB" << "GB" << "TB";

		QStringListIterator i(list);
		QString unit("B");

		while (size >= 1024.0 && i.hasNext())
		{
			unit = i.next();
			size /= 1024.0;
		}

		return QString().setNum(size, 'f', 2) + unit;
	}

	qint64 getFolderAllSize(QString folderPath)
	{
		if (folderPath.isEmpty())
			return 0;

		if (folderPath.right(1) != "\\")
			folderPath.append("\\");

		qint64 size = 0;
		QDir dir(folderPath);

		QDir::Filters fileFilters = QDir::Files | QDir::System | QDir::Hidden;
		for (QString filePath : dir.entryList(fileFilters)) {
			QFileInfo fi(dir, filePath);
			size += fi.size();
		}

		QDir::Filters dirFilters = QDir::Dirs | QDir::NoDotAndDotDot | QDir::System | QDir::Hidden;
		for (QString childDirPath : dir.entryList(dirFilters))
			size += getFolderAllSize(folderPath + QDir::separator() + childDirPath);
		return size;
	}

	QString getFolderSizeString(QString folderPath)
	{
		return getFileSizeType(getFolderAllSize(folderPath));
	}



	std::string getPlatform()
	{
		SYSTEM_INFO sysinfo;
		GetNativeSystemInfo(&sysinfo);
		if (sysinfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
			return std::string("32bit");
		else if (sysinfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
			return std::string("64bit");
		else
			return std::string("");
	}

	std::string getOSVersion()
	{
		std::string os;
		std::ostringstream ds;
		NTSTATUS(WINAPI * RtlGetVersion)(LPOSVERSIONINFOEXW);
		OSVERSIONINFOEXW osInfo;

		*reinterpret_cast<FARPROC*>(&RtlGetVersion) = GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion");

		if (nullptr != RtlGetVersion)
		{
			osInfo.dwOSVersionInfoSize = sizeof(osInfo);
			RtlGetVersion(&osInfo);

			std::stringstream ss;
			ss << "Windows "
				<< osInfo.dwMajorVersion
				<< "(" << osInfo.dwMajorVersion << "." << osInfo.dwMinorVersion << ")"
				<< ", " << osInfo.dwMajorVersion << "." << osInfo.dwMinorVersion << "." << osInfo.dwBuildNumber
				<< ", " << getPlatform();

			return ss.str();
		}

		return std::string("UnKnown");
	}


	bool networkControl(bool connect)
	{
		HRESULT hr = E_FAIL;
		hr = CoInitialize(NULL);

		INetConnectionManager* pNetConnectionManager = NULL;
		hr = CoCreateInstance(CLSID_ConnectionManager, NULL, CLSCTX_LOCAL_SERVER | CLSCTX_NO_CODE_DOWNLOAD,
			IID_INetConnectionManager, reinterpret_cast<LPVOID*>(&pNetConnectionManager));

		if (FAILED(hr))
		{
			CoUninitialize();
			return false;
		}

		IEnumNetConnection* pEnumNetConnection;
		hr = pNetConnectionManager->EnumConnections(NCME_DEFAULT, &pEnumNetConnection);
		if (FAILED(hr))
		{
			pNetConnectionManager->Release();
			CoUninitialize();
			//MWI_LOG->log_error(LOG_MARK, "INetConnectionManager Emum error");
			return false;
		}

		ULONG ulCount = 0;
		BOOL fFound = FALSE;
		hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND); //기본값

		HRESULT hrT = S_OK;
		do
		{
			NETCON_PROPERTIES* pProps = NULL;
			INetConnection* pConn;

			hrT = pEnumNetConnection->Next(1, &pConn, &ulCount);
			if (SUCCEEDED(hrT) && 1 == ulCount)
			{
				hrT = pConn->GetProperties(&pProps);
				if (S_OK == hrT)
				{
					if (connect)
					{
						hr = pConn->Connect();
					}
					else
					{
						hr = pConn->Disconnect();
					}
					CoTaskMemFree(pProps->pszwName);
					CoTaskMemFree(pProps->pszwDeviceName);
					CoTaskMemFree(pProps);
				}

				pConn->Release();
				pConn = NULL;
			}
		} while (SUCCEEDED(hrT) && 1 == ulCount && !fFound);

		if (FAILED(hrT))
			hr = hrT;

		pEnumNetConnection->Release();
		pNetConnectionManager->Release();
		CoUninitialize();

		if (FAILED(hr) && hr != HRESULT_FROM_WIN32(ERROR_RETRY))
			return false;

		return true;
	}


	size_t getAllFileCount(const QString folderpath)
	{
		QDir dir(folderpath);
		dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
		return dir.count();
	}

	size_t getAllFileCountSubfolder(const QString folderpath)
	{
		CoInitialize(NULL);
		size_t count = 0;
		QDirIterator it(folderpath, QDir::Files |  QDir::NoDotAndDotDot/*| QDir::Dirs*/, QDirIterator::Subdirectories);
		while (it.hasNext()) {
			if (!it.next().isEmpty()) {
				count++;
			}
		}
		CoUninitialize();
		return count;
	}

	size_t getAllFileCountSubfolder_exp(const QString folderpath, const QStringList exeptions)
	{
		size_t count = 0;
		QDirIterator it(folderpath, QDir::Files | QDir::NoDotAndDotDot/*| QDir::Dirs*/, QDirIterator::Subdirectories);
		while (it.hasNext()) {
			if (it.next().isEmpty())
				continue;

			QString filename = it.fileName();
			for (auto data : exeptions)
			{
				if (filename.indexOf(data) > 0)
					continue;
			}

			//it.fileName

				count++;
		}
		return count;
	}

	QString getRecycleBinPath()
	{
		QString recycleBinPath = "";

		LPWSTR path;
		WCHAR* filepath;
		WCHAR* binfilepath;

		if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &filepath))) {
			return recycleBinPath;
		}

		if (!SUCCEEDED(SHGetKnownFolderPath(FOLDERID_RecycleBinFolder, 0, NULL, &binfilepath))) {
			DWORD dw =  GetLastError();
			return recycleBinPath;
		}

		recycleBinPath = QString("%1").arg(path);
		CoTaskMemFree(path);


		return recycleBinPath;

	}

	std::wstring getKnownFolderPath()
	{
		std::wstring result;
		wchar_t* path = nullptr;
		const auto hr = SHGetKnownFolderPath(FOLDERID_SyncResultsFolder, 0, nullptr, &path);
		if (SUCCEEDED(hr)) {
			result = std::wstring(path);
		}
		if (path) { CoTaskMemFree(path); }
		return result;
	}

	QString getFileMakeDate(const QString filePath)
	{
		QFileInfo fileinfo(filePath);
		if(!fileinfo.exists())
			return "-1";

		QString date = QStringLiteral("%1%2%3").arg(fileinfo.birthTime().date().year())
			.arg(fileinfo.birthTime().date().month(), 2, 10, QLatin1Char('0'))
			.arg(fileinfo.birthTime().date().day(), 2, 10, QLatin1Char('0'));

		return date;
	}

	QString getAvaliableHardSpace()
	{
		QStorageInfo storage = QStorageInfo::root();
		return getFileSizeType(storage.bytesAvailable());
	}

	QString getUserAppPath()
	{
		QString appLocalPath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);
		return appLocalPath.left(appLocalPath.lastIndexOf("/") + 1);
	}

	QString getWindowsPath()
	{
		QString windowsPath;
		QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
		windowsPath = env.value("SystemRoot");
		windowsPath.replace("\\", "/");
		windowsPath += "/";

		return windowsPath;
	}

	QString toRichText(const QString& txt, const int lineHeight)
	{
		QString text(txt);
		text.replace("\n", "<br>");
		text = QString("<p style='line-height: %2px;'>%1</p>").arg(text, QString::number(lineHeight));

		return text;
	}


	void setStartup(const QString name, const QString path, bool use)
	{
		//컴퓨터\HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run
		//HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows\CurrentVersion\Run
		QString keypath = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run";
		QSettings settings(keypath, QSettings::NativeFormat);

		if (use) {
			QString value = settings.value(name).toString();
			if (value.isEmpty() || value != path) {
				settings.setValue(name, path);
			}
		}
		else {
			settings.remove(name);
		}

	}

	void setTaskScheduler(const QString name, const QString path, bool use)
	{
		QProcess process;
		QString program = "schtasks.exe";
		QStringList arguments;

		if (!use) {
			arguments << "/Change" << "/TN" << name << "/DISABLE";
			process.startDetached(program, arguments);
			process.waitForFinished();
			qDebug() << "Task state Enable.";
			return;
		}
		else { //use
			// Change the state of the task
			arguments << "/Change" << "/TN" << name << "/ENABLE";
			process.startDetached(program, arguments);
			process.waitForFinished();
			qDebug() << "Task state changed.";
		}

		//우선 on/off 만
		//return;

		arguments << "/Query" << "/TN" << name << "/FO LIST";

		//process.startDetached(program, arguments);
		process.start(program, arguments);
		process.waitForFinished();

		QString output(process.readAllStandardOutput());
		if (output == Q_NULLPTR || output.contains("ERROR: The system cannot find the file specified.")) {
			qDebug() << "Task not found. Creating the task.";

			// Clear the arguments list
			arguments.clear();
			//arguments << " /Create"
			//	<< " /SC ONLOGON"  // This will run the task at every logon
			//	<< QString(" /TN \"%1\"").arg(name)  // This is the name of your task
			//	<< QString(" /TR %1").arg(path)  // This is the path to your program
			//	<< " /RL HIGHEST"  // This will run the task with the highest privileges
			//	<< " /F";  // Force

			//process.start(program, arguments);
			//process.waitForFinished();
			//QString output(process.readAllStandardOutput());

			QString cmd = QString("schtasks.exe /Create /SC ONLOGON /TN %1 /TR %2 /RL HIGHEST /F").arg(name).arg(path);
			QByteArray ba = cmd.toLocal8Bit();
			system(ba.data());
			/*process.start(program, arguments);
			process.waitForFinished();
			QString create_output(process.readAllStandardOutput());*/
			qDebug() << "Task created.";
		}
		else {
			qDebug() << "Task found. Changing the state.";

			// Clear the arguments list
			arguments.clear();

			// Change the state of the task
			arguments << "/Change" << "/TN" << name << "/ENABLE";

			process.startDetached(program, arguments);
			process.waitForFinished();

			qDebug() << "Task state changed.";
		}

	}

	void pcShutDown()
	{
		//테스트는 빼고
		//QProcess::startDetached("shutdown -s -t 0");
		system("C:\\Windows\\System32\\shutdown /s /f /t 0");
	}



	QString aesEncryptor(QString str)
	{

		QString return_value("");
		QByteArray inputData = str.toUtf8();


		QAESEncryption encryption(QAESEncryption::AES_256, QAESEncryption::CBC);

		QString inputStr = str;
		QString key("FU29MOQOX8GYYDOFW2D6LMJ0DAHEKVLK");
		QString iv(key.mid(0, 16));

		QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
		QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

		QByteArray encodeText = encryption.encode(inputStr.toLocal8Bit(), hashKey, hashIV);
		

		return QString::fromUtf8(encodeText.toBase64());
	}

	QString aesDecryptor(QString str)
	{
		QString return_value("");

		QAESEncryption decryption(QAESEncryption::AES_256, QAESEncryption::CBC);

		QString inputStr = str;
		QString key("FU29MOQOX8GYYDOFW2D6LMJ0DAHEKVLK");
		QString iv(key.mid(0, 16));

		QByteArray hashKey = QCryptographicHash::hash(key.toLocal8Bit(), QCryptographicHash::Sha256);
		QByteArray hashIV = QCryptographicHash::hash(iv.toLocal8Bit(), QCryptographicHash::Md5);

		QByteArray sample = inputStr.toLocal8Bit();
		QByteArray data = QByteArray::fromBase64(sample);
		QByteArray decodeText = decryption.decode(data, hashKey, hashIV);

		//return_value = QString::fromUtf8(encodeText.toBase64());

		QString decryptedStr = QString::fromUtf8(decodeText);
		return_value = QString(decryption.removePadding(decodeText));



		return return_value;
	}

	QString getSystemUUID()
	{
		// Get machine UUID
		QByteArray machineUniqueId = QSysInfo::machineUniqueId();
		QString uuid = (QString)machineUniqueId;
		// Convert QByteArray to QString
		QString uuidString = QString::fromUtf8(machineUniqueId.toHex());
		return uuid;
	}


	
}






