#include "ssdefins.h"

namespace ssdefines {

	QString imageWidget = "QWidget { border-image: url(%1); background-color: transparent;}";

	QString imageButton = "QAbstractButton { background-color: rgba(255, 255, 255, 0); border-image: url(%1); background-repeat: no-repeat; } "
		" QAbstractButton:hover{ border-image: url(%2); } "
		" QAbstractButton:pressed{ border-image: url(%3); } "
		" QAbstractButton:disabled{ border-image: url(%4); }";

	QString imageToggleButton = "QAbstractButton { color: white; border-image: url(%1); background-repeat: no-repeat; } "
		" QAbstractButton:hover{ border-image: url(%2); } "
		" QAbstractButton:pressed{ border-image: url(%3); } "
		" QAbstractButton:disabled{ border-image: url(%4); }"
		" QAbstractButton:checked { border-image: url(%5); }"
		" QAbstractButton:checked:hover { border-image: url(%5); }"
		" QAbstractButton:checked:pressed{ border - image: url(%5); }";
	// 


	QString dropdown = "QComboBox{subcontrol - origin: padding; subcontrol - position: top right; selection - background - color: #111; selection - color: yellow;"
		"color: white; background-color: QLinearGradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 #565656, stop: 0.1 #525252, stop: 0.5 #4e4e4e, stop: 0.9 #4a4a4a, stop: 1 #464646);"
		"border-style: solid;		border: 1px solid #1e1e1e;		border-radius: 5;		padding: 1px 0px 1px 20px;}"
		"QComboBox:on{ padding-top: 0px;	padding-left: 4px;	color: black; background-color:white}"
		/*"QComboBox:on{ padding-top: 0px;	padding-left: 0px;	color: ; background-color: QLinearGradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 #2d2d2d, stop: 0.1 #2b2b2b, stop: 0.5 #292929, stop: 0.9 #282828, stop: 1 #252525); selection-background-color: #ffaa00;}"*/
		"QComboBox:!on { color: black;	background-color:white;	}"
		"QComboBox QAbstractItemView	{ border: 2px solid white; color: black; selection-background-color: QLinearGradient(x1 : 0, y1 : 0, x2 : 0, y2 : 1, stop : 0 #111, stop: 1 #333);	}"
		"QComboBox::down-arrow{ image:url(%1); }"
		"QComboBox::drop-down{ border: none;  padding:0px 0px 0px 0px; }";

	QString dropdownCustom = "QComboBox{ font: normal normal %1px; color:#666666; background:white; border-radius:4px; padding-left:6px; padding-right:6px;		background-image:url(%2); background-repeat:no-repeat;  background-position:right;} "
		" QComboBox:hover{	color:#666666; background-image:url(%3); background-repeat:no-repeat; background-position:right;}"
		" QComboBox:on{	border: 1px solid #d1d1d1;	color:#666666; background-image:url(%4); background-repeat:no-repeat; background-position:right;}"
		" QComboBox:disabled {	color:#bbbcbc; background-image:url(%5); background-repeat:no-repeat; background-position:right;}"
		" QComboBox::drop-down{	border:none; border-radius: 4px; }"
		" QComboBox QAbstractItemView {	background:transparent;	border: 1px solid #d1d1d1; border-bottom-right-radius:4px; border-bottom-left-radius:4px; font: normal normal %1px;	outline:none; min-height:30px;	}"
		" QComboBox QAbstractItemView::item	{ background:#ffffff; border:none; border-radius:4px; color:#666666; padding-left:3px; padding-right:3px;		min-height:30px; }"
		"QComboBox QAbstractItemView::item:selected	{background:#f4f5f7;color:#536c9e;}";


	QString checkbox = "QCheckBox { padding-left: 2px; border: none; background: transparent; font: normal normal 14px; color: #222222; }"
		" QCheckBox:hover{ color: #222222; }"
		" QCheckBox:pressed{ color: #222222; }"
		" QCheckBox::indicator{ width:15px; height:15px; }"
		" QCheckBox::indicator : unchecked{ image: url(%1); }"
		" QCheckBox::indicator : unchecked : hover{ image: url(%2); }"
		" QCheckBox::indicator : unchecked : pressed{ image: url(%3); }"
		" QCheckBox::indicator : checked{ image: url(%4); }"
		" QCheckBox::indicator : checked : hover{ image: url(%5); }"
		" QCheckBox::indicator : checked : pressed{ image: url(%6); }";


	QString checkboxFixImage = "QCheckBox { padding-left: 2px; border: none; background:transparent; font: normal normal 14px; color: #ffffff; }"
		" QCheckBox:hover{ color: #eeeeee; }"
		" QCheckBox:pressed{ color: #dddddd; }"
		" QCheckBox::indicator:checked { width:16px; height:16px; image:url(:/common/resource/common/checkbox_check.png); color:#999999; }"
		" QCheckBox::indicator:unchecked{ image: url(:/common/resource/common/checkbox.png); }";

	QString checkboxbigImage = "QCheckBox { padding-left: 2px; border: none; background:transparent; font: bold normal 20px; color: #ffffff; }"
		" QCheckBox:hover{ color: #eeeeee; }"
		" QCheckBox:pressed{ color: #dddddd; }"
		" QCheckBox::indicator:checked { width:16px; height:16px; image:url(:/common/resource/common/checkbox_check.png); color:#999999; }"
		" QCheckBox::indicator:unchecked{ image: url(:/common/resource/common/checkbox.png); }";


	QString checkboxbigImage_smallfont = "QCheckBox { padding-left: 2px; border: none; background:transparent; font: bold normal 18px; color: #ffffff; }"
		" QCheckBox:hover{ color: #eeeeee; }"
		" QCheckBox:pressed{ color: #dddddd; }"
		" QCheckBox::indicator:checked { width:16px; height:16px; image:url(:/common/resource/common/checkbox_check.png); color:#999999; }"
		" QCheckBox::indicator:unchecked{ image: url(:/common/resource/common/checkbox.png); }";

	//QString checkboxFixImage = "QCheckBox { padding-left: 2px; border: none; background:transparent; font: normal normal 14px; color: #ffffff; }"
	//	" QCheckBox:hover{ color: #eeeeee; }"
	//	" QCheckBox:pressed{ color: #dddddd; }"
	//	" QCheckBox::indicator{ width:16px; height:16px; image:url(:/common/resource/common/checkbox.png); color:#999999; }"
	//	" QCheckBox::indicator : unchecked{ image: url(:/common/resource/common/checkbox.png); }"
	//	" QCheckBox::indicator : unchecked : hover{ image: url(:/common/resource/common/checkbox.png); }"
	//	" QCheckBox::indicator : unchecked : pressed{ image: url(:/common/resource/common/checkbox.png); }"
	//	" QCheckBox::indicator : checked{ image:url(:/common/resource/common/checkbox_check.png);color:#cccccc;}"
	//	" QCheckBox::indicator : checked : hover{ image: url(:/common/resource/common/checkbox_check.png); }"
	//	" QCheckBox::indicator : checked : pressed{ image: url(:/common/resource/common/checkbox_check.png); }";


	//QString checkboxTable = "QCheckBox { margin-left:50%; margin-right:50%; }"
	QString checkboxTable = "QCheckBox { margin-right:0; }"
		//" QCheckBox:hover{ color: #eeeeee; }"
		//" QCheckBox:pressed{ color: #dddddd; }"
		" QCheckBox::indicator:checked { width:16px; height:16px; image:url(:/common/resource/common/checkbox_check.png); color:#999999; }"
		" QCheckBox::indicator:unchecked{ image: url(:/common/resource/common/checkbox.png); }";

	QString trayMenu = "QMenu { background-color:#1d2f61;	border:none; color:#687caf; font: normal normal 14px;}"
		" QMenu::right-arrow  { hide:true; }"
		" QMenu::separator	{ height:1px; background:#2f3643;	}"
		" QMenu::item	{ border:none; height:30px; padding: 1px 30px 1px 30px;}"
		" QMenu::item:selected 	{ color:#ffffff; font-weight:bold; }"
		" QMenu::item:disabled 	{ background:#f5f5f5; color:#9f9f9f; font-weight:bold; }";


	QString customlabel = "QLabel { padding-bottom: 1px; color: %2; font: normal normal %1; border=%3 solid #d9d9d9; }";
	QString customBoldlabel = "QLabel { padding-bottom: 1px; color: %2; font: bold normal %1; border=%3 ; }";


	QString editcoutrol = "QLineEdit{font: normal normal 24px; border-radius: 3px; border: 1px solid #cecece; padding:13px 5px 7px 10px; 	}"
		"QLineEdit:hover { font: normal normal 24px; border:none; border-bottom:1px solid rgba(255, 255, 255, 0.5); color:#000000; 	}"
		"QLineEdit:focus { font: normal normal 24px; border:none; border-bottom:1px solid #ff5000; 	color:#000000;	}";


	QString label = "QLabel{ padding - bottom: 1px; color: %2; font: normal normal %1; border = %3 solid #d9d9d9; }";
	QString boldLabel = "QLabel{ padding - bottom: 1px; color: %2; font: bold normal %1; border = %3; }";

	QString tablewidget = "QTableWidget { background-color: #282A2E; border: none;  }"
		" QTableWidget::item:selected{ background-color: #282A2E; color: #FFFFFF; }"
		//vertical scroll
		" QTableWidget QScrollBar:vertical{ border:none; background: #282A2E; width: 14px; margin: 28px 0 2px 0; border-radius: 7px; }"
		" QTableWidget QScrollBar::handle:vertical { background-color: #EEEEEE; min-height: 20px; border-radius: 7px;}"
		" QScrollBar::handle:vertical:hover {background-color: #FFFFFF; }"
		" QScrollBar::sub-line:vertical { border: none; background-color: #EEEEEE; height: 15px; border-top-left-radius: 7px; border-top-right-radius: 7px; subcontrol-position: top; subcontrol-origin: margin; margin: 28px 0 0px 0;}"
		" QScrollBar::sub-line:vertical:hover { background-color: #FFFFFF; }"
		" QScrollBar::sub-line:vertical:pressed{ background-color: #FFFFFF; }"
		" QScrollBar::add-line:vertical { border: none;	background-color: #EEEEEE; height: 15px; border-bottom-left-radius: 7px; border-bottom-right-radius: 7px; subcontrol-position: bottom;  subcontrol-origin: margin; margin: 0px 0 2px 0;}"
		" QScrollBar::add-line:vertical:hover { background-color: #FFFFFF; }"
		" QScrollBar::add-line:vertical:pressed {  background-color: #FFFFFF;}"
		" QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical { background: none; }"
		" QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }"
		//horizon scroll
		" QTableWidget QScrollBar:horizontal { border:none; color: #282A2E; height: 14px; margin: 0px 0px 0px 0px; border-radius: 7px; }"
		" QTableWidget QScrollBar::handle:horizontal { background-color: #EEEEEE; min-width: 20px; border-radius: 7px;}"
		" QScrollBar::handle:horizontal:hover {background-color: #FFFFFF; }"
		" QScrollBar::sub-line:horizontal { border: none; background-color: #EEEEEE; width: 15px; border-top-left-radius: 7px; border-bottom-left-radius: 7px; subcontrol-position: left; subcontrol-origin: margin; }"
		" QScrollBar::sub-line:horizontal:hover { background-color: #FFFFFF; }"
		" QScrollBar::sub-line:horizontal:pressed{ background-color: #FFFFFF; }"
		" QScrollBar::add-line:horizontal { border: none;	background-color: #EEEEEE; width: 15px; border-top-right-radius: 7px; border-bottom-right-radius: 7px; subcontrol-position: right;  subcontrol-origin: margin; }"
		" QScrollBar::add-line:horizontal:hover { background-color: #FFFFFF; }"
		" QScrollBar::add-line:horizontal:pressed {  background-color: #FFFFFF;}"
		" QScrollBar::up-arrow:horizontal, QScrollBar::down-arrow:horizontal { background: none; }"
		" QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: none; }" ;

	QString customtree = "QWidget { background-color: #2E3135; border:1px solid #444444; color:#FFFFFF;}"
		" QTreeView {show-decoration-selected: 1;}"
		" QTreeView::item {	border:none;	color:#FFFFFF; }"
		" QTreeView::item:selected	{	background: #666666; 	}"
		//arrow
		"QTreeView::branch:closed:has-children {"
		"border-image: none; "
		"image: url(:/optimize/resource/optimize/tree_right.png); "
		"}"
		"QTreeView::branch:open:has-children {"
		"border-image: none; "
		"image: url(:/optimize/resource/optimize/tree_down.png); "
		"}"
		//vertical scroll
		" QTreeView QScrollBar:vertical{ border:none; background: #282A2E; width: 14px; margin: 0px 0 2px 0; border-radius: 7px; }"
		" QTreeView QScrollBar::handle:vertical { background-color: #EEEEEE; min-height: 20px; border-radius: 7px;}"
		" QScrollBar::handle:vertical:hover {background-color: #FFFFFF; }"
		" QScrollBar::sub-line:vertical { border: none; background-color: #EEEEEE; height: 15px; border-top-left-radius: 7px; border-top-right-radius: 7px; subcontrol-position: top; subcontrol-origin: margin; margin: 0px 0 0px 0;}"
		" QScrollBar::sub-line:vertical:hover { background-color: #FFFFFF; }"
		" QScrollBar::sub-line:vertical:pressed{ background-color: #FFFFFF; }"
		" QScrollBar::add-line:vertical { border: none;	background-color: #EEEEEE; height: 15px; border-bottom-left-radius: 7px; border-bottom-right-radius: 7px; subcontrol-position: bottom;  subcontrol-origin: margin; margin: 0px 0 2px 0;}"
		" QScrollBar::add-line:vertical:hover { background-color: #FFFFFF; }"
		" QScrollBar::add-line:vertical:pressed {  background-color: #FFFFFF;}"
		" QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical { background: none; }"
		" QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical { background: none; }"
		//horizon scroll
		" QTreeView QScrollBar:horizontal { border:none; color: #2E3135; height: 14px; margin: 0px 0px 0px 0px; border-radius: 7px; }"
		" QTreeView QScrollBar::handle:horizontal { background-color: #EEEEEE; min-width: 20px; border-radius: 7px;}"
		" QScrollBar::handle:horizontal:hover {background-color: #FFFFFF; }"
		" QScrollBar::sub-line:horizontal { border: none; background-color: #EEEEEE; width: 15px; border-top-left-radius: 7px; border-bottom-left-radius: 7px; subcontrol-position: left; subcontrol-origin: margin; }"
		" QScrollBar::sub-line:horizontal:hover { background-color: #FFFFFF; }"
		" QScrollBar::sub-line:horizontal:pressed{ background-color: #FFFFFF; }"
		" QScrollBar::add-line:horizontal { border: none;	background-color: #EEEEEE; width: 15px; border-top-right-radius: 7px; border-bottom-right-radius: 7px; subcontrol-position: right;  subcontrol-origin: margin; }"
		" QScrollBar::add-line:horizontal:hover { background-color: #FFFFFF; }"
		" QScrollBar::add-line:horizontal:pressed {  background-color: #FFFFFF;}"
		" QScrollBar::up-arrow:horizontal, QScrollBar::down-arrow:horizontal { background: none; }"
		" QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal { background: none; }";
}